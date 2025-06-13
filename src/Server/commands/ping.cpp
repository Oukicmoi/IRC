/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:30:42 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/01 23:30:42 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::cmd_PING(User* user, IRCMessage& msg)
{
	const std::vector<std::string>& params = msg.getParams();

	if (params.empty())
	{
		sendToUser(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "PING"));
		return;
	}

	std::string token = params[0];
	sendToUser(user->getSocketFd(), RPL_PONG(token));
}
