/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:33:52 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/24 00:08:20 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCMessage.hpp"

size_t	IRCMessage::parsePrefix(const std::string &rawMsg)
{
	size_t i = 0;
	while (rawMsg[i] == ' ')
		i ++;
	if (rawMsg[i] != ':')
		return (i);
	size_t pos = rawMsg.find(' ');
	if (pos != std::string::npos)
		_prefix = rawMsg.substr(i + 1, pos);
	else
		_prefix = rawMsg.substr(i + 1);
	return (pos);
}

size_t IRCMessage::parseCmd(const std::string &rawMsg)
{
	if (rawMsg.empty())
		return (0);
	size_t i = 0;
	while (rawMsg[i] == ' ')
		i ++;
	size_t pos = rawMsg.find(' ');
	if (pos != std::string::npos)
		_cmd = rawMsg.substr(i, pos);
	else
		_cmd = rawMsg;
	return (pos);
}

void IRCMessage::parseParams(const std::string &rawMsg)
{
	if (rawMsg.empty())
		return;
	size_t i = 0;
	while (rawMsg[i] == ' ')
		i ++;
	if (rawMsg[0] == ':')
		_params.push_back(rawMsg.substr(1));
	else
	{
		size_t pos = rawMsg.find(' ');
		if (pos == std::string::npos)
			_params.push_back(rawMsg.substr(i));
		else
		{
			_params.push_back(rawMsg.substr(i, pos));
			parseParams(rawMsg.substr(pos + 1));
		}
	}
}

IRCMessage::IRCMessage(const std::string &rawMsg): _prefix(""), _cmd("")
{
	if (rawMsg.empty())
		return ;
	size_t	start = parsePrefix(rawMsg);
	if (start == std::string::npos)
		return ;
	start = parseCmd(rawMsg.substr(start));
	if (start == std::string::npos)
		return ;
	parseParams(rawMsg.substr(start));

	// std::cout << std::endl;
	// std::cout << "Treating msg " << rawMsg << "..." << std::endl;
	// std::cout << "Prefix: " << _prefix << std::endl;
	// std::cout << "Command: " << _cmd << std::endl;
	// std::cout << "Params: ";
	// size_t i = 0;
	// while (i < _params.size())
	// 	std::cout << _params[i ++] << " ";
	// std::cout << std::endl << std::endl;
}

IRCMessage::~IRCMessage(void) {}

std::string IRCMessage::getCmd(void) const { return (_cmd); }

const std::vector<std::string>&  IRCMessage::getParams() const { return _params; }
