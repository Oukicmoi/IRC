/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   up.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:37:11 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/15 14:24:19 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

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
		ERR_SYS("socket");
        return (false);
    }
	std::cout << BCYAN << "Server Socket" << R << " (TCP/IPv4) created on " << B << "fd " << _socket_fd << R << std::endl;
	
	int optval = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		ERR_SYS("setsockopt");

	if (!set_non_blocking_socket(_socket_fd))
		return (false);

    // 2. Configuration de l'adresse du serveur
    init_socket_address();

	// 3. bind de la socket au port du serveur
    if (bind(_socket_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
	{
		ERR_SYS("bind");
		return (false);
    }
	std::cout << BCYAN << "Server Socket" << R << " bind to " << B << "port " << _port << R << std::endl;

	// 4. Mise en écoute de la socket
    if (listen(_socket_fd, MAX_WAITING_ROOM) < 0)
	{
		ERR_SYS("listen");
		return 1;
    }
    std::cout << std::endl << BCYAN << "Server Socket" << R << B << " en écoute" << R << " sur le " << B << "port " << _port << R << "..." << std::endl << std::endl << std::endl;
    return (true);
}

bool	Server::init_epoll(void)
{
	_epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1)
	{
		ERR_SYS("epoll_create1");
        return (false);
    }
	return (add_to_epoll(_socket_fd, EPOLLIN | EPOLLET)); // add EPOLLEXCLUSIVE pour multiprocess/threading -> plusieurs process/thread epoll_add une même socket, et leurs epoll_wait respectifs se fight pour gérer un event qui arrive, ce flag assure qu'il n'y en a qu'un qui la gérera évitant des problèmes liés au racing par exemple
}

bool	Server::up(void)
{
    if (!init_socket())
		return (false);
	return (init_epoll());
}	
