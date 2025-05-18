/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:12:54 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/18 23:49:11 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

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
                std::cout << "Hola ! Message trop long la team" << std::endl;
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
			handleLine(fd, line);
		}
    }
    if (ev.events & (EPOLLHUP|EPOLLERR))
	{
        close(fd);
        _users[ev.data.fd]->recvBuffer().erase();
    }
}
