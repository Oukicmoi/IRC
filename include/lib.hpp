/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:22:52 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/01 20:46:20 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef LIB_HPP
#define LIB_HPP


// C++ Standard Library
#include <iostream>     // cout, cerr
#include <string>       // std::string
#include <vector>       // std::vector
#include <map>          // std::map
#include <set>          // std::set
#include <sstream>      // std::istringstream, std::ostringstream
#include <algorithm>    // std::find, std::remove_if…
#include <cstring>      // std::memcpy, std::strlen
#include <cerrno>       // errno
#include <csignal>      // signal, sigaction
#include <cstdlib>      // std::exit, std::atoi

// POSIX / Sockets / I/O non-bloquante
#include <unistd.h>     // close, read, write, lseek
#include <fcntl.h>      // fcntl (O_NONBLOCK)
#include <sys/types.h>  // types divers
#include <sys/socket.h> // socket, bind, listen, accept, send, recv, setsockopt
#include <netinet/in.h> // sockaddr_in, htons, htonl, ntohs, ntohl
#include <arpa/inet.h>  // inet_addr, inet_ntoa
#include <netdb.h>      // getaddrinfo, freeaddrinfo, gethostbyname, getprotobyname
#include <poll.h>       // poll
#include <signal.h>     // signal, sigaction
#include <sys/stat.h>   // fstat (si utilisé)

#include <sys/epoll.h>
#include <tr1/unordered_map>
#include <sys/resource.h>

# define B "\x1B[1m"
# define U "\x1B[4m"
# define R "\033[0m"

# define RED "\033[0;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define CYAN "\033[0;36m"
# define MAGENTA "\033[0;35m"

# define NICKLEN 9
# define USERLEN 9

#define ERR_SYS(msg) std::cerr << RED << B << "error " << msg << ": " << R << std::strerror(errno) << std::endl

# include "config.hpp"

#endif