/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:55:10 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/16 16:07:42 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

unsigned int User::_nextId = 1;

User::User(int socket_fd) : _id(_nextId++), _socket_fd(socket_fd), _Nickname("")
{
    memset(_buffer, 0, sizeof(char) * (MAX_MSG_SIZE + 1));
}
User::User() {}

User::User(int socket_fd, const std::string& nick) :  _id(_nextId++), _socket_fd(socket_fd), _Nickname(nick) {}

User::~User()
{
    if (_socket_fd >= 0)
        close(_socket_fd);
}

void User::setNick(const std::string& nick)
{
    _Nickname = nick;
}

std::string User::getNick() const
{
    return _Nickname;
}

void User::setId(unsigned int id)
{
    _id = id;
}

unsigned int User::getId() const
{
    return _id;
}

std::string&  User::recvBuffer()
{
    return clientBuffers;
}
const std::string& User::recvBuffer() const
{
    return clientBuffers;
}

void User::handleLine(int fd, std::string msg)
{
    (void)fd;
	if(msg == "KICK")
		return ; //KICK func
	if(msg == "INVITE")
		return ; //INVITE func
	if(msg == "TOPIC")
		return ; //TOPIC func
	else
		std::cerr << msg << std::endl;
}
