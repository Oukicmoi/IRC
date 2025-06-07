/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:46:13 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/01 23:46:13 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void	Server::sendChannelModesToUser(User* user, Channel* c, const std::vector<std::string>& p)
{
	std::string modes = "+";
	std::string args = "";
    std::stringstream ss;

	if (c->isInviteOnly())
        modes += "i";
	if (c->isTopicRestricted())
        modes += "t";
	if (!c->getKey().empty())
    {
        modes += "k";
        args += " " + c->getKey();
    }
	if (c->getUserLimit() != -1)
    {
		modes += "l";
        ss << c->getUserLimit();
        args += " " + ss.str();

	}
    sendToUser(user->getSocketFd(), RPL_CHANNELMODEIS(user->getNick(), p[0], modes + args));
}


bool	Server::handleMode(User* u, Channel* c, const std::vector<std::string>& p, size_t& i, char m, bool add)
{
    (void)u;
	if (m == 'i')
        c->setInviteOnly(add);
	else if (m == 't')
        c->setTopicRestricted(add);
	else if (m == 'k')
    {
		if (add && i >= p.size())
            return false;
		add ? c->setKey(p[i++]) : c->removeKey();
	}
	else if (m == 'l') {
		if (add)
        {
			if (i >= p.size())
                return false;
			int n = std::atoi(p[i++].c_str());
			if (n > 0)
                c->setUserLimit(n);
		}
        else
            c->removeUserLimit();
	}
	else
        return false;
	return true;
}


void	Server::applyChannelModes(User* user, Channel* c, const std::vector<std::string>& params)
{
	// std::string change = "MODE " + c->getName() + " " + m;

	
	
	// c->broadcast(":" + user->getPrefix() + " " + change);
}

void Server::cmd_MODE(User* user, const IRCMessage& msg)
{
    const std::vector<std::string>& params = msg.getParams();
    if (params.size() < 1)
        return sendToUser(user->getSocketFd(), ERR_UNKNOWNCOMMAND(user->getNick(), "MODE"));

    if (params[0].empty() || (params[0][0] != '#'))
        return sendToUser(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));

    Channel* channel = getChannels()[params[0]];
    if (!channel)
        return sendToUser(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), params[0]));

    if (params.size() == 1)
        return sendChannelModesToUser(user, channel , params);

    if (!channel->isOperator(user))
        return sendToUser(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), params[0]));

    applyChannelModes(user, channel, params);
}