/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/03 22:36:18 by gtraiman         ###   ########.fr       */
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

		bool	isValidNickname(const std::string& nick) const;
		bool	isNicknameInUse(const std::string& nick) const;
		void	broadcastToAllChannels(User* user, const std::string& message);
		void	sendWelcomeMessages(User* user);

		std::string	parseRealName(const std::vector<std::string>& params, size_t index);
		void	processUsername(std::string& username, const std::string& fallbackNick);
		void	EndRegister(User* user);


		const std::map<std::string, Channel *>& getChannels() const;

		std::map<std::string, Channel *>& getChannels();

		void	setChannels(const std::map<std::string, Channel *>& channels);
		void	sendChannelModes(User* user, Channel* c, const std::vector<std::string>& p);
		void	applyChannelModes(User* user, Channel* c, const std::vector<std::string>& p);
		bool	handleModeChar(User* u, Channel* c, const std::vector<std::string>& p, size_t& i, char m, bool add);

		// void handleLine(int fd, const std::string& line);
		
		Channel* getOrCreateChannel(const std::string& name, User& u);
		void	cmd_JOIN(User* user, const IRCMessage &msg);
		void	cmd_PART(User* user, const IRCMessage &msg);
		void	cmd_PRIVMSG(User* user, const IRCMessage &msg);
		void	cmd_TOPIC(User* user, const IRCMessage &msg);
		void	cmd_MSG(User* user, const IRCMessage &msg);
		void	cmd_PASS(User* user, const IRCMessage &msg);
		void	cmd_NICK(User* user, const IRCMessage &msg);
		void	cmd_USER(User* user, const IRCMessage &msg);
		void	cmd_PING(User* user, const IRCMessage &msg);
		void	cmd_QUIT(User* user, const IRCMessage& msg);
		void	cmd_MODE(User* user, const IRCMessage& msg);

};


#endif