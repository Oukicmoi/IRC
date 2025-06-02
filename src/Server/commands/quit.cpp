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
void Server::cmd_QUIT(User* user, const IRCMessage& msg)
{
    std::string reason = "Quit: ";
    if (!msg.getParams().empty())
        reason += msg.getParams()[0];

    std::string quitMsg = ":" + user_id(user->getNick(), user->getUsername()) + " QUIT :" + reason + "\r\n";

    // Diffuser QUIT à tous les channels où est présent l'utilisateur
    for (std::map<std::string, Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); )
    {
        Channel* chan = it->second;

        // S'il est dans ce channel
        if (chan->getMembers().count(user))
        {
            chan->broadcast(quitMsg, user);  // envoyer aux autres
            chan->removeMember(user);        // le retirer

            // S'il ne reste plus personne dans le channel, on peut le supprimer
            if (chan->getMembers().empty())
            {
                delete chan;
                this->_channels.erase(it++);
                continue;
            }
        }
        ++it;
    }

    // Optionnel : envoyer un message d'erreur localement (pas obligatoire)
    send(user->getSocketFd(), ("ERROR :" + reason + "\r\n").c_str(), reason.length() + 9, 0);

    // Fermer la socket immédiatement
    close(user->getSocketFd());

    // Retirer l'utilisateur du map _users
    this->_users.erase(user->getSocketFd());
    delete user;
}
