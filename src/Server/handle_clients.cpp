/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:01:07 by octoross          #+#    #+#             */
/*   Updated: 2025/06/20 22:37:29 by octoross         ###   ########.fr       */
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
			if (!user)
			{
				ERR_SYS("new");
				std::cout << "\tnew " << B << "connection " << BRED << "refused" << R << " on " << B << "fd " << client_fd << R << std::endl;
				close(client_fd);
			}
			else
			{
				_users[client_fd] = user;
				std::cout << "\tnew " << B << "connection " << BGREEN << "accepted" << R << " on " << B << "fd " << client_fd << R << " on " << B << "IP " << user->getHost() << std::endl;
			}
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
		if ((cmd != &Server::cmd_PASS) && !user->isPasswordValid())
			return (sendWhenReady(fd, ERR_NOTREGISTERED(msg.getCmd())), false);
		else if (!user->isAuthentified() && (cmd != &Server::cmd_PASS) && (cmd != &Server::cmd_NICK) && (cmd != &Server::cmd_USER))
			return (sendWhenReady(fd, ERR_NOTREGISTERED(msg.getCmd())), false);
			
		(this->*cmd)(user, msg);
		return (cmd == &Server::cmd_QUIT);
		
	}
	else
        return (sendWhenReady(fd, ERR_UNKNOWNCOMMAND(user->getNick(), msg.getCmd())), false);
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
            if ((n > 0) && (n < 513))
                _users[fd]->_recvBuffer.append(buf, n);
            else if (n > 512)
                return sendWhenReady(fd, ERR_INPUTTOOLONG(_users[fd]->getNick()));
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
		std::string& buffer = _users[fd]->_recvBuffer;
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
	else if (ev.events & EPOLLOUT)
		sendToUser(fd);
    else if (ev.events & (EPOLLHUP|EPOLLRDHUP))
		clientQuits(fd, "connection hang up");
	else if (ev.events & EPOLLERR)
		ERR_SYS("epoll event: EPOLLERR");
}
