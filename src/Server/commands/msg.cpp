/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:50:15 by octoross          #+#    #+#             */
/*   Updated: 2025/05/20 17:25:43 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_MSG(User* user, const IRCMessage &msg)
{
	
	(void)user;
	(void)msg;
    // if (params.size() < 2)
    //     return; // Pas de message à envoyer

    // std::string chname = params[0];
    // ChannelMap::iterator it = _channels.find(chname);

    // if (it == _channels.end() || ! it->second->isMember(u))
    // {
    //     // ERR_NOTONCHANNEL
    //     return;
    // }

    // // Concatène le message
    // std::string msg;
    // for (size_t i = 1; i < params.size(); ++i)
    // {
    //     msg += params[i];
    //     if (i + 1 < params.size())
    //         msg += " ";
    // }

    // // Formate le message au style IRC
    // std::string ircMsg = ":" + u->getNick() + "!" + u->getUsername() + "@localhost PRIVMSG " + chname + " :" + msg;

    // // Envoie à tous sauf l'expéditeur
    // Channel* ch = it->second;
    // ch->broadcast(ircMsg, u);
}