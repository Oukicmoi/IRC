/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:49:00 by octoross          #+#    #+#             */
/*   Updated: 2025/06/20 21:56:04 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_TOPIC(User* user, IRCMessage& msg)
{
	const std::vector<std::string>& params = msg.getParams();
	if (params.size() < 1)
		return sendWhenReady(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "TOPIC"));

	const std::string& channelName = params[0];
	Channel* channel = getChannelByName(channelName);
	if (!channel)
		return sendWhenReady(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), channelName));

	if (!channel->isMember(user))
		return sendWhenReady(user->getSocketFd(), ERR_NOTONCHANNEL(user->getNick(), channelName));

	if (params.size() == 1)
	{
		// TOPIC -> send channel topic to user
		if (channel->getTopic().empty())
			sendWhenReady(user->getSocketFd(), RPL_NOTOPIC(user->getNick(), channelName));
		else
			sendWhenReady(user->getSocketFd(), RPL_TOPIC(user->getNick(), channelName, channel->getTopic()));
		return;
	}

	// Set channel topic
	std::string newTopic = params[1];
	if (channel->isTopicRestricted() && !channel->isOperator(user))
		return sendWhenReady(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), channelName));

	channel->setTopic(newTopic);
	channel->setTopicSetter(user->getNick());
	channel->setTopicSetTime(std::time(NULL));
	channel->broadcast(RPL_TOPIC_SET(user->getFullNameMask(), channel->getName(), newTopic));
}
