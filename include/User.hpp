/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:21:41 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/14 22:55:41 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "lib.hpp"

class User
{
    protected:
        unsigned int    _id;
        static unsigned int _nextId;
        int             _port;
        int         _socket_fd;
        std::string _Nickname;
        std::string _Username;
        std::string _Host;

    public:
        User(int socket_fd);
        User(int socket_fd, const std::string& nick);
        ~User();

        void            setNick(const std::string& nick);
        std::string     getNick() const;
        void            setId(unsigned int id);
        unsigned int    getId() const;
};

#endif
