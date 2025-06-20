/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:54:36 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/20 22:38:41 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

Channel::Channel(const std::string& name, Server &server):
	_inviteOnly(false),
	_topicRestricted(false),
	_hasKey(false),
	_userLimit(-1),
	_creationTime(std::time(NULL)),
	_topicSetTime(std::time(NULL)),
	_name(name),
	_key(""),
	_topic(""),
	_topicSetter(""),
	_server(server) {}

bool Channel::addMember(User* u)
{
    std::pair<std::set<User*>::iterator,bool> result = _members.insert(u);
    return (result.second);
}

void Channel::removeMember(User* user)
{
    _members.erase(user);
    _operators.erase(user);
	std::map<time_t, User *>::iterator it = _membersJoinOrder.begin();
	while (it != _membersJoinOrder.end())
	{
		if (it->second == user)
		{
			_membersJoinOrder.erase(it);
			break;
		}
		it ++;
	}
	if (_operators.empty() && !(_membersJoinOrder.empty()))
	{
		User *newOperator = _membersJoinOrder.begin()->second;
		_operators.insert(newOperator);
		broadcast(RPL_SERVER_MODE(_name,  "+o", newOperator->getNick()), user);
	}
	
}


void Channel::addOperator(User* u)
{
	_members.insert(u);
	_operators.insert(u);
	_membersJoinOrder[std::time(NULL)] = u;
}
void Channel::removeOperator(User* user) { _operators.erase(user); }


std::string	Channel::getNickList() const
{
	std::string nickList;
	for (std::set<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if (!nickList.empty())
			nickList += " ";
		if (isOperator(*it))
			nickList += "@";
		nickList += (*it)->getNick();
	}
	return (nickList);
}

void Channel::broadcast(const std::string& message, User* except) const
{
    for (std::set<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        User* u = *it;
        if (u == except)
            continue;
        // envoie le message à chaque membre (sauf « except »)
        _server.sendWhenReady(u->getSocketFd(), message);
    }
} 


void Channel::printOperators() const
{
    std::cout << "Operators on channel " << _name << " (“ operators.size() = " << _operators.size() << "):\n";
    for (std::set<User*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
        std::cout << "  - " << (*it)->getNick() << "\n";
}

void Channel::printMembers() const
{
    std::cout << "Members on channel " << _name << " (“ _members.size() = " << _members.size() << "):\n";
    for (std::set<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
        std::cout << "  - " << (*it)->getNick() << "\n";
}

void Channel::printInviteList() const
{
    std::cout << "Invite users on channel " << _name << " (“ _inviteList.size() = " << _inviteList.size() << "):\n";
    for (std::set<User*>::const_iterator it = _inviteList.begin(); it != _inviteList.end(); ++it)
        std::cout << "  - " << (*it)->getNick() << "\n";
}
