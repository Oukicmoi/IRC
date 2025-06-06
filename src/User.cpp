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

User::User(int socket_fd) : _id(_nextId++), _socket_fd(socket_fd), _Nickname(""), _Username(""), _Host(""), _password(""), _registered(false), _passValid(false)
{
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


void User::setPort(int port)
{
    _port = port;
}

int User::getPort() const
{
    return _port;
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

// void Server::handleLine(int fd, const std::string& line)
// {
//     // On récupère l'utilisateur
//     User* u = getUser(fd);
//     if (!u)
//         return; // pas trouvé, on ignore

//     // On découpe la ligne en tokens
//     std::vector<std::string> tokens = split(line, ' ');
//     if (tokens.empty())
//         return;

//     // Le verbe IRC est le premier token
//     std::string cmd = tokens[0];

//     // JOIN et PART prennent tous leurs paramètres après le verbe
//     std::vector<std::string> params(tokens.begin() + 1, tokens.end());
//     if (cmd == "JOIN")
//     {
//         if(params.empty())
//             return(write(fd,"You need a name for your channel",32), (void)cmd);
//         cmd_JOIN(u, params);
//     }
//     else if (cmd == "PART")
//     {
//         cmd_PART(u, params);
//     }
//     // PRIVMSG prend 2 paramètres explicitement : cible et message
//     else if (cmd == "PRIVMSG")
//     {
//         // On vérifie qu'on a au moins "PRIVMSG cible texte..."
//         if (tokens.size() >= 3)
//         {
//             std::string target  = tokens[1];
//             std::string message = join_rest(tokens, 2);
//             std::vector<std::string> params;
//             params.push_back(target);
//             params.push_back(message);
//             cmd_PRIVMSG(u, params);
//         }
//     }
//     else if (cmd == "TOPIC")
//     {
//         if(params.empty())
//             return(write(fd,"You need a name for your topic",30), (void)cmd);
//         std::vector<std::string> params(tokens.begin() + 1, tokens.end());
//         cmd_TOPIC(u, params);
//     }
//     else
//     {
//         // On reconstruit correctement params pour cmd_MSG
//         std::vector<std::string> fixedParams;
//         fixedParams.push_back(cmd); // l'ancien "cmd" est ici en réalité le nom du canal
//         fixedParams.insert(fixedParams.end(), params.begin(), params.end());
//         cmd_MSG(u, fixedParams);
//     }

// }

// ssize_t User::send(const std::string& msg, int flags)
// {
// 	ssize_t ret = send(_socket_fd, msg.c_str(), msg.size(), flags);
// 	if (ret == -1)
// 		ERR_SYS("send");
// 	return ret;
// }