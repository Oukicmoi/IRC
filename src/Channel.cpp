/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:54:36 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/18 23:33:31 by gtraiman         ###   ########.fr       */
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



