/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:55:10 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/01 22:33:40 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

unsigned int User::_nextId = 1;


std::string User::getIPFromSocket(int socket_fd)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    memset(&clientAddr, 0, addrLen);

    if (getsockname(socket_fd, (struct sockaddr*)&clientAddr, &addrLen) == -1)
        return ("unknown");
    return (std::string(inet_ntoa(clientAddr.sin_addr)));
}

User::User(int socket_fd) : _authentified(false), _passValid(false), _socket_fd(socket_fd), _id(_nextId++), _Nickname(""), _Username(""), _Host(getIPFromSocket(socket_fd)), _password("")
{
	std::cout << "client has ip address: " << _Host << std::endl;
    memset(_buffer, 0, sizeof(char) * (MAX_MSG_SIZE + 1));
}

// User::User(int socket_fd, const std::string& nick) :  _id(_nextId++), _socket_fd(socket_fd), _Nickname(nick) {}

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

std::string User::getUsername() const
{
    return _Username;
}

void User::setId(unsigned int id)
{
    _id = id;
}

unsigned int User::getId() const
{
    return _id;
}

int User::getSocketFd() const 
{
    return _socket_fd;
}

std::string&  User::recvBuffer()
{
    return clientBuffers;
}
const std::string& User::recvBuffer() const
{
    return clientBuffers;
}


void User::setUsername(const std::string& username)
{
    _Username = username;
}

void User::setHost(const std::string& host)
{
    _Host = host;
}

std::string User::getHost() const
{
    return _Host;
}

std::string& User::getClientBuffers()
{
    return clientBuffers;
}

const std::string& User::getClientBuffers() const
{
    return clientBuffers;
}

void User::setClientBuffers(const std::string& buf)
{
    clientBuffers = buf;
}

char* User::getBuffer()
{
    return _buffer;
}

const char* User::getBuffer() const
{
    return _buffer;
}

unsigned int User::getNextId()
{
    return _nextId;
}

std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::string item;
    std::istringstream iss(s);
    while (std::getline(iss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

// Concatène tous les éléments de `v` à partir de l’indice `start`,
// en les séparant par un espace.
std::string join_rest(const std::vector<std::string>& v, size_t start)
{
    if (start >= v.size())
        return std::string();

    std::string result = v[start];
    for (size_t i = start + 1; i < v.size(); ++i)
    {
        result += " ";
        result += v[i];
    }
    return result;
}

std::string User::getPrefix() const
{
	return _Nickname + "!" + _Username + "@" + _Host;
}
