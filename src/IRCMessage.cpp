/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:33:52 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/20 15:51:27 by octoross         ###   ########.fr       */
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
    for (std::set<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        User* u = *it;
        if (u == except)
            continue;
        // envoie le message à chaque membre (sauf « except »)
        sendServerRpl(u->getSocketFd(), message);
    }
}