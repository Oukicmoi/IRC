/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:55:10 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/01 22:33:40 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

std::string User::getIPFromSocket(int socket_fd)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    memset(&clientAddr, 0, addrLen);

    if (getsockname(socket_fd, (struct sockaddr*)&clientAddr, &addrLen) == -1)
        return ("unknown");
    return (std::string(inet_ntoa(clientAddr.sin_addr)));
}

std::string maskIPOfIp(const std::string& ip)
{
    size_t lastDot = ip.find_last_of('.');
    if (lastDot == std::string::npos) return ("hidden.xxx");
    return ("hidden." + ip.substr(0, lastDot) + ".xxx");
}


User::User(int socket_fd) :
	_authentified(false), _passValid(false),
	_socket_fd(socket_fd),
	_Nickname(""), _Username(""), _password(""),
	_Host(getIPFromSocket(socket_fd)),
	_HostMask(maskIPOfIp(_Host))
{
	memset(_buffer, 0, sizeof(char) * (MAX_MSG_SIZE + 1));
}


User::~User()
{
    if (_socket_fd >= 0)
        close(_socket_fd);
}
