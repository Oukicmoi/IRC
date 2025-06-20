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
    
    if (params.empty())
        return sendWhenReady(user->getSocketFd(), ERR_NONICKNAMEGIVEN(user->getNick()));

    std::string newNick = params[0];

    if (!isValidNickname(newNick))
        return sendWhenReady(user->getSocketFd(), ERR_ERRONEUSNICKNAME(user->getNick(), newNick));

    if (isNicknameInUse(newNick))
        return sendWhenReady(user->getSocketFd(), ERR_NICKNAMEINUSE(user->getNick(), newNick));

    std::string oldNick = user->getNick();
    user->setNick(newNick);

    if (user->isAuthentified())
        broadcastToAllChannels(user, RPL_NICK(user->getOldFullNameMask(oldNick), newNick));

	// end authentification if possible
    endAuthentification(user);
}
