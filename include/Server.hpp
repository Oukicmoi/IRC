/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/16 10:39:06 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

# include "lib.hpp"
# include "User.hpp"
# include <sys/epoll.h>
#include <tr1/unordered_map>
# include <sys/resource.h>



#define ERR_SYS(msg) std::cerr << RED << B << "error " << msg << ": " << R << std::strerror(errno) << std::endl

class Server
{
    private:
        std::string						_mdp;
        unsigned int					_port;
		struct sockaddr_in				_server_addr;
		int								_socket_fd;
		int								_epoll_fd;
		std::map<int, User *>			_users;

		void	init_socket_address(void);
		bool	set_non_blocking_socket(int socket_fd);
		bool	init_socket(void);
		bool	add_to_epoll(int socket_fd, uint32_t event_type);
		bool	init_epoll(void);
		void	addUsers(void);

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