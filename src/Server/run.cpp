/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:37:02 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/07 21:30:21 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

extern volatile bool g_running;

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
				handleNewClients();
			else
				handleClient(events[event_index]);
			event_index ++;
		}
		if (events_count)
			std::cout << std::endl;
	}
}
