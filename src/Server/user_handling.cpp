/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_handling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:27:51 by octoross          #+#    #+#             */
/*   Updated: 2025/06/14 20:27:03 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

bool	Server::isValidChannelName(const std::string &channelName)
{	
	std::string notAllowed = "\a ,";
	unsigned int i = 0;
	while (i < channelName.size())
	{
		if (notAllowed.find(channelName[i ++]) != std::string::npos)
			return (false);
	}
	return (true);
}

bool	Server::isValidChannelPrefix(const std::string &channelName)
{	
	return (!channelName.empty() && (channelName[0] == '#'));
}



std::string	Server::isValidKey(const std::string &key)
{
	std::string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{};:'\",.<>/?";
	std::string notAllowed = " ";
	
	if ((key.size() < MIN_KEYLEN) || (key.size() > MAX_KEYLEN))
		return ("Key must be between " + stringOfType<int>(MIN_KEYLEN) + "-" + stringOfType<int>(MAX_KEYLEN) + " characters");
	unsigned int i = 0;
	while (i < key.size())
	{
		if ((notAllowed.find(key[i]) != std::string::npos)
			|| (std::iscntrl(static_cast<unsigned char>(key[i]))))
			return ("Key contains invalid characters");
		i ++;
	}
	return ("");
}


// Validation du format du nickname selon RFC
bool Server::isValidNickname(const std::string& nick)
{
	if (nick.empty() || (nick.size() > NICKLEN))
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
		if (it->second->getNick() == nick)
			return (true);
	}
	return (false);
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


void Server::endAuthentification(User* user)
{
	if (user->isPasswordValid() && !user->getNick().empty() && !user->getUsername().empty())
	{
		user->setAuthentified(true);
		sendWelcomeMessages(user);
	}
}

// Envoi des messages de bienvenue
void Server::sendWelcomeMessages(User* user)
{
	//const std::string datetime = "May 2024"; // À remplacer par la date réelle de création
	
	const std::string &nick = user->getNick();
	sendToUser(user->getSocketFd(), RPL_WELCOME(nick, user->getFullNameMask()));
	sendToUser(user->getSocketFd(), RPL_YOURHOST(nick));
	// sendToUser(user->getSocketFd(), RPL_CREATED(nick, datetime)); // TODO
	sendToUser(user->getSocketFd(), RPL_MYINFO(nick, "", "ti", "kol"));	// Channel modes avec paramètres
	sendToUser(user->getSocketFd(), RPL_ISUPPORT(nick, "NICKLEN=9 CHANTYPES=# PREFIX=(ov)@+ :are supported by this server")); // TODO reregarder ce moreceau ce que ca fait
	// Erreur MOTD manquant (à remplacer par l'implémentation complète si besoin) -> TODO
	sendToUser(user->getSocketFd(), ERR_NOMOTD(nick));
}

// Diffusion dans tous les canaux du user
void Server::broadcastToAllChannels(User* user, const std::string& message)
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isMember(user))
			it->second->broadcast(message);

	}
}

void	Server::sendToUser(int const client_fd, std::string client_buffer)
{
	std::istringstream	buf(client_buffer);
	std::string			reply;
	
	send(client_fd, client_buffer.c_str(), client_buffer.size(), 0);
	while (getline(buf, reply))
	{
		std::cout << "║\t[Server] Message sent to client " \
				  << B << client_fd << R << "       >> " << BCYAN << reply << R << std::endl;
	}
}
