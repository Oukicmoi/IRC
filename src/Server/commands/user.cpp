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
    const std::vector<std::string>& params = msg.getParams();
    if (params.size() < 4)
    {
        sendToUser(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "USER"));
        return;
    }
    if (user->isAuthentified())
    {
        sendToUser(user->getSocketFd(), ERR_ALREADYREGISTERED(user->getNick()));
        return;
    }
    // Process user parameters
    std::string username = params[0];
    
    // Validate and normalize username
    processUsername(username, user->getNick());

    // Update user attributes
    user->setUsername(username);
    // user->setNick(realname);
    
    // Complete registration if possible
    endRegister(user);
}
