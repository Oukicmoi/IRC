/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:09:24 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/31 18:09:24 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::cmd_NICK(User* user, IRCMessage& msg)
{
    const std::vector<std::string>& params = msg.getParams();
    
    // 1. Vérifier la présence du nickname
    if (params.empty())
    {
        sendToUser(user->getSocketFd(), ERR_NONICKNAMEGIVEN(user->getNick()));
        return;
    }

    std::string newNick = params[0];

    // 2. Valider le format du nickname
    if (!isValidNickname(newNick))
    {
        sendToUser(user->getSocketFd(), ERR_ERRONEUSNICKNAME(user->getNick(), newNick));
        return;
    }

    // 3. Vérifier la disponibilité du nickname
    if (isNicknameInUse(newNick))
    {
        sendToUser(user->getSocketFd(), ERR_NICKNAMEINUSE(user->getNick(), newNick));
        return;
    }

    // 4. Mise à jour du nickname
    std::string oldNick = user->getNick();
    user->setNick(newNick);

    // 5. Notifier le changement si déjà enregistré
    if (user->isAuthentified())
    {
        std::string nickChangeMsg = ":" + oldNick + "!" + user->getUsername() + "@localhost NICK :" + newNick + "\r\n";
        broadcastToAllChannels(user, nickChangeMsg);
    }

    endRegister(user);
}
