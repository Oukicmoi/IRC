/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:51:18 by octoross          #+#    #+#             */
/*   Updated: 2025/06/13 01:27:32 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

extern std::vector<std::string> split(const std::string& s, char delim);

void Server::cmd_JOIN(User* user, IRCMessage& msg)
{
	const std::vector<std::string>& params = msg.getParams();
	// 1) validation
	if (params.empty())
	{
		sendToUser(user->getSocketFd(),
			ERR_NEEDMOREPARAMS(user->getNick(), "JOIN"));
		return;
	}

	// 2) liste de canaux et éventuellement de clés
	std::vector<std::string> chans = split(params[0], ',');
	std::vector<std::string> keys;
	if (params.size() > 1)
		keys = split(params[1], ',');

	unsigned int key_index = 0;
	for (size_t i = 0; i < chans.size(); ++i)
	{
		const std::string& name = chans[i];
		
		// 3) création/récupération du Channel
		Channel* channel = getChannelByName(name);
		if (!channel)
			createChannel(name, user);
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
		
		channel->printMembers();
		channel->printOperatorse();
		// 4) tentative d'ajout


		// 5) diffusion du JOIN à tous
		{
			std::string prefix = user_id(user->getNick(), user->getUsername());
			std::string joinMsg = ":" + prefix + " JOIN :" + name + "\r\n";
			channel->broadcast(joinMsg);
		}

		// 6) envoi du topic ou pas de topic
		if (channel->getTopic().empty())
		{
			sendToUser(user->getSocketFd(),
				RPL_NOTOPIC(user->getNick(), name));
		}
		else
		{
			sendToUser(user->getSocketFd(),
				RPL_TOPIC(user->getNick(), name, channel->getTopic()));
		}

		// 7) envoi de la liste des noms (NAMES)
		{
			std::string list;
			const std::set<User*>& members = channel->getMembers();
			for (std::set<User*>::const_iterator it = members.begin(); it != members.end(); ++it)
			{
				if (!list.empty())
					list += " ";
				list += (*it)->getNick();
			}
			sendToUser(user->getSocketFd(), RPL_NAMREPLY(user->getNick(), "=", name, list));
			sendToUser(user->getSocketFd(), RPL_ENDOFNAMES(user->getNick(), name));
		}
	}
}
