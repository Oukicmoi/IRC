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

// Set/remove Invite-only channel
void	Server::handleMode_i(User* user, Channel* channel, bool sign)
{
	channel->_inviteOnly = sign;
}

//  Set/remove the restrictions of the TOPIC command to channel operators
void	Server::handleMode_t(User* user, Channel* channel, bool sign)
{
	channel->_topicRestricted = sign;
}

// Set/remove the channel key (password)
void	Server::handleMode_k(User* user, Channel* channel, std::string password)
{

}

// Give/take channel operator privilege
void	Server::handleMode_o(User* user, Channel* channel)
{

}

//  Set/remove the user limit to channel
void	Server::handleMode_l(User* user, Channel* channel, std::string limit)
{

}

void	Server::applyChannelModes(User* user, Channel* channel, std::vector<std::string> params)
{
	// std::string change = "MODE " + c->ge
	// 
	// tName() + " " + m;
	std::vector<std::string>::iterator it = params.begin();
	while (it != params.end())
	{
		if (*it.empty())
			it ++;
		else if (((*it)[0] != '-') && ((*it)[0] != '+'))
        	return sendToUser(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));
		else
		{

		}
	}
	
	// c->broadcast(":" + user->getPrefix() + " " + change);
}

void Server::cmd_MODE(User* user, IRCMessage& msg)
{
    std::vector<std::string>& params = msg.getParams();
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