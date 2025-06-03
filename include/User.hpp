/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:21:41 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/03 22:12:15 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "lib.hpp"

class User
{
	protected:
		unsigned int	_id;
		static unsigned int _nextId;
		int		_port;
		int		_socket_fd;
		char		_buffer[MAX_MSG_SIZE + 1];
		std::string	clientBuffers;
		std::string	_Nickname;
		std::string	_Username;
		std::string	_Host;
		std::string	_password;
		bool		_registered;
		bool		_passValid;

	public:
		User();
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
		bool isRegistered() const { return _registered; }
		void setRegistered(bool status) { _registered = status; }
		void setPasswordValidated(bool status) { _passValid = status; }
		bool isPasswordValidated() const { return _passValid; }
		// ssize_t	send(const std::string& msg, int flags = 0);

		bool hasUsername() const { return !_Username.empty(); }
		
		void            setPort(int port);
		int             getPort() const;

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
