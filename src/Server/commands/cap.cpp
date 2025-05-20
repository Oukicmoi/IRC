/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:40:03 by octoross          #+#    #+#             */
/*   Updated: 2025/05/20 17:49:57 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::cmd_CAP(User *user, const IRCMessage &msg)
{
	(void)user;
	(void)msg;
	// std::cout << "CAP received" << std::endl;
}