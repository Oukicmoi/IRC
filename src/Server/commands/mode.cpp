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

void Server::cmd_MODE(User* user, const IRCMessage& msg)
{
    const std::vector<std::string>& p = msg.getParams();
    if (p.size() < 1)
        return sendToUser(user->getSocketFd(), ERR_UNKNOWNCOMMAND(user->getNick(), "MODE"));

    if (p[0].empty() || p[0][0] != '#')
        return sendToUser(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));

    Channel* c = getChannels()[p[0]];
    if (!c)
        return sendToUser(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), p[0]));

    if (p.size() == 1)
        return sendChannelModesToUser(user, c , p);

    if (!c->isOperator(user))
        return sendToUser(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), p[0]));

    applyChannelModes(user, c, p);
}


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

void	Server::applyChannelModes(User* user, Channel* c, const std::vector<std::string>& p)
{
	std::string m = p[1];
	bool add = true;
	size_t i = 2;
	std::string change = "MODE " + c->getName() + " " + m;

	for (size_t j = 0; j < m.size(); j++)
	{
		if (m[j] == '+')
			add = true;
		else if (m[j] == '-')
			add = false;
		else
		{
			continue; // ou return; selon la logique que tu veux
		}
		if (!handleMode(user, c, p, i, m[j], add))
			continue;
	}

	for (size_t k = 2; k < i; k++)
		change += " " + p[k];

	c->broadcast(":" + user->getPrefix() + " " + change);
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
