/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:54:36 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/14 04:36:44 by octoross         ###   ########.fr       */
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
}


bool Channel::addOperator(User* u)
{
    if (!isMember(u))
		return (false);
	_operators.insert(u);
	return (true);
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


void	Channel::sendWelcomeInfo(User *user)
{
	// envoi du topic du channel
	if (_topic.empty())
		Server::sendToUser(user->getSocketFd(), RPL_NOTOPIC(user->getNick(), _name));
	else
		Server::sendToUser(user->getSocketFd(), RPL_TOPIC(user->getNick(), _name, _topic));

	// envoi de la liste des nick des users du channel (NAMES)
	Server::sendToUser(user->getSocketFd(), RPL_NAMREPLY(user->getNick(), "=", _name, getNickList()));
	Server::sendToUser(user->getSocketFd(), RPL_ENDOFNAMES(user->getNick(), _name));
}

bool Channel::userJoin(User *user, std::string *password)
{  
    if (!canUserJoin(user, password))
			return (false);
		
	if (_inviteOnly)
		rmFromInviteList(user);
	if (_members.find(user) != _members.end())
		return (Server::sendToUser(user->getSocketFd(), ERR_ALREADYJOINED(user->getNick(), _name)), false);
	return (true);
}

bool	Channel::canUserJoin(User *user, const std::string *password)
{
	if (_hasKey && (!password || (_key != *password)))
		return (Server::sendToUser(user->getSocketFd(), ERR_BADCHANNELKEY(user->getNick(), _name)), false); // ERR_INVALIDKEY -> valid password check TODO
	if (_inviteOnly && !userOnInviteList(user))
		return (Server::sendToUser(user->getSocketFd(), ERR_INVITEONLYCHAN(user->getNick(), _name)), false);
	if ((_userLimit >= 0) && (_userLimit <= getSize()))
		return (Server::sendToUser(user->getSocketFd(), ERR_CHANNELISFULL(user->getNick(), _name)), false);
	return (true);
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
	broadcast(RPL_MODE(userPrefix, _name, (sign? "+" : "-") + "i"));
}

//  Set/remove the restrictions of the TOPIC command to channel operators
void	Channel::mode_topicRestriction(bool sign, std::string &userPrefix)
{
	_topicRestricted = sign;
	std::cout << "MODE t: sign " << sign << std::endl;
	broadcast(RPL_MODE(userPrefix, _name, (sign? "+" : "-") + "t"));
}

// Set/remove the channel key (password)
void	Channel::mode_key(bool sign, std::string &userPrefix, std::string *password)
{
	_hasKey = sign;
	std::cout << "MODE k: sign " << sign << *password << std::endl;
	if (_hasKey && password)
	{
		_key = *password;
		broadcast(RPL_MODE_WITH_ARG(userPrefix, _name, (sign? "+" : "-") + "k", _key));
	}
	else
	{
		_hasKey = false;
		broadcast(RPL_MODE(userPrefix, _name, "-k"));
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
			Server::sendToUser(user->getSocketFd(), ERR_ALREADYCHANOPRIVS(user->getNick(), _name));
		else if (!sign && !is_operator)
			Server::sendToUser(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), _name));
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
			
			broadcast(RPL_MODE_WITH_ARG(user->getFullNameMask(), _name, (sign? "+" : "-") + "o", target->getNick()));
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
			broadcast(RPL_MODE_WITH_ARG(user->getFullNameMask(), _name, "+l", *limit));
		}
		catch (std::exception &e)
		{
			Server::sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "l", "Invalid limit parameter (must be a positive number)"));
		}
	}
	else
	{
		_userLimit = -1;
		broadcast(RPL_MODE(user->getFullNameMask(), _name, "-l"));
	}
}



void	Channel::applyMode(char mode, bool sign, User *user, IRCMessage &msg)
{
	std::string userPrefix = user->getFullNameMask();
	std::vector<std::string> &params = msg.getParams();
	if (mode == 'i')
		mode_invite(sign, userPrefix);
	else if (mode == 't')
		mode_topicRestriction(sign, userPrefix);
	else if (mode == 'k')
	{
		if (sign)
		{
			std::vector<std::string>::iterator password = msg.getFirstNonModeArg();
			if (password == params.end())
				Server::sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "k", "You must specify a parameter for the key mode"));
			else
			{
				mode_key(sign, userPrefix, &(*password));
				params.erase(password);
			}
		}
		else
			mode_key(sign, userPrefix);
	}
	else if (mode == 'o')
	{
		std::vector<std::string>::iterator targetNick = msg.getFirstNonModeArg();
		if (targetNick == params.end())
			Server::sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "l", "You must specify a parameter for the operator mode"));
		else
		{
			User *target = _server.getUserByNick(*targetNick);
			if (!target)
				Server::sendToUser(user->getSocketFd(), ERR_NOSUCHNICK(user->getNick(), *targetNick));
			else
				mode_operators(sign, user, target);
			params.erase(targetNick);
		}
	}
	else if (mode == 'l')
	{
		if (sign)
		{
			std::vector<std::string>::iterator limit = msg.getFirstNonModeArg();
			if (limit == params.end())
				Server::sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "l", "You must specify a parameter for the limit mode"));
			else
			{
				mode_userLimit(sign, user, &(*limit));
				params.erase(limit);
			}
		}
		else
			mode_userLimit(sign, user);
	}
	else
		Server::sendToUser(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));
}


// waits for any arg that starts with + or - and check all char inside to see if they are a known mode : ktoli
// 	-> if a mode is known, search for the first arg inside the params if needed (kol), and rm this arg from other modes args
//		Example: +kil lala 15 -> k will search for first param lala for password, then l will have 15 as first param
void	Channel::applyModes(User* user, IRCMessage &msg)
{
	bool	oneValid = false;
	std::vector<std::string> &params=  msg.getParams();
	while (!params.empty())
	{
		std::string &param = params[0];
		if (param.empty())
		{
			params.erase(params.begin());
			continue ;
		}
		bool	sign = (param[0] == '+');
		if (!sign && (param[0] != '-'))
			;
		else
		{
			oneValid = true;
			unsigned int i = 1;
			while (i < param.size())
			{
				if ((param[i] == '+') || (param[i] == '-'))
					sign = (param[i ++] == '+');
				else
					applyMode(param[i ++], sign, user, msg);
			}
		}
		params.erase(params.begin());
	}
	if (!oneValid)
		Server::sendToUser(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));
}

void	Channel::sendModesToUser(User* user)
{
	std::string modes = "+";
	std::string args = "";
    std::stringstream ss;

	if (isInviteOnly())
        modes += "i";
	if (isTopicRestricted())
        modes += "t";
	if (!hasKey())
    {
        modes += "k";
		if (!args.empty())
			args += " ";
        args += _key;
    }
	if (_userLimit != -1)
    {
		modes += "l";
        ss << _userLimit;
		
		if (!args.empty())
			args += " ";
        args += ss.str();
	}
    Server::sendToUser(user->getSocketFd(), RPL_CHANNELMODEIS(user->getNick(), _name, modes, args));
}
