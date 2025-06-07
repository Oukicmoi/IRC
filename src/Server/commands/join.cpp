/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:51:18 by octoross          #+#    #+#             */
/*   Updated: 2025/06/07 21:17:58 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

extern std::vector<std::string> split(const std::string& s, char delim);

void Server::cmd_JOIN(User* user, const IRCMessage& msg)
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

	for (size_t i = 0; i < chans.size(); ++i)
	{
		const std::string& name = chans[i];
		const std::string  key  = (i < keys.size() ? keys[i] : "");

		// <– ici, gérer éventuellement la clé 'key', l'invite-only, la limite…
		
		// 3) création/récupération du Channel
		Channel* ch = getOrCreateChannel(name, *user);

		ch->printMembers();
		ch->printOpe();
		// 4) tentative d'ajout


		// 5) diffusion du JOIN à tous
		{
			std::string prefix = user_id(user->getNick(), user->getUsername());
			std::string joinMsg = ":" + prefix + " JOIN :" + name + "\r\n";
			ch->broadcast(joinMsg);
		}

		// 6) envoi du topic ou pas de topic
		if (ch->getTopic().empty())
		{
			sendToUser(user->getSocketFd(),
				RPL_NOTOPIC(user->getNick(), name));
		}
		else
		{
			sendToUser(user->getSocketFd(),
				RPL_TOPIC(user->getNick(), name, ch->getTopic()));
		}

		// 7) envoi de la liste des noms (NAMES)
		{
			std::string list;
			const std::set<User*>& members = ch->getMembers();
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
