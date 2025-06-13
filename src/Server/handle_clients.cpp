/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:01:07 by octoross          #+#    #+#             */
/*   Updated: 2025/06/13 03:55:33 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
			if (!add_to_epoll(client_fd, EPOLLIN | EPOLLET | EPOLLOUT | EPOLLRDHUP))
				continue;
			User* user = new User(client_fd);
			_users[client_fd] = user;
			std::cout << "\tnew " << B << "connection " << BGREEN << "accepted" << R << " on " << B << "fd " << client_fd << R << std::endl;
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

bool	Server::handleMsg(int fd, const std::string& line)
{
	User* user = getUser(fd);
    if (!user)
		return (false);

	IRCMessage msg(line);
	std::map<std::string, void (Server::*)(User *, IRCMessage &)>::iterator it = _cmds.find(msg.getCmd());
	if (it != _cmds.end())
	{
		void (Server::*cmd)(User*, IRCMessage&) = it->second;
		if ((cmd != &Server::cmd_PASS) && !user->isPasswordValidated())
			return (sendToUser(fd, ERR_NOTREGISTERED), false);
		else if (!user->isAuthentified() && (cmd != &Server::cmd_PASS) && (cmd != &Server::cmd_NICK) && (cmd != &Server::cmd_USER))
			return (sendToUser(fd, ERR_NOTREGISTERED), false);
			
		(this->*cmd)(user, msg);
		return (cmd == &Server::cmd_QUIT);
		
	}
	else
        return (sendToUser(fd, ERR_UNKNOWNCOMMAND(user->getNick(), msg.getCmd())), false);
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
                write(fd,"Hola ! Message trop long la team",33); // TODO change ca
            else if (n == 0)
				return clientQuits(fd, "connection lost");
            else
			{
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
				else
				{
					if (errno == EBADF)
						return ;
					clientQuits(fd, "connection error");
					return ;
                }
            }
        }
		std::string& buffer = _users[ev.data.fd]->recvBuffer();
		std::cout << "⤷ Recv: '" << BYELLOW << buffer << R << "'" << std::endl;
		size_t pos;
		std::string endline = "\r\n";
		if (TEST_WITH_NC)
			endline = "\n";
		bool hasQuit = false;
		while (!hasQuit && ((pos = buffer.find(endline)) != std::string::npos))
		{
			std::string line = buffer.substr(0, pos);
			std::cout << "╔════ Msg: " << BYELLOW << line << R << std::endl;
			buffer.erase(0, pos + 2);
			// traitez 'line' comme une commande IRC complète
			hasQuit = handleMsg(fd, line);
			std::cout << "╚══════════" << std::endl << std::endl;
		}
    }
    else if (ev.events & (EPOLLHUP|EPOLLRDHUP))
		clientQuits(fd, "connection hang up");
	else if (ev.events & EPOLLERR)
		ERR_SYS("epoll event: EPOLLERR");
}
