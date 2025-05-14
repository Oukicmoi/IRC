/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:21:41 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/14 22:55:41 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "lib.hpp"

class Client
{
    protected:
        std::string _Nickname;
        std::string _Username;
        std::string _Host;
        unsigned int    _id;
        static unsigned int _nextId;
        int             _port;

    public:
        Client();
        Client(const std::string& nick);
        ~Client();

        void            setNick(const std::string& nick);
        std::string     getNick() const;
        void            setId(unsigned int id);
        unsigned int    getId() const;
};

#endif
