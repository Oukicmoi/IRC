/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 22:33:59 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

# include "lib.hpp"
# include "User.hpp"
# include <sys/epoll.h>
#include <tr1/unordered_map>

# define ALLOW_NOT_RECOMMENDED_PORT true
# define DEFAULT_PORT 6667
# define MAX_WAITING_ROOM 1024

class Server
{
    private:
        std::string						_mdp;
        unsigned int					_port;
		struct sockaddr_in				_server_addr;
		int								_socket_fd;
		int								_epoll_fd;
		std::tr1::unordered_map<int, User>	_users;

		void	init_socket_address(void);
		bool	init_socket(void);
		bool	init_epoll(void);

    public:
        Server();
		Server(unsigned int port, const std::string& password);
        ~Server();

        void setmdp(const std::string& password);
        std::string getmdp() const;
        unsigned int getport() const;

	
		bool	init(void);
		void	up(void);
};



#endif