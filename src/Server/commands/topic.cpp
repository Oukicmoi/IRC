/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:49:00 by octoross          #+#    #+#             */
/*   Updated: 2025/05/20 16:39:40 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

typedef std::map<std::string,Channel*> ChannelMap;

void Server::cmd_TOPIC(User* u, const std::vector<std::string>& params)
{
    std::string chname = params[0];
    ChannelMap::iterator it = _channels.find(chname);

    if (it == _channels.end() || ! it->second->isMember(u))
    {
        // ERR_NOTONCHANNEL
        return;
    }

    Channel* ch = it->second;

    if (params.size() == 1)
    {
        // simple affichage
        std::string r = RPL_TOPIC(u->getNick(),chname,ch->getTopic());
        sendServerRpl(u->getSocketFd(), r);
    }
    else
    {
        // changement de topic (vérifier +t si nécessaire)
        ch->setTopic(params[1]);
        std::string ntf = RPL_TOPIC(u->getNick(),chname,params[1]);
        ch->broadcast(ntf);
    }
}

