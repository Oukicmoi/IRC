/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:51:18 by octoross          #+#    #+#             */
/*   Updated: 2025/05/20 17:25:43 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::cmd_JOIN(User* user, const IRCMessage &msg)
{
    // params[0] == "#channel"
	(void)user;
	(void)msg;
    // std::string chname = params[0];
    // std::cout << "hereeee" << std::endl;
    // Channel*    ch = getOrCreateChannel(chname, *u);

    // // si déjà membre
    // if (! ch->addMember(u))
    // {
    //     // ERR_ALREADYJOINED
    //     return;
    // }
    // // on notifie tout le canal
    // std::string joinMsg = RPL_JOIN(user_id(u->getNick(), u->getUsername()), chname);
    // ch->broadcast(joinMsg);
}
