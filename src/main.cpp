/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:57:35 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/21 00:15:11 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

volatile bool g_running = true;

std::string formatTime(time_t timestamp)
{
    char buffer[80];
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

std::string	cleanIRCLine(const std::string &line, int n)
{
	std::string copy = "";
	long unsigned int i = 0;
	while ((i < line.size()) && ((n < 0) || (i < (long unsigned int)n)))
	{
		if (line[i] == '\n')
			copy += "\\n";
		else if (line[i] == '\r')
			copy += "\\r";
		else
			copy += line[i];
		i ++;
	}
	return (copy);
}

std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::string item;
    std::istringstream iss(s);
    while (std::getline(iss, item, delim))
        elems.push_back(item);
    return (elems);
}

void	show_limits(void)
{
	std::cout << "Socket Maximum Connections: " << B << MAX_WAITING_ROOM << R << "/" << SOMAXCONN << std::endl;
	struct rlimit rlim;
	// Récupère la limite de descripteurs de fichiers
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

void signal_handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM || signal == SIGTSTP)
    {
		if (signal == SIGINT)
        	std::cout << BRED << "\nSIGINT";
		else if (signal == SIGTERM)
        	std::cout << BRED << "\nSIGTERM";
		else if (signal == SIGTSTP)
        	std::cout << BRED << "\nSIGTSTP";
		std::cout << R << " catch" << std::endl;
        g_running = false;
    }
}

void	setup_signals(void)
{
	struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
}

int main(int ac, char **av)
{
	if (ac != 3)
    {
		std::cerr << "Usage: " << av[0] << " <port> <password>\n";
        return (1);
    }
	
	setup_signals();
	show_limits();
    try
    {
        char **end = NULL;
        Server server(std::strtod(av[1], end), av[2]);
		server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() <<std::endl;    
    }
    return(0);
}