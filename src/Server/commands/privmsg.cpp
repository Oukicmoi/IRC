/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:48:09 by octoross          #+#    #+#             */
/*   Updated: 2025/06/04 17:49:52 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_PRIVMSG(User* user, const IRCMessage& msg)
{
	const std::vector<std::string>& params = msg.getParams();

    msg.printParams();
	// Vérifie qu'il y a au moins 2 paramètres (cible + message)
	if (params.size() < 1)
	{
		send(user->getSocketFd(), ERR_NORECIPIENT(user->getNick()).c_str(), ERR_NORECIPIENT(user->getNick()).size(), 0);
		return;
	}
	if (params.size() < 2 || params[1].empty())
	{
		send(user->getSocketFd(), ERR_NOTEXTTOSEND(user->getNick()).c_str(), ERR_NOTEXTTOSEND(user->getNick()).size(), 0);
		return;
	}

	const std::string& target = params[0];
	const std::string& message = params[1][0] == ':' ? params[1].substr(1) : params[1];
	std::string fullMessage = RPL_PRIVMSG(user->getNick(), user->getUsername(), target, ":" + message);

	// Envoi vers un canal
	if (!target.empty() && (target[0] == '#' || target[0] == '&'))
	{
		std::map<std::string, Channel*>::iterator it = _channels.find(target);
		if (it == _channels.end())
		{
			send(user->getSocketFd(), ERR_NOSUCHNICK(user->getNick(), target).c_str(), ERR_NOSUCHNICK(user->getNick(), target).size(), 0);
			return;
		}
		Channel* channel = it->second;

		// Vérifie que l'utilisateur est dans le canal
		if (!channel->isMember(user))
		{
			send(user->getSocketFd(), ERR_CANNOTSENDTOCHAN(user->getNick(), target).c_str(), ERR_CANNOTSENDTOCHAN(user->getNick(), target).size(), 0);
			return;
		}

		// Vérifie si le canal est modéré et l'utilisateur autorisé à parler (exemple, à adapter)
		// if (channel->isModeSet('m') && !channel->canSpeak(user))
		// {
		// 	send(user->getSocketFd(), ERR_CANNOTSENDTOCHAN(user->getNick(), target).c_str(), ERR_CANNOTSENDTOCHAN(user->getNick(), target).size(), 0);
		// 	return;
		// }   ///TODO les modes
        channel->broadcast(fullMessage, user);

	}
	else // Envoi vers un utilisateur
	{
		User* dest = NULL;
		for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
		{
			if (it->second->getNick() == target)
			{
				dest = it->second;
				break;
			}
		}
		if (!dest)
		{
			send(user->getSocketFd(), ERR_NOSUCHNICK(user->getNick(), target).c_str(), ERR_NOSUCHNICK(user->getNick(), target).size(), 0);
			return;
		}

		send(dest->getSocketFd(), fullMessage.c_str(), fullMessage.size(), 0);
	}
}
