/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:33:39 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/04 18:33:39 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

static std::vector<std::string> splitComma(const std::string& s)
{
    std::vector<std::string> out;
    std::string token;
    std::istringstream iss(s);
    while (std::getline(iss, token, ','))
    {
        if (!token.empty())
            out.push_back(token);
    }
    return out;
}

// Kick un seul utilisateur du channel (C++98, <30 lignes)
void Server::kickOneUser(User* kicker, Channel* chan, const std::string& targetNick, const std::string& reason)
{
    User* target = NULL;
    // Recherche du User* correspondant à targetNick
    for (std::map<int, User*>::iterator itUser = _users.begin(); itUser != _users.end(); ++itUser)
    {
        if (itUser->second->getNick() == targetNick)
        {
            target = itUser->second;
            break;
        }
    }
    // ERR_USERNOTINCHANNEL si l'utilisateur n'existe pas ou n'est pas membre
    if (target == NULL || ! chan->isMember(target))
                return(sendServerRpl(kicker->getSocketFd(), ERR_USERNOTINCHANNEL(kicker->getNick(), targetNick, chan->getName())),(void)target);
    
    if(reason.empty())
        chan->broadcast(RPL_KICK(chan->getName(), targetNick, ":No reason"), NULL);
    else
        chan->broadcast(RPL_KICK(chan->getName(), targetNick, (":" + reason)), NULL);
    chan->removeMember(target);
}

void Server::cmd_KICK(User* user, const IRCMessage& msg)
{
    const std::vector<std::string>& p = msg.getParams();
    if (p.size() < 2)
        return(sendServerRpl(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "KICK")), (void)p);

    msg.printParams();
    const std::string& channelName = p[0];
    std::string userList = p[1];
    std::string reason = "";
    if (p.size() >= 3)
        reason = p[2];

    // Vérifier que le channel existe
    std::map<std::string, Channel*>::iterator itChan = _channels.find(channelName);
    if (itChan == _channels.end())
        return(sendServerRpl(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), channelName)),(void)p);
    Channel* chan = itChan->second;

    chan->printMembers();
    // Vérifier que l'utilisateur courant est opérateur
    if (!chan->isOperator(user))
        return(sendServerRpl(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), channelName)), (void)p);
    std::cout << "HEREEEEEEEE" << std::endl;
    // Découper la liste des cibles séparées par ','
    std::vector<std::string> targets = splitComma(userList);
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
        kickOneUser(user, chan, *it, reason);

    // Si le channel est devenu vide, on le supprime
    if (chan->getMembers().empty())
        return(_channels.erase(channelName), delete chan, (void)p);
}
