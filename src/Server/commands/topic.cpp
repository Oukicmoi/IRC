/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:49:00 by octoross          #+#    #+#             */
/*   Updated: 2025/06/03 22:36:04 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_TOPIC(User* user, const IRCMessage& msg)
{
	const std::vector<std::string>& p = msg.getParams();
	if (p.size() < 1)
	{
		sendServerRpl(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "TOPIC"));
		return;
	}

	const std::string& channelName = p[0];
	Channel* channel = getChannels()[channelName];
	if (!channel)
	{
		sendServerRpl(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), channelName));
		return;
	}

	if (!channel->isMember(user))
	{
		sendServerRpl(user->getSocketFd(), ERR_NOTONCHANNEL(user->getNick(), channelName));
		return;
	}

	// View topic
	if (p.size() == 1)
	{
		if (channel->getTopic().empty())
		{
			sendServerRpl(user->getSocketFd(), RPL_NOTOPIC(user->getNick(), channelName));
		}
		else
		{
			sendServerRpl(user->getSocketFd(), RPL_TOPIC(user->getNick(), channelName, channel->getTopic()));
		}
		return;
	}

	// Set or clear topic
	std::string newTopic = msg.getParams()[1];
	if (channel->isTopicProtected() && !channel->isOperator(user))
	{
		sendServerRpl(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), channelName));
		return;
	}

	channel->setTopic(newTopic);
	channel->setTopicSetter(user->getNick());
	channel->setTopicSetTime(std::time(NULL));

	channel->broadcast(":" + user->getPrefix() + " TOPIC " + channelName + " :" + newTopic);
}


