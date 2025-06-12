/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:54:36 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/12 23:23:34 by octoross         ###   ########.fr       */
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
	
                                                          
const std::string& Channel::getName()  const { return _name; }
const std::set<User*>& Channel::getMembers() const { return(_members); }

Channel* Server::getChannelByName(const std::string& name)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it == _channels.end())
        return NULL;
    return it->second;
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
bool Channel::isMember(User* u) const { return (_members.find(u) != _members.end()); }


bool Channel::addOperator(User* u)
{
    if (!isMember(u))
        return (false);
    _operators.insert(u);
    return (true);
}
void Channel::removeOperator(User* u) { _operators.erase(u); }
bool Channel::isOperator(User* u) const { return (_operators.find(u) != _operators.end()); }


typedef std::map<std::string,Channel*> ChannelMap;

Channel* Server::getOrCreateChannel(const std::string& name, User& u)
{  
    // itérateur explicite
    Channel* ch;
    ChannelMap::iterator it = _channels.find(name);
    if (it != _channels.end())
        ch =  it->second;
    else        
        ch = new Channel(name, *this);

    // pas trouvé, on crée
    if (!ch->addMember(&u))
        return(sendToUser(u.getSocketFd(), ERR_ALREADYJOINED(u.getNick(), name)), ch);
    if (it == _channels.end())
        ch->addOperator(&u);
    _channels[name] = ch;
    return ch;
}


void Channel::broadcast(const std::string& message, User* except) const
{
    for (std::set<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        User* u = *it;
        if (u == except)
            continue;
        // envoie le message à chaque membre (sauf « except »)
        Server::sendToUser(u->getSocketFd(), message);
    }
} 


// TOPIC
void Channel::setTopic(const std::string& topic) { _topic = topic; }
std::string Channel::getTopic() const { return _topic; }

void Channel::setTopicSetter(const std::string& setter) { _topicSetter = setter; }
std::string Channel::getTopicSetter() const { return _topicSetter; }

void Channel::setTopicSetTime(std::time_t t) { _topicSetTime = t; }
std::time_t Channel::getTopicSetTime() const { return _topicSetTime; }

bool Channel::isTopicProtected() const { return _topicRestricted; } // +t


void Channel::printOperatorse() const
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

// MODE changes


// Set/remove Invite-only channel -+i
void	Channel::mode_invite(bool sign, std::string &userPrefix)
{
	_inviteOnly = sign;
	std::cout << "MODE i: sign " << sign << std::endl;
	broadcast(":" + userPrefix + " MODE #" + _name + " " + (sign? "+" : "-") + "i\r\n");
}

//  Set/remove the restrictions of the TOPIC command to channel operators
void	Channel::mode_topicRestriction(bool sign, std::string &userPrefix)
{
	_topicRestricted = sign;
	std::cout << "MODE t: sign " << sign << std::endl;
	broadcast(":" + userPrefix + " MODE #" + _name + " " + (sign? "+" : "-") + "t\r\n");
}

// Set/remove the channel key (password)
void	Channel::mode_key(bool sign, std::string &userPrefix, std::string *password)
{
	_hasKey = sign;
	std::cout << "MODE k: sign " << sign << *password << std::endl;
	if (_hasKey && password)
	{
		_key = *password;
		broadcast(":" + userPrefix + " MODE #" + _name + " " + (sign? "+" : "-") + "k" + " " + _key + "\r\n");
	}
	else
	{
		_hasKey = false;
		broadcast(":" + userPrefix + " MODE #" + _name + " " + "-k\r\n");
	}
}

// Give/take channel operator privilege
void	Channel::mode_operators(bool sign, User *user, User *target)
{
	if (_members.find(target) == _members.end())
		Server::sendToUser(user->getSocketFd(), ERR_USERNOTINCHANNEL(user->getNick(), target->getNick(), _name));
	else
	{
		bool is_operator = (_operators.find(target) != _operators.end());
		if (sign && is_operator)
			Server::sendToUser(user->getSocketFd(), ":" + _server.getServerName() + " 482 " + user->getNick() + " #" + _name + " :User is already an operator\r\n");
		else if (!sign && !is_operator)
			Server::sendToUser(user->getSocketFd(), ":" + _server.getServerName() + " 482 " + user->getNick() + " #" + _name + " :User is not an operator\r\n");
		else
		{
			if (sign)
				_operators.insert(target);
			else
			{
				if (_operators.size() == 1)
					return Server::sendToUser(user->getSocketFd(), ERR_LASTCHANOP(user->getNick(), _name));
				else
					_operators.erase(target);
			}
			broadcast(":" + user->getPrefix() + " MODE #" + _name + " " + (sign ? "+" : "-") + "o" + + "\r\n");
		}
		
	}
}

//  Set/remove the user limit to channel
void	Channel::mode_userLimit(bool sign, User *user, std::string *limit)
{
	std::cout << "MODE l: sign " << sign << *limit << std::endl;
	if (sign && limit)
	{
		try
		{
			_userLimit = (int)typeOfString<unsigned int>(*limit);
			broadcast(":" + user->getPrefix() + " MODE #" + _name + " " + "+l" + " " + *limit + "\r\n");
		}
		catch (std::exception &e)
		{
			Server::sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(_server.getServerName(), _name, "l", "Invalid limit parameter (must be a positive number)"));
		}
	}
	else
	{
		_userLimit = -1;
		broadcast(":" + user->getPrefix() + " MODE #" + _name + " " + "-l\r\n");
	}
}