/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:33:52 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/18 23:43:44 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "all.hpp"
// Dans Server.hpp, assurez-vous d’avoir :
// #include <map>
// #include <string>
// #include <vector>

// typedef pour alléger le code
typedef std::map<std::string,Channel*> ChannelMap;

Channel* Server::getOrCreateChannel(const std::string& name, User& u)
{
    // itérateur explicite
    ChannelMap::iterator it = _channels.find(name);
    if (it != _channels.end())
        return it->second;

    // pas trouvé, on crée
    Channel* ch = new Channel(name);
    ch->addOperator(&u);
    _channels[name] = ch;
    return ch;
}

void Server::cmd_JOIN(User* u, const std::vector<std::string>& params)
{
    // params[0] == "#channel"
    std::string chname = params[0];
    Channel*    ch = getOrCreateChannel(chname, *u);

    // si déjà membre
    if (! ch->addMember(u))
    {
        // ERR_ALREADYJOINED
        return;
    }

    // on notifie tout le canal
    // std::string joinMsg = RPL_JOIN(user_id(u->getNick(), u->getUsername()), chname);
    // ch->broadcast(joinMsg);
}

void Server::cmd_PART(User* u, const std::vector<std::string>& params)
{
    std::string chname = params[0];
    ChannelMap::iterator it = _channels.find(chname);

    if (it == _channels.end() || ! it->second->isMember(u))
    {
        // ERR_NOTONCHANNEL
        return;
    }

    Channel* ch = it->second;
    // on envoie le PART avant de retirer
    // std::string partMsg = RPL_PART(user_id(u->getNick(), u->getUsername()),chname,"");
    // ch->broadcast(partMsg);

    ch->removeMember(u);

    // si canal vide, on supprime
    if (ch->getMembers().empty())
    {
        delete ch;
        _channels.erase(it);
    }
}

void Server::cmd_PRIVMSG(User* u, const std::vector<std::string>& params)
{
    std::string target = params[0];
    std::string msg    = params[1];

    if (! target.empty() && target[0] == '#')
    {
        ChannelMap::iterator it = _channels.find(target);
        if (it == _channels.end() || ! it->second->isMember(u))
        {
            // ERR_CANNOTSENDTOCHAN
            return;
        }
        Channel* ch = it->second;
        std::string fmt = RPL_PRIVMSG(u->getNick(), u->getUsername(), target,msg);
        // broadcast en excluant l’envoyeur
        ch->broadcast(fmt, u);
    }
    else
    {
        // message privé à un autre User
        // ex. lookup dans _users par nick, puis sendServerRpl(...)
    }
}

void Server::cmd_TOPIC(User* u, const std::vector<std::string>& params)
{
    std::string chname = params[0];
    ChannelMap::iterator it = _channels.find(chname);

    if (it == _channels.end() || ! it->second->isMember(u))
    {
        // ERR_NOTONCHANNEL
        return;
    }

    Channel* ch = it->second;

    if (params.size() == 1)
    {
        // simple affichage
        std::string r = RPL_TOPIC(u->getNick(),chname,ch->getTopic());
        sendServerRpl(u->getSocketFd(), r);
    }
    else
    {
        // changement de topic (vérifier +t si nécessaire)
        ch->setTopic(params[1]);
        std::string ntf = RPL_TOPIC(u->getNick(),chname,params[1]);
        ch->broadcast(ntf);
    }
}

void	sendServerRpl(int const client_fd, std::string client_buffer)
{
	std::istringstream	buf(client_buffer);
	std::string			reply;
	
	send(client_fd, client_buffer.c_str(), client_buffer.size(), 0);
	while (getline(buf, reply))
	{
		std::cout << "[Server] Message sent to client " \
				  << client_fd << "       >> " << CYAN << reply << R << std::endl;
	}
}

void Channel::broadcast(const std::string& message, User* except) const
{
    for (std::set<User*>::const_iterator it = _members.begin();
         it != _members.end();
         ++it)
    {
        User* u = *it;
        if (u == except)
            continue;
        // envoie le message à chaque membre (sauf « except »)
        sendServerRpl(u->getSocketFd(), message);
    }
}