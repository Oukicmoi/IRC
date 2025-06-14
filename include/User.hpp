/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:21:41 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/14 20:07:15 by octoross         ###   ########.fr       */
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
		std::string	_Nickname;
		std::string	_Username;
		std::string	_password;
		std::string	_Host;
		std::string	_HostMask;
		
		std::string getIPFromSocket(int socket_fd);
		
	public:
		std::string	_recvBuffer;
		User(int socket_fd);
		~User();
		
		int					getSocketFd() const { return (_socket_fd); }
		const std::string	&getNick() const { return (_Nickname); }
		const std::string	&getUsername() const { return (_Username); }
		const std::string	&getPassword() const { return (_password); }
		bool				isAuthentified() const { return (_authentified); }
		bool				isPasswordValid() const { return _passValid; }
		bool				hasUsername() const { return (!_Username.empty()); }
		const std::string	&getHost() const { return (_Host); };
		std::string			getFullNameMask() const { return (_Nickname + "!~" + _Username + "@" + _HostMask); };
		std::string			getOldFullNameMask(const std::string &oldNick) const { return (oldNick + "!~" + _Username + "@" + _HostMask); };
		
		void	setNick(const std::string& nick) { _Nickname = nick; };
		void	setUsername(const std::string& username) {_Username = username; }
		void	setPassword(const std::string& pass) { _password = pass; }
		void	setAuthentified(bool status) { _authentified = status; }
		void	setPasswordValid(bool status) { _passValid = status; }
};

#endif
