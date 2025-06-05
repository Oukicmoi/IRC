/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:19:38 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/31 18:19:38 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

// Validation du format du nickname selon RFC
bool Server::isValidNickname(const std::string& nick) const
{
	if (nick.empty() || nick.size() > NICKLEN)
		return false;
	if (std::string("#: ").find(nick[0]) != std::string::npos)
		return false;
	
	const std::string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789[]{}|\\_-`^";
	return (nick.find_first_not_of(allowed) == std::string::npos);
}

// Vérification de l'unicité du nickname
bool Server::isNicknameInUse(const std::string& nick) const
{
	for (std::map<int, User*>::const_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		std::cout << "OKAYYY TEST 12 12" << it->second->getNick() << std::endl;
		if (it->second->getNick() == nick)
			return true;
	}
	return false;
}

// Diffusion dans tous les canaux du user
void Server::broadcastToAllChannels(User* user, const std::string& message)
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isMember(user))
		{
			it->second->broadcast(message);
		}
	}
}

// Envoi des messages de bienvenue
void Server::sendWelcomeMessages(User* user)
{
	const std::string& nick = user->getNick();
	//const std::string datetime = "May 2024"; // À remplacer par la date réelle de création
	
	sendServerRpl(user->getSocketFd(), RPL_WELCOME(user_id(user->getNick(), user->getUsername()), nick));
	sendServerRpl(user->getSocketFd(), RPL_YOURHOST(nick, SERVER_NAME, SERVER_VERSION));
	// sendServerRpl(user->getSocketFd(), RPL_CREATED(nick, datetime));
	// 004: Capacités serveur
	sendServerRpl(user->getSocketFd(), RPL_MYINFO(nick, SERVER_NAME, SERVER_VERSION, "iwso", "mtklbv", "lv"));	// Channel modes avec paramètres
	sendServerRpl(user->getSocketFd(), RPL_ISUPPORT(nick, "NICKLEN=9 CHANTYPES=# PREFIX=(ov)@+ :are supported by this server"));
	// Erreur MOTD manquant (à remplacer par l'implémentation complète si besoin)
	sendServerRpl(user->getSocketFd(), ERR_NOMOTD(nick));
}


std::string Server::parseRealName(const std::vector<std::string>& params, size_t index)
{
    if (index >= params.size())
    	return "";
    
    std::string realname;
    for (size_t i = index; i < params.size(); ++i)
    {
        if (i > index) realname += " ";
        realname += params[i];
    }
    
    // Remove leading colon if present
    if (!realname.empty() && realname[0] == ':')
        realname = realname.substr(1);
    
    return realname;
}

void Server::processUsername(std::string& username, const std::string& fallbackNick)
{
	// Handle empty username case
	if (username.empty())
	{
		username = fallbackNick;
		return;
	}
	// Truncate to USERLEN (9 characters as per RPL_ISUPPORT)
	if (username.length() > USERLEN)
		username = username.substr(0, USERLEN);
	// Convert to lowercase (case-insensitive comparison)
	std::transform(username.begin(), username.end(), username.begin(), ::tolower);
}


void Server::EndRegister(User* user)
{
	if (user->isPasswordValidated() && !user->getNick().empty() && !user->getUsername().empty())
	{
		user->setRegistered(true);
		sendWelcomeMessages(user);
	}
}
