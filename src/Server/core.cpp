/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:36:48 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/02 17:47:53 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void	Server::loadCmds(void)
{
	_cmds["CAP"] = &Server::cmd_CAP;
	_cmds["JOIN"] = &Server::cmd_JOIN;
	_cmds["PART"] = &Server::cmd_PART;
	_cmds["PRIVMSG"] = &Server::cmd_PRIVMSG;
	_cmds["TOPIC"] = &Server::cmd_TOPIC;
	_cmds["MSG"] = &Server::cmd_MSG;
	_cmds["PASS"] = &Server::cmd_PASS;
	_cmds["NICK"] = &Server::cmd_NICK;
	_cmds["USER"] = &Server::cmd_USER;
	_cmds["PING"] = &Server::cmd_PING;
	_cmds["QUIT"] = &Server::cmd_QUIT;

}

void	Server::init(void)
{
	_socket_fd = -1;
	_epoll_fd = -1;
	std::memset(&_server_addr, 0, sizeof(_server_addr)); // mise à 0 de tout l'espace de l'addresse serveur pour éviter des bugs futurs
    
	if (!ALLOW_NOT_RECOMMENDED_PORT && (_port <= 1023))
		throw std::invalid_argument("Port integer is recommended to be between 1024 and 65535");
	if ((_port > 65535))
		throw std::invalid_argument("Port integer should be inferior to 65535");
	loadCmds();
}

Server::Server(void) : _port(DEFAULT_PORT), _server_name(SERVER_NAME), _mdp("admin") { init(); }
Server::Server(unsigned int port) : _port(port), _server_name(SERVER_NAME), _mdp("admin") { init(); }
Server::Server(const std::string &password) : _port(DEFAULT_PORT), _server_name(SERVER_NAME), _mdp(password) { init(); }
Server::Server(unsigned int port, const std::string& password) : _port(port), _server_name(SERVER_NAME), _mdp(password) { init(); }

void	Server::shutdown(void)
{
	for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end(); it++)
		delete it->second;

	_users.clear();
	if (_epoll_fd >= 0)
	{
		close(_epoll_fd);
		_epoll_fd = -1;
	}
	if (_socket_fd >= 0)
	{
		close(_socket_fd);
		_socket_fd = -1;
	}
}

Server::~Server()
{
	shutdown();
}

// SETTERS

void Server::setmdp(const std::string& password) { _mdp = password; }

// GETTERS

std::string Server::getmdp() const { return (_mdp); }
unsigned int Server::getport() const { return (_port); }
const std::map<int, User*>& Server::getUsers() const { return (_users); }

User* Server::getUser(int fd) const
{
    std::map<int, User*>::const_iterator it = _users.find(fd);
    if (it == _users.end())
        return (NULL);
    return it->second;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


bool	Server::set_non_blocking_socket(int socket_fd)
{
	int	flags = fcntl(socket_fd, F_GETFL, 0);  // Récupère les flags actuels
	if (flags == -1)
	{
		close(socket_fd);
		ERR_SYS("ftnl (get flags)");
	    return (false);
	}
	if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		close(socket_fd);
		ERR_SYS("ftnl (set flags)");
        return (false);
	}
	std::cout << B << "\tfd=" << socket_fd << R << " put in " << B << "mode non-blocking" << R << std::endl;
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
		close(socket_fd);
		ERR_SYS("epoll_ctl");
		return (false);
    }
	return (true);
}
