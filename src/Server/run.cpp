#include "Server.hpp"

extern volatile bool g_running;

void	Server::addUsers(void)
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

std::string	string_epoll_event(const epoll_event& ev)
{
	if (ev.events & EPOLLIN)
		return ("EPOLLIN");
	if (ev.events & EPOLLOUT)
		return ("EPOLLOUT");
	if (ev.events & EPOLLERR)
		return ("EPOLLERR ");
	if (ev.events & EPOLLHUP)
		return ("EPOLLHUP ");
	if (ev.events & EPOLLET)
		return ("EPOLLET");
	if (ev.events & EPOLLONESHOT)
		return ("EPOLLONESHOT");
	return ("unknown");
}

void	Server::run()
{
	if (!up())
		return ;

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
			std::cout << "New event: " << B << "fd=" << events[event_index].data.fd << R << ", " << B << "type=" << string_epoll_event(events[event_index]) << R << std::endl;
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
