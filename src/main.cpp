/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:57:35 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/13 22:19:54 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"
int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Usage: " << av[0] << " <port> <password>\n";
        return 1;
    }
    try
    {
        char **end = NULL;
        Server server(std::strtod(av[1],end), av[2]);
		if (!server.init())
			return (1);
		server.up();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() <<std::endl;    
    }
    return(0);
}