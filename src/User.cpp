/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:55:10 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/15 22:20:10 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

unsigned int User::_nextId = 1;

User::User(int socket_fd) : _socket_fd(socket_fd), _Nickname(""), _id(_nextId++) {}

User::User(int socket_fd, const std::string& nick) : socket_fd(socket_fd), __Nickname(nick), _id(_nextId++) {}

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

int create_client_socket(const char* host, const char* port_str)
{
    // 1) Résolution de l'hôte + port (getaddrinfo) 
    struct addrinfo hints;
    struct addrinfo* res;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host, port_str, &hints, &res); //Traduit un nom d’hôte ("irc.example.com" ou "127.0.0.1") et un port ("6667") en liste d’adresses (IPv4/IPv6)
    if (err != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(err) << "\n";
        return -1;
    }

    int sockfd = -1;
    for (addrinfo* p = res; p; p = p->ai_next)
    {
        // 2) Création de la socket
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0) {
            std::cerr << "socket(): " << strerror(errno) << "\n";
            continue;
        }

        // 3) Tentative de connexion
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) {
            // succès !
            break;
        }

        // échec → fermer et essayer l’adresse suivante
        std::cerr << "connect(): " << strerror(errno) << "\n";
        close(sockfd);
        sockfd = -1;
    }

    freeaddrinfo(res);
    return sockfd;  // ≥0 sur succès, -1 sur échec
}
