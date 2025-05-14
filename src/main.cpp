/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:57:35 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/14 13:20:35 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void	show_limits(void)
{
	std::cout << "Socket Maximum Connections: " << B << MAX_WAITING_ROOM << R << "/" << SOMAXCONN << std::endl;
	struct rlimit rlim;
	// Récupère la limite de descripteurs de fichiers (RLIMIT_NOFILE)
	std::cout << "Opened Fd limits" << std::endl;
	if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
	{
		std::cout << "\t- Limite actuelle (soft): " << B << (long long)rlim.rlim_cur << R << std::endl;
		std::cout << "\t- Limite maximale (hard): " << (long long)rlim.rlim_max << std::endl;
	}
	else
		std::cout << "getrlimit failed: " << strerror(errno) << std::endl;
	std::cout << std::endl;
}

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

		show_limits();
	
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