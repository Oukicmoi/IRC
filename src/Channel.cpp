/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:54:36 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/20 19:49:47 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

Channel::Channel(const std::string& name) : _name(name) {}                                                             

const std::string& Channel::getName()  const
{
    return _name;
}
const std::string& Channel::getTopic() const
{
    return _topic;
}

const std::set<User*>& Channel::getMembers() const
{
    return(_members);
}


bool Channel::addMember(User* u)
{
    std::pair<std::set<User*>::iterator,bool> result = _members.insert(u);
    return result.second;
}

void Channel::removeMember(User* u)
{
    _members.erase(u);
    _operators.erase(u);
}
bool Channel::isMember(User* u) const
{
    return _members.count(u) != 0;
}

bool Channel::addOperator(User* u)
{
    if (!isMember(u)) return false;
    _operators.insert(u);
    return true;
}
void Channel::removeOperator(User* u)
{
    _operators.erase(u);
}
bool Channel::isOperator(User* u) const
{
    return _operators.count(u) != 0;
}

void Channel::setTopic(const std::string& t)
{
    _topic = t;
}

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