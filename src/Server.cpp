/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:12:54 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 21:19:54 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

Server::Server() : _mdp(""), _port(DEFAULT_PORT) {}

Server::Server(unsigned int port, const std::string& password) : _mdp(password)
{
	_socket_fd = -1;
	_epoll_fd = -1;
	std::memset(&_server_addr, 0, sizeof(_server_addr)); // mise à 0 de tout l'espace de l'addresse serveur pour éviter des bugs futurs
    
	if (ALLOW_NOT_RECOMMENDED_PORT && (port <= 1023))
		throw std::invalid_argument("Port integer is recommended to be between 1024 and 65535");
	if ((port > 65535))
		throw std::invalid_argument("Port integer should be inferior to 65535");
	_port = port;
}

Server::~Server() {}

// SETTERS

void Server::setmdp(const std::string& password) { _mdp = password; }

// GETTERS

std::string Server::getmdp() const { return (_mdp); }
unsigned int Server::getport() const { return (_port); }

///////////////////////////////////////////////////////////////


void	Server::init_socket_address(void)
{
	_server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port); // network port -> big-endian donc conversion du port en big-endian
    _server_addr.sin_addr.s_addr = INADDR_ANY; // listen all IP adresses
}

bool	Server::init_socket(void)
{
	// 1. Création du socket TCP
    _socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET = famille de protocle IPv4, SOCK_STREAM = Socket TCP, IPPROTO_TCP = protocole TCP
    if (_socket_fd < 0)
	{
        std::cerr << "Erreur socket: " << strerror(errno) << std::endl;
        return (false);
    }
	std::cout << "Server TP Socket created (IPv4)\n\tfd: " << _socket_fd << std::endl;
	
	int	flags = fcntl(_socket_fd, F_GETFL, 0);  // Récupère les flags actuels
	if (flags == -1)
	{
		std::cerr << "Erreur ftnl (get flags): " << strerror(errno) << std::endl;
        return (false);
	}
	if (fcntl(_socket_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
        std::cerr << "Erreur ftnl: " << strerror(errno) << std::endl;
        return (false);
	}
	std::cout << "Server Socket put in mode non-blocking" << std::endl;

    // 2. Configuration de l'adresse du serveur
    init_socket_address();

    if (bind(_socket_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
	{
		std::cerr << "Erreur bind: " << strerror(errno) << std::endl;
        close(_socket_fd);
        return (false);
    }
	std::cout << "Server Socket bind to port " << _port << std::endl;

    if (listen(_socket_fd, MAX_WAITING_ROOM) < 0)
	{
		std::cerr << "Erreur listen: " << strerror(errno) << std::endl;
        close(_socket_fd);
        return 1;
    }
    std::cout << "Serveur en écoute sur le port " << _port << "..." << std::endl;
    return (true);
}

bool	Server::init(void)
{
    if (!init_socket())
		return (false);

	_epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if (_epoll_fd == -1)
	{
		close(_socket_fd);
		std::cerr << "Erreur epoll_create: " << strerror(errno) << std::endl;
        return (false);
    }

	struct epoll_event	event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN;  // Surveille les connexions entrantes
    event.data.fd = _socket_fd;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _socket_fd, &event) == -1)
	{
		
		std::cerr << "Erreur epoll_ctl: " << strerror(errno) << std::endl;
        close(_epoll_fd);
		close(_socket_fd);
        return (false);
    }

	return (true);
}
