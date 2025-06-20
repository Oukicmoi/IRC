/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:33:39 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/04 18:33:39 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::kickOneUser(User* kicker, Channel* chan, const std::string& targetNick, const std::string& reason)
{
    User* target = getUserByNick(targetNick);
   
    if ((target == NULL) || !chan->isMember(target))
    	return sendWhenReady(kicker->getSocketFd(), ERR_USERNOTINCHANNEL(kicker->getNick(), targetNick, chan->getName()));
    
    if(reason.empty())
        chan->broadcast(RPL_KICK(kicker->getFullNameMask(), chan->getName(), targetNick, "No reason"), NULL);
    else
        chan->broadcast(RPL_KICK(kicker->getFullNameMask(), chan->getName(), targetNick, reason), NULL);
    chan->removeMember(target);
}

void Server::cmd_KICK(User* user, IRCMessage& msg)
{
    const std::vector<std::string>& params = msg.getParams();
    if (params.size() < 2)
        return sendWhenReady(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "KICK"));

    const std::string& channelName = params[0];
    std::string userList = params[1];
    std::string reason = "";
    if (params.size() >= 3)
        reason = params[2];

    // Vérifier que le channel existe
    std::map<std::string, Channel*>::iterator itChan = _channels.find(channelName);
    if (itChan == _channels.end())
        return sendWhenReady(user->getSocketFd(), ERR_NOSUCHCHANNEL(user->getNick(), channelName));
	
    Channel* chan = itChan->second;
    if (!chan->isOperator(user))
        return sendWhenReady(user->getSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNick(), channelName));

    // Découper la liste des cibles séparées par ','
    std::vector<std::string> targets = split(userList, ',');
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
        kickOneUser(user, chan, *it, reason);

    // Si le channel est devenu vide, on le supprime
    if (chan->isEmpty())
	{
		delete chan;
		_channels.erase(channelName);
	}
}
