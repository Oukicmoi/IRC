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


std::vector<std::string>::iterator	Server::getFirstNonModeArg(std::vector <std::string> &params)
{
	std::vector<std::string>::iterator it = params.begin();
	it ++;
	while (it != params.end())
	{
		if ((*it).empty())
			return (it);
		else if (((*it)[0] != '-') && ((*it)[0] != '+'))
			return (it);
		it ++;
	}
	return (it);
}

void	Server::applyChannelMode(char mode, bool sign, User *user, Channel *channel, std::vector<std::string> &params)
{
	if (mode == 'i')
		channel->mode_invite(sign);
	else if (mode == 't')
		channel->mode_topicRestriction(sign);
	else if (mode == 'k')
	{
		if (sign)
		{
			std::vector<std::string>::iterator it = getFirstNonModeArg(params);
			if (it == params.end())
				sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(_server_name, channel->getName(), "k", "You must specify a parameter for the key mode"));
			else
			{
				channel->mode_key(sign, &(*it));
				params.erase(it);
			}
		}
		else
			channel->mode_key(sign);
	}
	else if (mode == 'o')
	{
		std::vector<std::string>::iterator it = getFirstNonModeArg(params);
		if (it == params.end())
			sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(_server_name, channel->getName(), "l", "You must specify a parameter for the operator mode"));
		else
		{
			channel->mode_operators(sign, user, *it);
			params.erase(it);
		}
	}
	else if (mode == 'l')
	{
		if (sign)
		{
			std::vector<std::string>::iterator it = getFirstNonModeArg(params);
			if (it == params.end())
				sendToUser(user->getSocketFd(), ERR_INVALIDMODEPARAM(_server_name, channel->getName(), "l", "You must specify a parameter for the limit mode"));
			else
			{
				channel->mode_userLimit(sign, &(*it));
				params.erase(it);
			}
		}
		else
			channel->mode_userLimit(sign);
	}
	else
		sendToUser(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));
}


void	Server::applyChannelModes(User* user, Channel* channel, std::vector<std::string> &params)
{
	// std::string change = "MODE " + c->ge
	// 
	// tName() + " " + m;
	while (!params.empty())
	{
		// bad parameter
		std::string &param = params[0];
		if (param.empty())
			params.erase(params.begin());
		bool	sign = true;
		if (param[0] == '-')
			sign = false;
		if (sign && (param[0] != '+'))
			;
		else
		{
			unsigned int i = 1;
			while (i < param.size())
				applyChannelMode(param[i ++], sign, user, channel, params);
		}
		params.erase(params.begin());
	}
	// c->broadcast(":" + user->getPrefix() + " " + change);
}

void	Server::sendChannelModesToUser(User* user, Channel* channel, const std::vector<std::string>& params)
{
	std::string modes = "+";
	std::string args = "";
    std::stringstream ss;

	if (channel->isInviteOnly())
        modes += "i";
	if (channel->isTopicRestricted())
        modes += "t";
	if (!channel->hasKey())
    {
        modes += "k";
        args += " " + channel->getKey();
    }
	if (channel->getUserLimit() != -1)
    {
		modes += "l";
        ss << channel->getUserLimit();
        args += " " + ss.str();
	}
    sendToUser(user->getSocketFd(), RPL_CHANNELMODEIS(user->getNick(), params[0], modes + args));
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
	// TODO: laisser admin serveur changer 

    applyChannelModes(user, channel, params);
}