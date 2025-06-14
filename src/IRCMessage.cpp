/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:33:52 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/14 04:17:08 by octoross         ###   ########.fr       */
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
	size_t i = 0;
	while (i < rawMsg.length() && rawMsg[i] == ' ')
		i++;

	while (i < rawMsg.length())
	{
		if (rawMsg[i] == ':') // paramètre final
		{
			_params.push_back(rawMsg.substr(i + 1));
			break;
		}

		size_t nextSpace = rawMsg.find(' ', i);
		if (nextSpace == std::string::npos)
		{
			_params.push_back(rawMsg.substr(i));
			break;
		}
		else
		{
			_params.push_back(rawMsg.substr(i, nextSpace - i));
			i = nextSpace + 1;
			while (i < rawMsg.length() && rawMsg[i] == ' ') // skip espaces multiples
				i++;
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
}

void IRCMessage::printParams() const
{
	for (size_t i = 0; i < _params.size(); ++i)
		std::cout << "Param[" << i << "]: \"" << _params[i] << "\"" << std::endl;
}

std::vector<std::string>::iterator	IRCMessage::getFirstNonModeArg()
{
	std::vector<std::string>::iterator it = _params.begin();
	it ++;
	while (it != _params.end())
	{
		if ((*it).empty())
			return (it);
		else if (((*it)[0] != '-') && ((*it)[0] != '+'))
			return (it);
		it ++;
	}
	return (it);
}
