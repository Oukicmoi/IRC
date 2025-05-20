/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:01:07 by octoross          #+#    #+#             */
/*   Updated: 2025/05/20 15:36:03 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// void	Server::welcome(User *user)
// {
// 	send(user->getFd(), , MSG_DONTWAIT | MSG_NOSIGNAL);
// }

void	Server::handleNewClients(void)
{
	socklen_t	addrlen = sizeof(_server_addr);
	bool	acceptBool = true;
	while (acceptBool)
	{
		int	client_fd = accept(_socket_fd, (struct sockaddr *)&_server_addr, &addrlen);
		if (client_fd >= 0)
		{
			if (!set_non_blocking_socket(client_fd))
				continue;
			if (!add_to_epoll(client_fd, EPOLLIN | EPOLLET | EPOLLOUT))
				continue;
			User* user = new User(client_fd);
			_users[client_fd] = user;
			std::cout << "\tnew " << B << "connection " << GREEN << "accepted" << R << " on " << B << "fd " << client_fd << R << std::endl;
		}
		else
			acceptBool = false;
	}
	if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
	{
		std::cout << "\t";
		ERR_SYS("accept");
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
            int n = recv(fd, buf, 4096, 0);
            if (n > 0 && n < 513)
                _users[ev.data.fd]->recvBuffer().append(buf, n);    // On stocke dans un buffer associé à ce client
            else if (n > 512)
                write(fd,"Hola ! Message trop long la team",33);
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
		while ((pos = buffer.find("\r\n")) != std::string::npos)
		{
			std::string line = buffer.substr(0, pos);
			std::cout << B << "line: " << line << R << std::endl;
			buffer.erase(0, pos + 2);
            std::cout << "hereeeeeee" << std::endl;
			// traitez 'line' comme une commande IRC complète
			handleLine(fd, line);
		}
    }
    if (ev.events & (EPOLLHUP|EPOLLERR))
	{
        close(fd);
        _users[ev.data.fd]->recvBuffer().erase();
    }
}
