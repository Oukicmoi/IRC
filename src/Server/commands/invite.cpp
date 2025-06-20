/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 21:52:53 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/06 21:52:53 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::cmd_INVITE(User* user, IRCMessage& msg)
{
    const std::vector<std::string>& params = msg.getParams();
    if (params.size() < 2)
        return sendWhenReady(user->getSocketFd(),ERR_NEEDMOREPARAMS(user->getNick(), "INVITE"));

    std::string targetNick  = params[0];
    std::string channelName = params[1];

    Channel* channel = getChannelByName(channelName);
    if (!channel)
        return sendWhenReady(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), channelName));

    if (!channel->isMember(user))
        return sendWhenReady(user->getSocketFd(), ERR_NOTONCHANNEL(user->getNick(), channelName));

    // If +i is set, only ops may invite
    if (channel->isInviteOnly() && !channel->isOperator(user))
        return sendWhenReady(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), channelName));

    User* target = getUserByNick(targetNick);
    if (!target)
        return sendWhenReady(user->getSocketFd(), ERR_NOSUCHNICK(user->getNick(), targetNick));

    if (channel->isMember(target))
        return sendWhenReady(user->getSocketFd(), ERR_USERONCHANNEL(user->getNick(), targetNick, channelName));

    // Notify sender that invite succeeded
    sendWhenReady(user->getSocketFd(), RPL_INVITING(user->getNick(), targetNick, channelName));

	channel->addToInviteList(target);
    // Send INVITE to target
    sendWhenReady(target->getSocketFd(), RPL_INVITE(user->getFullNameMask(), targetNick, channelName));
}
