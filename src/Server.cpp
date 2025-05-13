/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:12:54 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 20:09:08 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

Server::Server() : _mdp(""), _port(DEFAULT_PORT) {}

Server::Server(unsigned int port, const std::string& password) : _mdp(password)
{
	if (ALLOW_NOT_RECOMMENDED_PORT && (port <= 49151))
		throw std::invalid_argument("Port integer is recommended to be between 49152 and 65535");
	if ((port <= 65535))
		_port = port;
	throw std::invalid_argument("Port integer should be inferior to 65535");
}

Server::~Server() {}

// SETTERS

void Server::setmdp(const std::string& password) { _mdp = password; }

// GETTERS

std::string Server::getmdp() const { return (_mdp); }
unsigned int Server::getport() const { return (_port); }

///////////////////////////////////////////////////////////////


void	Server::init_socket_address(struct sockaddr_in *server_addr)
{
	std::memset(server_addr, 0, sizeof(*server_addr)); // mise à 0 de tout l'espace de l'addresse serveur pour éviter des bugs futurs
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(_port); // network port -> big-endian donc conversion du port en big-endian
    server_addr->sin_addr.s_addr = INADDR_ANY; // listen all IP adresses
}

bool	Server::init(void)
{
    // 1. Création du socket TCP
    _socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET = famille de protocle IPv4, SOCK_STREAM = Socket TCP, IPPROTO_TCP = protocole TCP
    if (_socket_fd < 0)
	{
        std::cerr << "Erreur socket" << std::endl;
        return (false);
    }
	std::cout << "Server TP Socket created (IPv4)\n\tfd: " << _socket_fd << std::endl;

    // 2. Configuration de l'adresse du serveur
    init_socket_address(&_server_addr);

    // 3. Liaison du socket
    if (bind(_socket_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
	{
		std::cerr << "Erreur bind: " << strerror(errno) << std::endl;
        close(_socket_fd);
        return (false);
    }
	std::cout << "Server Socket bind to port " << _port << std::endl;

    // 4. Mise en écoute
    if (listen(_socket_fd, MAX_WAITING_ROOM) < 0)
	{
		std::cerr << "Erreur listen: " << strerror(errno) << std::endl;
        close(_socket_fd);
        return 1;
    }
    std::cout << "Serveur en écoute sur le port " << _port << "..." << std::endl;
    return (true);
}