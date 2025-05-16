/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/16 22:09:27 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

# include "lib.hpp"
# include "User.hpp"
# include <sys/epoll.h>
#include <tr1/unordered_map>
# include <sys/resource.h>


class Server
{
    private:
        std::string						_mdp;
        unsigned int					_port;
		struct sockaddr_in				_server_addr;
		int								_socket_fd;
		int								_epoll_fd;
		std::map<int, User *>			_users;

		void	init(void);
		void	shutdown(void);
		void	init_socket_address(void);
		bool	set_non_blocking_socket(int socket_fd);
		bool	init_socket(void);
		bool	add_to_epoll(int socket_fd, uint32_t event_type);
		bool	init_epoll(void);
		void	addUsers(void);
		void	handleClient(const epoll_event& ev);
		bool	up(void);

    public:
        Server(void);
		Server(unsigned int port);
		Server(const std::string& password);
		Server(unsigned int port, const std::string& password);
        ~Server(void);

        void setmdp(const std::string& password);
        std::string getmdp() const;
        unsigned int getport() const;
		const std::map<int, User*>&  getUsers() const;
	    User*   getUser(int fd) const;
	
		void	run(void);
};


#endif