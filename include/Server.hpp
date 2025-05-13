/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 16:23:52 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

#include "lib.hpp"


class Server
{
    protected:
        std::string mdp;
        unsigned int port;

    public:
        Server();
        Server(const std::string& password);
        ~Server();

        void setmdp(const std::string& password);
        std::string getmdp() const;
        void setport(const unsigned int& password);
        unsigned int getport() const;
};



#endif