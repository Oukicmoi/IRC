/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:46:13 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/01 23:46:13 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::cmd_MODE(User* user, IRCMessage& msg)
{
    std::vector<std::string>& params = msg.getParams();
    if (params.size() < 1)
        return sendToUser(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "USER"));

    if (params[0].empty() || (params[0][0] != '#'))
        return sendToUser(user->getSocketFd(), ERR_UMODEUNKNOWNFLAG(user->getNick()));

    Channel* channel = getChannelByName(params[0]);
    if (!channel)
        return sendToUser(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), params[0]));

    if (params.size() == 1)
        return channel->sendModesToUser(user);

    if (!channel->isOperator(user))
        return sendToUser(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), params[0]));
	// TODO: laisser admin serveur changer 

    channel->applyModes(user, msg);
}