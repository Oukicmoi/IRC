/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:57:58 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/01 23:57:58 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

// src/Server/commands/quit.cpp

void	Server::clientQuits(int client_fd, std::string reason)
{
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1)
		ERR_SYS("epoll_ctl DEL");

	std::string quitMsg = ":" + user_id(_users[client_fd]->getNick(), _users[client_fd]->getUsername()) + " QUIT :" + reason + "\r\n";

    // Diffuser QUIT à tous les channels où est présent l'utilisateur
    for (std::map<std::string, Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); )
    {
        Channel* chan = it->second;

        // S'il est dans ce channel
        if (chan->getMembers().find(_users[client_fd]) != chan->getMembers().end())
        {
            chan->broadcast(quitMsg, _users[client_fd]);  // envoyer aux autres
            chan->removeMember(_users[client_fd]);        // le retirer

            // S'il ne reste plus personne dans le channel, on peut le supprimer
            if (chan->getMembers().empty())
            {
                delete chan;
                _channels.erase(it ++);
                continue;
            }
        }
        it ++;
    }

    // Optionnel : envoyer un message d'erreur localement (pas obligatoire)
    send(_users[client_fd]->getSocketFd(), ("ERROR :" + reason + "\r\n").c_str(), reason.length() + 9, 0);

    // Fermer la socket immédiatement
    close(_users[client_fd]->getSocketFd());

    // Retirer l'utilisateur du map _users
    delete _users[client_fd];
    _users.erase(client_fd);
}



void Server::cmd_QUIT(User* user, const IRCMessage& msg)
{
    std::string reason = "Quit: ";
    if (!msg.getParams().empty())
        reason += msg.getParams()[0];

   clientQuits(user->getSocketFd(), reason);
}
