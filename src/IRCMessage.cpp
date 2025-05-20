/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:33:52 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/20 16:42:38 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCMessage.hpp"

size_t	IRCMessage::parsePrefix(const std::string &rawMsg)
{
	if (rawMsg[0] != ':')
		return (0);
	size_t pos = rawMsg.find(' ');
	if (pos != std::string::npos)
		_prefix = rawMsg.substr(1, pos - 1);
	return (pos + 1);
}

size_t IRCMessage::parseCmd(const std::string &rawMsg)
{
	if (rawMsg.empty())
		return (0);
	size_t pos = rawMsg.find(' ');
	if (pos != std::string::npos)
		_cmd = rawMsg.substr(0, pos - 1);
	return (pos + 1);
}

void IRCMessage::parseParams(const std::string &rawMsg)
{
	if (rawMsg.empty())
		return;
	if (rawMsg[0] == ':')
		_params.push_back(rawMsg.substr(1));
	else
	{
		size_t pos = rawMsg.find(' ');
		_params.push_back(rawMsg.substr(0, pos - 1));
		parseParams(rawMsg.substr(pos + 1));
	}
}

IRCMessage::IRCMessage(const std::string &rawMsg): _prefix(""), _cmd("")
{
	if (rawMsg.empty())
		return;
	size_t	start = parsePrefix(rawMsg);
	start = parseCmd(rawMsg.substr(start));
	if (start == 0)
		return;
	parseParams(rawMsg.substr(start));

	std::cout << "Prefix: " << _prefix << std::endl;
	std::cout << "Command: " << _cmd << std::endl;
	std::cout << "Params: ";
	size_t i = 0;
	while (i < _params.size())
		std::cout << _params[i ++] << " ";
}

IRCMessage::~IRCMessage(void) {}

std::string IRCMessage::getCmd(void) const { return (_cmd); }