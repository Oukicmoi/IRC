/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:26:09 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/24 00:07:53 by gtraiman         ###   ########.fr       */
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
		~IRCMessage(void);
		const std::vector<std::string>&  getParams() const;
		std::string getCmd() const;
};

#endif