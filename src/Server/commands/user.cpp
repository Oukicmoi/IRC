/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:45:43 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/31 18:45:43 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::cmd_USER(User* user, IRCMessage& msg)
{
	// USER <username> <hostname> <servername> <realname>
	std::vector<std::string> &params = msg.getParams();
    if (params.size() < 4)
        return sendWhenReady(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "USER"));

    if (user->isAuthentified())
    {
        sendWhenReady(user->getSocketFd(), ERR_ALREADYREGISTERED(user->getNick()));
        return;
    }
    std::string &username = params[0];
    
    // Validate and normalize username
    processUsername(username, user->getNick());

    user->setUsername(username);
    
    // Complete registration if possible
    endAuthentification(user);
}
