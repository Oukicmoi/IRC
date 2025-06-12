/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:26:09 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/08 02:01:52 by octoross         ###   ########.fr       */
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
		void printParams() const;
		IRCMessage(const std::string &rawMsg);
		~IRCMessage(void);
		const std::vector<std::string>&  getParams() const;
		std::vector<std::string>&  getParams();
		std::string getCmd() const;
};

#endif