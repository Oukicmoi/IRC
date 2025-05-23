/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/20 17:27:24 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

# include "lib.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "IRCMessage.hpp"

class IRCMessage;

class Server
{
    private:
		int								_socket_fd;
		int								_epoll_fd;
		unsigned int					_port;
		struct sockaddr_in				_server_addr;
		std::string						_server_name;
		std::string						_mdp;
		std::map<int, User *>			_users;
		std::map<std::string, Channel *> _channels;
		std::map<std::string, void (Server::*)(User *, const IRCMessage &)>	_cmds;

		void	init(void);
		void	shutdown(void);
	
		void	init_socket_address(void);
		bool	set_non_blocking_socket(int socket_fd);
		bool	init_socket(void);
		bool	add_to_epoll(int socket_fd, uint32_t event_type);
		bool	init_epoll(void);
	
		void	handleNewClients(void);
		void	handleClient(const epoll_event& ev);
		void	handleMsg(int fd, const std::string& line);
		
		void	loadCmds(void);
		bool	up(void);
		
		void	welcome(User *user);
		void	cmd_CAP(User *user, const IRCMessage &msg);

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

		// void handleLine(int fd, const std::string& line);
		
		Channel* getOrCreateChannel(const std::string& name, User& u);
		void	cmd_JOIN(User* user, const IRCMessage &msg);
		void	cmd_PART(User* user, const IRCMessage &msg);
		void	cmd_PRIVMSG(User* user, const IRCMessage &msg);
		void	cmd_TOPIC(User* user, const IRCMessage &msg);
		void	cmd_MSG(User* user, const IRCMessage &msg);
};


#endif