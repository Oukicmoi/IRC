/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:58:21 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/14 04:11:21 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
#define SERVER_HPP

class IRCMessage;
class Channel;

# include "lib.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "IRCMessage.hpp"

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
		std::map<std::string, void (Server::*)(User *, IRCMessage &)>	_cmds;
		
		std::vector<std::string>::iterator	getFirstNonModeArg(std::vector <std::string> &params);

		
		void	init(void);
		void	shutdown(void);
	
		void	init_socket_address(void);
		bool	set_non_blocking_socket(int socket_fd);
		bool	init_socket(void);
		bool	add_to_epoll(int socket_fd, uint32_t event_type);
		bool	init_epoll(void);
	
		void	handleNewClients(void);
		void	handleClient(const epoll_event& ev);
		bool	handleMsg(int fd, const std::string& line);
		
		void	loadCmds(void);
		bool	up(void);
		

    public:
		// SERVER CORE (construcors, destructors, overloads, setters, getters) ///////////////////
		Server(void);
		Server(unsigned int port);
		Server(const std::string& password);
		Server(unsigned int port, const std::string& password);
		~Server(void);

		
		const std::string	&getmdp() const { return (_mdp); };
		User*   getUser(int fd) const;
	    User*	getUserByNick(const std::string& nick);
	    Channel* getChannelByName(const std::string& name);
	
		bool	isValidNickname(const std::string& nick) const;
		bool	isNicknameInUse(const std::string& nick) const;
		
		void 	setmdp(const std::string& password) { _mdp = password; }
		Channel	*createChannel(const std::string &name, User *user);
		
		
		//////////////////////////////////////////////////////////////////////////////////////////
		
		
		// USER handling ////////////////////////////////////////////////////////////////////
		
		void	processUsername(std::string& username, const std::string& fallbackNick);
		
		void	sendWelcomeMessages(User* user);
		void	endAuthentification(User* user);

		void	clientQuits(int client_fd, std::string reason);

		void	sendMsgToUser(User *user, const std::string &message, const std::string target);
		static void	sendToUser(int const client_fd, std::string client_buffer);
		
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		// CHANNEL HANDLING ///////////////////////////////////////////////////////////////////
		
		void	userJoinChannel(User *user, const std::string &channelName, unsigned int key_index, std::vector<std::string> &keys);
		void	sendMsgToChannel(User *user, const std::string &message, const std::string &channelName);
		void	broadcastToAllChannels(User* user, const std::string& message);
			
		void	applyChannelMode(char mode, bool sign, User *user, Channel *channel, std::vector<std::string> &params);
		void	sendChannelModesToUser(User* user, Channel* channel, const std::vector<std::string>& params);
		void	applyChannelModes(User* user, Channel* channel, std::vector<std::string>& params);

		void	kickOneUser(User* kicker, Channel* chan, const std::string& targetNick, const std::string& reason);

		//////////////////////////////////////////////////////////////////////////////////////////////////////


		
		// COMMANDS //////////////////////////////////////////////////////////////////////////////////////////
	
		void	cmd_JOIN(User* user, IRCMessage &msg);
		void	cmd_PART(User* user, IRCMessage &msg);
		void	cmd_PRIVMSG(User* user, IRCMessage &msg);
		void	cmd_TOPIC(User* user, IRCMessage &msg);
		void	cmd_PASS(User* user, IRCMessage &msg);
		void	cmd_NICK(User* user, IRCMessage &msg);
		void	cmd_USER(User* user, IRCMessage &msg);
		void	cmd_PING(User* user, IRCMessage &msg);
		void	cmd_QUIT(User* user, IRCMessage& msg);
		void	cmd_MODE(User* user, IRCMessage& msg);
		void	cmd_KICK(User* user, IRCMessage& msg);
		void	cmd_INVITE(User* user, IRCMessage& msg);
		// void	cmd_CAP(User *user, IRCMessage &msg); // TODO

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void	run(void);
};


#endif