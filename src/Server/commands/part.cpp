/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:50:58 by octoross          #+#    #+#             */
/*   Updated: 2025/05/20 17:25:43 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_PART(User* user, const IRCMessage &msg)
{
	
	(void)user;
	(void)msg;
    // std::string chname = params[0];
    // ChannelMap::iterator it = _channels.find(chname);

    // if (it == _channels.end() || ! it->second->isMember(u))
    // {
    //     // ERR_NOTONCHANNEL
    //     return;
    // }

    // Channel* ch = it->second;
    // // on envoie le PART avant de retirer
    // std::string partMsg = RPL_PART(user_id(u->getNick(), u->getUsername()),chname,"");
    // ch->broadcast(partMsg);

    // ch->removeMember(u);

    // // si canal vide, on supprime
    // if (ch->getMembers().empty())
    // {
    //     delete ch;
    //     _channels.erase(it);
    // }
}