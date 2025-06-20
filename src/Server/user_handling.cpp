/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_handling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:27:51 by octoross          #+#    #+#             */
/*   Updated: 2025/06/20 22:33:19 by octoross         ###   ########.fr       */
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
	const std::string &nick = user->getNick();
	sendWhenReady(user->getSocketFd(), RPL_WELCOME(nick, user->getFullNameMask()));
	sendWhenReady(user->getSocketFd(), RPL_YOURHOST(nick));
	
	sendWhenReady(user->getSocketFd(), RPL_CREATED(nick, formatTime(_creationTime)));
	sendWhenReady(user->getSocketFd(), RPL_MYINFO(nick, "", "ti", "kol"));	// Channel modes sans et avec paramètres
	sendWhenReady(user->getSocketFd(), RPL_ISUPPORT(nick, "NICKLEN=9 CHANTYPES=# PREFIX=(o)@"));
	sendWhenReady(user->getSocketFd(), ERR_NOMOTD(nick));
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

void	Server::sendToUser(const int fd)
{
	std::queue<std::string> &msgsToSend = _users[fd].getToSend();
	int n;
	while (!msgsToSend.empty())
	{
		std::string msg = msgsToSend.front();
		n = send(fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
		if (n <= 0)
		{
			msgsToSend.push(msg);
			std::cout << "║\t[Server] Has yet to send to client " << B << fd << R \
							<< "       >> " << BCYAN << msg << R << std::endl;
			return ;
		}
		else if (n < msg.size())
		{
			msgsToSend.pop();
			std::string yetToSend = msg.substr(n, msg.size() - n);
			msgsToSend.push(yetToSend);
		
			std::cout << "║\t[Server] Message sent to client " << B << fd << R \
							<< "       >> " << BCYAN << msg.substr(n) << R << std::endl;
			std::cout << "║\t[Server] Has yet to send to client " << B << fd << R \
							<< "       >> " << BCYAN << yetToSend << R << std::endl;
			return ;
		}
		std::cout << "║\t[Server] Message sent to client " << B << fd << R \
							<< "       >> " << BCYAN << msg << R << std::endl;
		msgsToSend.pop();
	}
}

void	Server::sendWhenReady(const int fd, std::string msg)
{
	_users[fd].addToSend(msg);
	sendToUser(fd);
}