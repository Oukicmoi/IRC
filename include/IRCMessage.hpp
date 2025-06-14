/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:26:09 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/14 04:17:24 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCMESSAGE_HPP

# define IRCMESSAGE_HPP

#include "all.hpp"

class IRCMessage
{
	private:
		std::string _prefix;
		std::string _cmd;
		std::vector<std::string> _params;	

		size_t	parsePrefix(const std::string &rawMsg);
		size_t	parseCmd(const std::string &rawMsg);
		void	parseParams(const std::string &rawMsg);
		
	public:
		IRCMessage(const std::string &rawMsg);
		~IRCMessage(void) {};
		const std::vector<std::string>&  getParams() const { return (_params); };
		std::vector<std::string>&  getParams() { return (_params); };
		std::string getCmd() const { return (_cmd); };

		std::vector<std::string>::iterator	getFirstNonModeArg();
		
		void printParams() const;
};

#endif