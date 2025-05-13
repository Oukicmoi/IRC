/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 21:18:14 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

#include "lib.hpp"
#include <sys/epoll.h>

# define ALLOW_NOT_RECOMMENDED_PORT 1
# define DEFAULT_PORT 6667
# define MAX_WAITING_ROOM 1024

class Server
{
    private:
        std::string			_mdp;
        unsigned int		_port;
		struct sockaddr_in	_server_addr;
		int					_socket_fd;
		int					_epoll_fd;
		void	init_socket_address(void);
		bool	init_socket(void);
	

    public:
        Server();
		Server(unsigned int port, const std::string& password);
        ~Server();

        void setmdp(const std::string& password);
        std::string getmdp() const;
        void setport(const unsigned int& password);
        unsigned int getport() const;

	
		bool	init(void);
};



#endif