/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/14 15:29:24 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

# include "lib.hpp"
# include "User.hpp"
# include <sys/epoll.h>
#include <tr1/unordered_map>
# include <sys/resource.h>

# define B "\x1B[1m"
# define U "\x1B[4m"
# define R "\033[0m"

# define RED "\033[0;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define CYAN "\033[0;36m"
# define MAGENTA "\033[0;35m"

# define ALLOW_NOT_RECOMMENDED_PORT true
# define DEFAULT_PORT 6667
# define MAX_WAITING_ROOM SOMAXCONN

#define ERR_SYS(msg) std::cerr << RED << B << "error " << msg << ": " << R << std::strerror(errno) << std::endl

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