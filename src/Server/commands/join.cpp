/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:51:18 by octoross          #+#    #+#             */
/*   Updated: 2025/06/14 22:52:55 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void	Server::userJoinChannel(User *user, const std::string &channelName, unsigned int key_index, std::vector<std::string> &keys)
{	
	Channel* channel = getChannelByName(channelName);
	if (!channel)
	{
		channel = createChannel(channelName, user);
		if (!channel)
			return ;
	}
	else
	{
		if (channel->hasKey() && (key_index < keys.size()))
		{
			if (!channel->userJoin(user, &keys[key_index ++]))
				return ;
		}
		else if (!channel->userJoin(user))
			return ;
	}
	
	// notif du JOIN a tous les membres du channel
	channel->broadcast(RPL_JOIN(user->getFullNameMask(), channelName));
	
	channel->sendWelcomeInfo(user);
}

void Server::cmd_JOIN(User* user, IRCMessage& msg)
{
	const std::vector<std::string>& params = msg.getParams();
	if (params.empty())
		return sendToUser(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "JOIN"));

	std::vector<std::string> chans = split(params[0], ',');
	std::vector<std::string> keys;
	if (params.size() > 1)
		keys = split(params[1], ',');

	unsigned int key_index = 0;
	for (size_t i = 0; i < chans.size(); ++i)
		userJoinChannel(user, chans[i], key_index, keys);
}
