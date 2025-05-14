/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:55:10 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/14 23:58:10 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

unsigned int Client::_nextId = 1;

Client::Client() : _Nickname(""), _id(_nextId++) {}

Client::Client(const std::string& nick) : _Nickname(nick), _id(_nextId++) {}

Client::~Client() {}

void Client::setNick(const std::string& nick)
{
    _Nickname = nick;
}

std::string Client::getNick() const
{
    return _Nickname;
}

void Client::setId(unsigned int id)
{
    _id = id;
}

unsigned int Client::getId() const
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
