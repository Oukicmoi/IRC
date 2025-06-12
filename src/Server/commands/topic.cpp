/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:49:00 by octoross          #+#    #+#             */
/*   Updated: 2025/06/12 19:10:58 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_TOPIC(User* user, IRCMessage& msg)
{
	const std::vector<std::string>& p = msg.getParams();
	if (p.size() < 1)
	{
		sendToUser(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "TOPIC"));
		return;
	}

	const std::string& channelName = p[0];
	Channel* channel = getChannels()[channelName];
	if (!channel)
	{
		sendToUser(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), channelName));
		return;
	}

	if (!channel->isMember(user))
	{
		sendToUser(user->getSocketFd(), ERR_NOTONCHANNEL(user->getNick(), channelName));
		return;
	}

	if (p.size() == 1)
	{
		if (channel->getTopic().empty())
		{
            std::cout << "no topic" << std::endl;
			sendToUser(user->getSocketFd(), RPL_NOTOPIC(user->getNick(), channelName));
		}
		else
		{
            std::cout << "a topic" << std::endl;
			sendToUser(user->getSocketFd(), RPL_TOPIC(user->getNick(), channelName, channel->getTopic()));
		}
		return;
	}

	// Set or clear topic
	std::string newTopic = msg.getParams()[1];
	if (channel->isTopicProtected() && !channel->isOperator(user))
	{
		sendToUser(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), channelName));
		return;
	}

	channel->setTopic(newTopic);
	channel->setTopicSetter(user->getNick());
	channel->setTopicSetTime(std::time(NULL));
	channel->broadcast(":" + user->getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n");
}
