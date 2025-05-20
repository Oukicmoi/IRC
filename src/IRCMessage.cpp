/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:33:52 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/20 16:09:58 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCMessage.hpp"

IRCMessage::IRCMessage(const std::string &rawMsg)
{
	
}

IRCMessage::~IRCMessage(void);

std::string IRCMessage::getCommand() const;