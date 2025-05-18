/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/18 23:47:59 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

# include "lib.hpp"
# include "User.hpp"
# include "Channel.hpp"



class Server
{
    private:
        std::string						_mdp;
        unsigned int					_port;
		struct sockaddr_in				_server_addr;
		int								_socket_fd;
		int								_epoll_fd;
		std::map<int, User *>			_users;
		std::map<std::string,Channel*> _channels;

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

		void handleLine(int fd, const std::string& line);
		
		Channel* getOrCreateChannel(const std::string& name, User& u);
		void     cmd_JOIN(User* u, const std::vector<std::string>& params);
		void     cmd_PART(User* u, const std::vector<std::string>& params);
		void     cmd_PRIVMSG(User* u, const std::vector<std::string>& params);
		void     cmd_TOPIC(User* u, const std::vector<std::string>& params);
};


#endif