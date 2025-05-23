/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:40:03 by octoross          #+#    #+#             */
/*   Updated: 2025/05/24 00:08:39 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void	Server::cmd_CAP(User *user, const IRCMessage &msg)
{
	(void)user;
	(void)msg;
	// std::cout << "CAP received" << std::endl;
}