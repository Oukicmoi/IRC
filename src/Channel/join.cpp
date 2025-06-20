#include "all.hpp"

void	Channel::sendWelcomeInfo(User *user)
{
	// envoi du topic du channel
	if (_topic.empty())
		_server.sendWhenReady(user->getSocketFd(), RPL_NOTOPIC(user->getNick(), _name));
	else
		_server.sendWhenReady(user->getSocketFd(), RPL_TOPIC(user->getNick(), _name, _topic));

	// envoi de la liste des nick des users du channel (NAMES)
	_server.sendWhenReady(user->getSocketFd(), RPL_NAMREPLY(user->getNick(), "=", _name, getNickList()));
	_server.sendWhenReady(user->getSocketFd(), RPL_ENDOFNAMES(user->getNick(), _name));
}

bool	Channel::canUserJoin(User *user, const std::string *password)
{
	if (_hasKey)
	{
		if (!password)
			return (_server.sendWhenReady(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "JOIN")), false);
		std::string invalidKeyReason = Server::isValidKey(*password);
		if (!invalidKeyReason.empty())
			return (_server.sendWhenReady(user->getSocketFd(), ERR_INVALIDKEY(user->getNick(), _name, invalidKeyReason)), false);
		if (*password != _key)
			return (_server.sendWhenReady(user->getSocketFd(), ERR_BADCHANNELKEY(user->getNick(), _name)), false);
	}
	if (_inviteOnly && !userOnInviteList(user))
	{
		
		return (_server.sendWhenReady(user->getSocketFd(), ERR_INVITEONLYCHAN(user->getNick(), _name)), false);
	}
	if ((_userLimit >= 0) && (_userLimit <= getSize()))
		return (_server.sendWhenReady(user->getSocketFd(), ERR_CHANNELISFULL(user->getNick(), _name)), false);
	return (true);
}

bool Channel::userJoin(User *user, std::string *password)
{  
    if (!canUserJoin(user, password))
			return (false);
		
	if (_inviteOnly)
		rmFromInviteList(user);
	if (!addMember(user))
		return (_server.sendWhenReady(user->getSocketFd(), ERR_ALREADYJOINED(user->getNick(), _name)), false);
	_membersJoinOrder[std::time(NULL)] = user;
	return (true);
}


