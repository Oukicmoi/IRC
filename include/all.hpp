/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:24:19 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/13 23:22:19 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef ALL_HPP
#define ALL_HPP

#include "Channel.hpp"
#include "User.hpp"
#include "IRCMessage.hpp"
#include "IRC_MSG.hpp"
#include "Server.hpp"

template <typename T>
T	typeOfString(const std::string &str)
{
	std::istringstream iss(str);
    T	value;
    
    if (!(iss >> value))
		throw std::runtime_error(str + " can't be converted to desired type");


	char remaining;
	if (iss.get(remaining))
		throw std::runtime_error(str + " can't be converted to desired type");
    return (value);
}

#endif