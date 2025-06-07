/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:54:36 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/07 21:19:19 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

Channel::Channel(const std::string& name): _name(name),  _topic(""),  _inviteOnly(false),  _topicRestricted(false),  _key(""),  _userLimit(-1),  _creationTime(std::time(NULL)) {}
                                                          

const std::string& Channel::getName()  const
{
    return _name;
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
    return (_members.find(u) != _members.end());
}

bool Channel::addOperator(User* u)
{
    if (!isMember(u))
        return (false);
    _operators.insert(u);
    return (true);
}
void Channel::removeOperator(User* u)
{
    _operators.erase(u);
}
bool Channel::isOperator(User* u) const
{
    return (_operators.find(u) != _operators.end());
}


typedef std::map<std::string,Channel*> ChannelMap;

Channel* Server::getOrCreateChannel(const std::string& name, User& u)
{  
    // itérateur explicite
    Channel* ch;
    ChannelMap::iterator it = _channels.find(name);
    if (it != _channels.end())
        ch =  it->second;
    else        
        ch = new Channel(name);

    // pas trouvé, on crée
    if (!ch->addMember(&u))
        return(sendToUser(u.getSocketFd(), ERR_ALREADYJOINED(u.getNick(), name)), ch);
    if (it == _channels.end())
        ch->addOperator(&u);
    _channels[name] = ch;
    return ch;
}

const std::map<std::string, Channel *>& Server::getChannels() const
{
	return _channels;
}

std::map<std::string, Channel *>& Server::getChannels()
{
	return _channels;
}

// void Server::setChannels(const std::map<std::string, Channel *>& channels)
// {
// 	_channels = channels;
// }

void Channel::broadcast(const std::string& message, User* except) const
{
    for (std::set<User*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        User* u = *it;
        if (u == except)
            continue;
        // envoie le message à chaque membre (sauf « except »)
        sendToUser(u->getSocketFd(), message);
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




void Channel::printOpe() const
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