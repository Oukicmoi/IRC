#include "all.hpp"

// Set/remove Invite-only channel -+i
void	Channel::mode_invite(bool sign, std::string &userPrefix)
{
	_inviteOnly = sign;
	broadcast(RPL_MODE(userPrefix, _name, (sign? "+" : "-") + "i"));
}

//  Set/remove the restrictions of the TOPIC command to channel operators
void	Channel::mode_topicRestriction(bool sign, std::string &userPrefix)
{
	_topicRestricted = sign;
	broadcast(RPL_MODE(userPrefix, _name, (sign? "+" : "-") + "t"));
}

// Set/remove the channel key (password)
void	Channel::mode_key(bool sign, User *user, std::string *password)
{
	if (sign && !password)
		return _server.sendWhenReady(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "MODE"));
		
	if (sign)
	{
		std::string invalidKeyReason = Server::isValidKey(*password);
		if (!invalidKeyReason.empty())
			return _server.sendWhenReady(user->getSocketFd(), ERR_INVALIDKEY(user->getNick(), _name, invalidKeyReason));
		_key = *password;	
		broadcast(RPL_MODE_WITH_ARG(user->getFullNameMask(), _name, (sign? "+" : "-") + "k", _key));
	}
	else
		broadcast(RPL_MODE(user->getFullNameMask(), _name, "-k"));
	_hasKey = sign;
}

// Give/take channel operator privilege
void	Channel::mode_operators(bool sign, User *user, User *target)
{
	if (_members.find(target) == _members.end())
		_server.sendWhenReady(user->getSocketFd(), ERR_USERNOTINCHANNEL(user->getNick(), target->getNick(), _name));
	else
	{
		bool is_operator = (_operators.find(target) != _operators.end());
		if (sign && is_operator)
			_server.sendWhenReady(user->getSocketFd(), ERR_ALREADYCHANOPRIVS(user->getNick(), _name));
		else if (!sign && !is_operator)
			_server.sendWhenReady(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), _name));
		else
		{
			if (sign)
				_operators.insert(target);
			else
			{
				if (_operators.size() == 1)
					return _server.sendWhenReady(user->getSocketFd(), ERR_LASTCHANOP(user->getNick(), _name));
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
	if (sign && limit)
	{
		try
		{
			_userLimit = (int)typeOfString<unsigned int>(*limit);
			broadcast(RPL_MODE_WITH_ARG(user->getFullNameMask(), _name, "+l", *limit));
		}
		catch (std::exception &e)
		{
			_server.sendWhenReady(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "l", "Invalid limit parameter (must be a positive number)"));
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
				_server.sendWhenReady(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "k", "You must specify a parameter for the key mode"));
			else
			{
				mode_key(sign, user, &(*password));
				params.erase(password);
			}
		}
		else
			mode_key(sign, user);
	}
	else if (mode == 'o')
	{
		std::vector<std::string>::iterator targetNick = msg.getFirstNonModeArg();
		if (targetNick == params.end())
			_server.sendWhenReady(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "l", "You must specify a parameter for the operator mode"));
		else
		{
			User *target = _server.getUserByNick(*targetNick);
			if (!target)
				_server.sendWhenReady(user->getSocketFd(), ERR_NOSUCHNICK(user->getNick(), *targetNick));
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
				_server.sendWhenReady(user->getSocketFd(), ERR_INVALIDMODEPARAM(user->getNick(), _name, "l", "You must specify a parameter for the limit mode"));
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
		_server.sendWhenReady(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));
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
		_server.sendWhenReady(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));
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
    _server.sendWhenReady(user->getSocketFd(), RPL_CHANNELMODEIS(user->getNick(), _name, modes, args));
}
