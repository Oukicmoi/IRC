/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:55:10 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 16:55:26 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

unsigned int Client::_nextId = 1;

Client::Client() : _nick(""), _id(_nextId++) {}

Client::Client(const std::string& nick) : _nick(nick), _id(_nextId++) {}

Client::~Client() {}

void Client::setNick(const std::string& nick)
{
    _nick = nick;
}

std::string Client::getNick() const
{
    return _nick;
}

void Client::setId(unsigned int id)
{
    _id = id;
}

unsigned int Client::getId() const
{
    return _id;
}
