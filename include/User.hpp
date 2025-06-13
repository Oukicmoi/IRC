/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:21:41 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/13 19:14:26 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "lib.hpp"

class User
{
	protected:
		bool		_authentified;
		bool		_passValid;
		char		_buffer[MAX_MSG_SIZE + 1];
		int			_socket_fd;
		unsigned int	_id;
		static unsigned int _nextId;
		std::string	clientBuffers;
		std::string	_Nickname;
		std::string	_Username;
		std::string	_Host;
		std::string	_password;

		std::string getIPFromSocket(int socket_fd);
	
	public:
		User(int socket_fd);
		// User(int socket_fd, const std::string& nick);
		~User();
		

		void			setNick(const std::string& nick);
		std::string	 	getNick() const;
		std::string		getUsername() const;
		void			setId(unsigned int id);
		unsigned int		getId() const;
		int			getSocketFd() const;
		std::string&		recvBuffer();
		const std::string&	recvBuffer() const;

		void setPassword(const std::string& pass) { _password = pass; }
		const std::string& getPassword() const { return _password; }
		bool isAuthentified() const { return _authentified; }
		void setRegistered(bool status) { _authentified = status; }
		void setPasswordValidated(bool status) { _passValid = status; }
		bool isPasswordValidated() const { return _passValid; }
		// ssize_t	send(const std::string& msg, int flags = 0);

		bool hasUsername() const { return !_Username.empty(); }
	

		void            setUsername(const std::string& username);
		// getUsername() existe déjà

		void            setHost(const std::string& host);
		std::string     getHost() const;

		std::string&    getClientBuffers();
		const std::string& getClientBuffers() const;
		void            setClientBuffers(const std::string& buf);
		std::string getPrefix() const;

		char*           getBuffer();
		const char*     getBuffer() const;

		static unsigned int getNextId();

};

#endif
