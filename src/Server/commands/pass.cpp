/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:02:13 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/31 18:02:13 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::cmd_PASS(User* user, IRCMessage& msg)
{
    const std::vector<std::string>& params = msg.getParams();

    if (params.empty())
        return sendToUser(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "PASS"));

    if (user->isAuthentified())    
        return sendToUser(user->getSocketFd(), ERR_ALREADYREGISTERED(user->getNick()));


    const std::string &givenPass = params[0];
    if (givenPass != _mdp)
        return sendToUser(user->getSocketFd(), ERR_PASSWDMISMATCH(user->getNick()));

    user->setPasswordValid(true);
}
