/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:12:54 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/16 16:45:36 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

Server::Server() : _mdp("admin"), _port(DEFAULT_PORT) {}

Server::Server(unsigned int port, const std::string& password) : _mdp(password)
{
	// _users;
	_socket_fd = -1;
	_epoll_fd = -1;
	std::memset(&_server_addr, 0, sizeof(_server_addr)); // mise à 0 de tout l'espace de l'addresse serveur pour éviter des bugs futurs
    
	if (!ALLOW_NOT_RECOMMENDED_PORT && (port <= 1023))
		throw std::invalid_argument("Port integer is recommended to be between 1024 and 65535");
	if ((port > 65535))
		throw std::invalid_argument("Port integer should be inferior to 65535");
	_port = port;
}

Server::~Server()
{
	_users.clear();
	if (_epoll_fd >= 0)
		close(_epoll_fd);
	if (_socket_fd)	
		close(_socket_fd);
}

// SETTERS

void Server::setmdp(const std::string& password) { _mdp = password; }

// GETTERS

std::string Server::getmdp() const { return (_mdp); }
unsigned int Server::getport() const { return (_port); }


// Retourne l'utilisateur enregistré sous le descripteur fd, ou nullptr si absent
User* Server::getUser(int fd) const
{
    std::map<int, User*>::const_iterator it = _users.find(fd);
    if (it == _users.end())
        return NULL;
    return it->second;
}

const std::map<int, User*>& Server::getUsers() const
{
    return _users;
}


///////////////////////////////////////////////////////////////


void	Server::init_socket_address(void)
{
	_server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port); // network port -> big-endian donc conversion du port en big-endian
    _server_addr.sin_addr.s_addr = INADDR_ANY; // listen all IP adresses
}

bool	Server::set_non_blocking_socket(int socket_fd)
{
	int	flags = fcntl(socket_fd, F_GETFL, 0);  // Récupère les flags actuels
	if (flags == -1)
	{
		ERR_SYS("ftnl (get flags)");
	    return (false);
	}
	if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		
		ERR_SYS("ftnl (set flags)");
        return (false);
	}
	std::cout << B << "\tfd=" << socket_fd << R << " put in " << B << "mode non-blocking" << R << std::endl;
	return (true);
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
	std::cout << B << CYAN << "Server Socket" << R << " (TCP/IPv4) created on " << B << "fd " << _socket_fd << R << std::endl;
	
	// if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) < 0){
	// 	std::cout << "Error:\tCreation socket failed." << std::endl;
	// 	return ; // TODO gérer ce caca
	// }

	if (!set_non_blocking_socket(_socket_fd))
		return (false);

    // 2. Configuration de l'adresse du serveur
    init_socket_address();

    if (bind(_socket_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
	{
		ERR_SYS("bind");
		return (false);
    }
	std::cout << B << CYAN << "Server Socket" << R << " bind to " << B << "port " << _port << R << std::endl;

    if (listen(_socket_fd, MAX_WAITING_ROOM) < 0)
	{
		ERR_SYS("listen");
		return 1;
    }
    std::cout << std::endl << "Serveur en écoute sur le port " << _port << "..." << std::endl << std::endl;
    return (true);
}

bool	Server::add_to_epoll(int socket_fd, uint32_t event_type)
{
	struct epoll_event	event;
    memset(&event, 0, sizeof(event));
    event.events = event_type;
    event.data.fd = socket_fd;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
	{
		ERR_SYS("epoll_ctl");
		return (false);
    }
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

bool	Server::init(void) // TODO mettre le init dans le constructor
{
    if (!init_socket())
		return (false);
	return (init_epoll());
}	

// void	Server::addUsers(void)	
// {
// 	socklen_t	addrlen = sizeof(_server_addr);
// 	bool	acceptBool = true;
// 	int	client_fd = 0; //temporary? idk
// 	while (acceptBool)
// 	{
// 		client_fd = accept(_socket_fd, (struct sockaddr *)&_server_addr, &addrlen);
// 		std::cout << "\tnew " << B << "connection " << GREEN << "accepted" << R << " on " << B << "fd " << client_fd << R << std::endl;
// 		if (client_fd >= 0)
// 		{
// 			User	*user = new User(client_fd);
// 			_users[client_fd] = user;
// 			// sr
// 		}
// 		else
// 		{
// 			acceptBool = false;
// 			if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
// 			{
// 				std::cout << "\t";
// 				ERR_SYS("accept");
// 			}	
// 		}
// 	}
// }

void Server::addUsers(void)
{
    socklen_t addrlen = sizeof(_server_addr);

    while (true)
    {
        int client_fd = accept(_socket_fd,
                               reinterpret_cast<struct sockaddr *>(&_server_addr),
                               &addrlen);

        if (client_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;

            // vraie erreur
            ERR_SYS("accept");
            break;
        }
        std::cout << "\tnew connection accepted on fd " << client_fd << std::endl;

        if (!set_non_blocking_socket(client_fd))
        {
            close(client_fd);
            continue;
        }
        if (!add_to_epoll(client_fd, EPOLLIN | EPOLLET))
        {
            close(client_fd);
            continue;
        }
        User* user = new User(client_fd);
        _users[client_fd] = user;
    }
}



void Server::handleClient(const epoll_event& ev)
{
    int fd = ev.data.fd;
    if (ev.events & EPOLLIN)
    {
        char buf[4096];
        while (true)
        {
            int n = recv(fd, buf, sizeof(buf), 0);
            if (n > 0)
                // On stocke dans un buffer associé à ce client
                _users[ev.data.fd]->recvBuffer().append(buf, n);
            else if (n == 0)
			{
                // le client a fermé la connexion
                close(fd);
                _users[ev.data.fd]->recvBuffer().erase();
                return;
            }
            else
			{
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
				else
				{
                    // vraie erreur
                    ERR_SYS("recv");
                    close(fd);
                    _users[ev.data.fd]->recvBuffer().erase();
                    return;
                }
            }
        }
		std::string& buffer = _users[ev.data.fd]->recvBuffer();
		size_t pos;
		while ((pos = buffer.find("\n")) != std::string::npos)
		{
			std::string line = buffer.substr(0, pos);
			buffer.erase(0, pos + 2);
			// traitez 'line' comme une commande IRC complète
			_users[ev.data.fd]->handleLine(fd, line);
		}
    }
    if (ev.events & (EPOLLHUP|EPOLLERR))
	{
        close(fd);
        _users[ev.data.fd]->recvBuffer().erase();
    }
}

extern volatile bool g_running;

void	Server::up()
{
	struct epoll_event events[MAX_WAITING_ROOM];
	while (g_running)
	{
		int events_count = epoll_wait(_epoll_fd, events, MAX_WAITING_ROOM, -1);
		if (events_count == -1)
		{
			ERR_SYS("epoll_wait");
			break ;
		}
		int event_index = 0;
		while (event_index < events_count)
		{
			// std::cerr << "bablbablabl\n\n" << std::endl;
			std::cout << "New event: " << B << "fd=" << events[event_index].data.fd << R << ", " << B << "type=" << events[event_index].events << R << std::endl;
			if (events[event_index].data.fd == _socket_fd) //requete sk server = nouvelle demande connexon (sinon client a deja sa propre socket et dans ce cas hanleClient)
				addUsers();
			else
				handleClient(events[event_index]);
			event_index ++;
		}
		if (events_count)
			std::cout << std::endl;
	}
}
