/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octoross <octoross@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:22:52 by octoross          #+#    #+#             */
/*   Updated: 2025/06/15 16:33:09 by octoross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP

#define CONFIG_HPP

# define ALLOW_NOT_RECOMMENDED_PORT true
# define DEFAULT_PORT 6667
# define MAX_WAITING_ROOM SOMAXCONN
# define MAX_MSG_SIZE 512

# define NICKLEN 9
# define USERLEN 9
# define MAX_KEYLEN 30
# define MIN_KEYLEN 5

# define SERVER_VERSION "1.0"
# define SERVER_NAME "localhost"
# define NETWORK_NAME "jeffrey"
# define HOSTNAME "localhost"

# define TEST_WITH_NC false

#endif