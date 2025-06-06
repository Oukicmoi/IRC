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

void Server::cmd_INVITE(User* user, const IRCMessage& msg)
{
    const std::vector<std::string>& p = msg.getParams();
    if (p.size() < 2)
        return(sendServerRpl(user->getSocketFd(),ERR_NEEDMOREPARAMS(user->getNick(), "INVITE")), (void)p);

    std::string targetNick  = p[0];
    std::string channelName = p[1];

    Channel* channel = getChannelByName(channelName);
    if (!channel)
        return(sendServerRpl(user->getSocketFd(),ERR_NOSUCHCHANNEL(user->getNick(), channelName)), (void)p);

    // Only members may invite
    if (!channel->isMember(user))
        return(sendServerRpl(user->getSocketFd(),ERR_NOTONCHANNEL(user->getNick(), channelName)), (void)p);

    // If +i is set, only ops may invite
    if (channel->isInviteOnly() && !channel->isOperator(user))
        return(sendServerRpl(user->getSocketFd(),ERR_CHANOPRIVSNEEDED(user->getNick(), channelName)), (void)p);

    User* target = getUserByNick(targetNick);
    if (!target)
        return(sendServerRpl(user->getSocketFd(),ERR_NOSUCHNICK(user->getNick(), targetNick)), (void)p);

    // If target is already on channel, error
    if (channel->isMember(target))
        return(sendServerRpl(user->getSocketFd(),ERR_USERONCHANNEL(user->getNick(), targetNick, channelName)), (void)p);

    // Notify sender that invite succeeded
    sendServerRpl(user->getSocketFd(), RPL_INVITING(user->getNick(), targetNick, channelName));

    // Send INVITE to target
    std::string prefix = user_id(user->getNick(), user->getUsername());
    sendServerRpl(target->getSocketFd(), RPL_INVITE(prefix, targetNick, channelName));
}
