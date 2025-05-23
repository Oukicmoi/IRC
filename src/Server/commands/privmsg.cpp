/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:48:09 by octoross          #+#    #+#             */
/*   Updated: 2025/05/24 00:08:55 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_PRIVMSG(User* user, const IRCMessage &msg)
{
	(void)user;
	(void)msg;
    // std::string target = params[0];
    // std::string msg    = params[1];

    // if (! target.empty() && target[0] == '#')
    // {
    //     ChannelMap::iterator it = _channels.find(target);
    //     if (it == _channels.end() || ! it->second->isMember(u))
    //     {
    //         // ERR_CANNOTSENDTOCHAN
    //         return;
    //     }
    //     Channel* ch = it->second;
    //     std::string fmt = RPL_PRIVMSG(u->getNick(), u->getUsername(), target,msg);
    //     // broadcast en excluant l’envoyeur
    //     ch->broadcast(fmt, u);
    // }
    // else
    // {
    //     // message privé à un autre User
    //     // ex. lookup dans _users par nick, puis sendServerRpl(...)
    // }
}