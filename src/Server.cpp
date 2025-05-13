/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:12:54 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 16:26:24 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

Server::Server() : mdp("") {}

Server::Server(const std::string& password) : mdp(password) {}

Server::~Server() {}

void Server::setmdp(const std::string& password)
{
    mdp = password;
}

std::string Server::getmdp() const
{
    return mdp;
}

void Server::setport(const unsigned int& newport)
{
    port = newport;
}

unsigned int Server::getport() const
{
    return port;
}

