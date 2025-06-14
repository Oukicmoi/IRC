/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:48:09 by octoross          #+#    #+#             */
/*   Updated: 2025/06/14 20:13:17 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void	Server::sendMsgToChannel(User *user, const std::string &message, const std::string &channelName)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it == _channels.end())
		return sendToUser(user->getSocketFd(), ERR_NOSUCHNICK(user->getNick(), channelName));

	Channel* channel = it->second;

	if (!channel->isMember(user))
		return sendToUser(user->getSocketFd(), ERR_CANNOTSENDTOCHAN(user->getNick(), channelName));

	channel->broadcast(RPL_PRIVMSG(user->getFullNameMask(), channelName, message), user);
}

void	Server::sendMsgToUser(User *user, const std::string &message, const std::string target)
{
	User* destUser = getUserByNick(target);
	if (!destUser)
		return sendToUser(user->getSocketFd(), ERR_NOSUCHNICK(user->getNick(), target));

	sendToUser(destUser->getSocketFd(), RPL_PRIVMSG(user->getFullNameMask(), target, message));
}

void Server::cmd_PRIVMSG(User* user, IRCMessage& msg)
{
	const std::vector<std::string>& params = msg.getParams();

	// Vérifie qu'il y a au moins 2 paramètres (cible + message)
	if (params.size() < 1)
		return sendToUser(user->getSocketFd(), ERR_NORECIPIENT(user->getNick(), "PRIVMSG"));

	if ((params.size() < 2) || params[1].empty())
		return sendToUser(user->getSocketFd(), ERR_NOTEXTTOSEND(user->getNick()));
	
	const std::string& target = params[0];
	const std::string& message = params[1];

	// Envoi vers un canal
	if (!target.empty() && ((target[0] == '#') || (target[0] == '&')))
		sendMsgToChannel(user, message, target);
	else // Envoi vers un utilisateur
		sendMsgToUser(user, message, target);
}
