#ifndef IRC_MSGS_HPP

# define IRC_MSGS_HPP

// server_name -> nom technique, si jamais un réseau irc.libera.chat / irc2.libera.chat si network a plusieurs serveurs
// network_name -> nom public

# define IRC_MSG(content) (std::string(":") + HOSTNAME + " " + content + "\r\n")

// REPONSES COMMANDES STANDARTS /////////////////////////////////////////////////////////////

# define RPL_PONG(msg)															(std::string("PONG ") + SERVER_NAME + " :" + msg + "\r\n")
# define RPL_PRIVMSG(userFullName, target_nick, msg)							(":" + userFullName + " PRIVMSG " + target_nick + " :" + msg + "\r\n")
# define RPL_INVITE(userFullName, invited_nick, channel)						(":" + userFullName + " INVITE " + invited_nick + " :" + channel + "\r\n")
# define RPL_KICK(userFullName, channel, kicked_nick, reason)					(":" + userFullName + " KICK " + channel + " " + kicked_nick + " :" + reason + "\r\n")
# define RPL_JOIN(userFullName, channel)										(":" + userFullName + " JOIN " + channel + "\r\n")
# define RPL_QUIT(userFullName, reason)											(":" + userFullName + " QUIT :" + reason + "\r\n")
# define RPL_MODE(userFullName, channel, change)								(":" + userFullName + " MODE " + channel + " " + change + "\r\n")
# define RPL_MODE_WITH_ARG(userFullName, channel, change, change_args)			(":" + userFullName + " MODE " + channel + " " + change + " " + change_args + "\r\n")
# define RPL_SERVER_MODE(channel, change, change_args)							(":MODE " + channel + " " + change + " " + change_args + "\r\n")
# define RPL_TOPIC_SET(userFullName, channel, topic)							(":" + userFullName + " TOPIC " + channel + " :" + newTopic + "\r\n")
# define RPL_NICK(userFullName, newNick)										(":" + userFullName + " NICK :" + newNick + "\r\n")

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:

// RPL /////////////////////////////////////////////////////////////////////////////////////

// WELCOME MSG: PASS + NICK + USER -> authentified=WELCOME
# define RPL_WELCOME(nick, userFullName) 					 					(IRC_MSG("001 " + nick + " :Welcome to the " + NETWORK_NAME + " Network, " + userFullName))
# define RPL_YOURHOST(nick) 													(IRC_MSG("002 " + nick + " :Your host is " + HOSTNAME + ", running version " + SERVER_VERSION))
# define RPL_CREATED(nick, creation_date)										(IRC_MSG("003 " + nick + " :This server was created " + creation_date))
# define RPL_MYINFO(nick, user_modes, channel_modes, channel_modes_with_args)	(IRC_MSG("004 " + nick + " " + SERVER_NAME + " " + SERVER_VERSION + " " + user_modes + channel_modes + channel_modes_with_args))
# define RPL_ISUPPORT(nick, features)											(IRC_MSG("005 " + nick + " " + features + " :are supported by this server"))

// MODE -> send channel modes (+/- k/t/l/i/o)
# define RPL_CHANNELMODEIS(nick, channel, modes, modes_params)					(IRC_MSG("324 " + nick + " " + channel + " " + modes + " " + modes_params))
# define RPL_NOTOPIC(nick, channel)												(IRC_MSG("331 " + nick + " " + channel + " :No topic is set"))
# define RPL_TOPIC(nick, channel, topic)										(IRC_MSG("332 " + nick + " " + channel + " :" + topic))
# define RPL_INVITING(nick, invited_nick, channel)								(IRC_MSG("341 " + nick + " " + invited_nick + " " + channel + " :" + nick + " invited " + invited_nick + " to " + channel))
// JOIN -> send user all users on channel
# define RPL_NAMREPLY(nick, symbol, channel, nicklist)							(IRC_MSG("353 " + nick + " " + symbol + " " + channel + " :" + nicklist))
# define RPL_ENDOFNAMES(nick, channel)											(IRC_MSG("366 " + nick + " " + channel + " :End of /NAMES list"))
# define ERR_NOSUCHNICK(nick, target_nick)										(IRC_MSG("401 " + nick + " " + target_nick + " :No such nick/channel"))
# define ERR_NOSUCHCHANNEL(nick, channel)										(IRC_MSG("403 " + nick + " " + channel + " :No such channel"))
# define ERR_CANNOTSENDTOCHAN(nick, channel)									(IRC_MSG("404 " + nick + " " + channel + " :Cannot send to channel"))
// PRIVMSG -> no target found to send msg
# define ERR_NORECIPIENT(nick, cmd)												(IRC_MSG("411 " + nick + " :No recipient given (" + cmd + ")"))
# define ERR_NOTEXTTOSEND(nick)													(IRC_MSG("412 " + nick + " :No text to send"))
# define ERR_INPUTTOOLONG(nick)													(IRC_MSG("417 " + nick + " :Input line was too long"))
# define ERR_UNKNOWNCOMMAND(nick, cmd)											(IRC_MSG("421 " + nick + " " + cmd + " :Unknown command"))
# define ERR_NOMOTD(nick)														(IRC_MSG("422 " + nick + " :MOTD File is missing"))
# define ERR_NONICKNAMEGIVEN(nick)												(IRC_MSG("431 " + nick + ":No nickname given"))
# define ERR_ERRONEUSNICKNAME(nick, bad_nick)									(IRC_MSG("432 " + nick + " " + bad_nick + " :Erroneous nickname"))
# define ERR_NICKNAMEINUSE(nick, taken_nick)									(IRC_MSG("433 " + nick + " " + taken_nick + " :Nickname is already in use"))
# define ERR_USERNOTINCHANNEL(nick, target_nick, channel)						(IRC_MSG("441 " + nick + " " + target_nick + " " + channel + " :They aren't on that channel"))
# define ERR_NOTONCHANNEL(nick, channel)										(IRC_MSG("442 " + nick + " " + channel + " :You're not on that channel"))
# define ERR_USERONCHANNEL(nick, target_nick, channel)							(IRC_MSG("443 " + nick + " " + target_nick + " " + channel + " :is already on channel"))
# define ERR_ALREADYJOINED(nick, channel)										(IRC_MSG("443 " + nick + " " + channel + " :is already on channel"))
# define ERR_NOTREGISTERED(cmd)													(IRC_MSG("451 " + cmd + " :You have not registered"))
# define ERR_NEEDMOREPARAMS(nick, cmd)											(IRC_MSG("461 " + nick + " " + cmd + " :Not enough parameters"))
# define ERR_ALREADYREGISTERED(nick)											(IRC_MSG("462 " + nick + " :You may not reregister"))
# define ERR_PASSWDMISMATCH(nick)												(IRC_MSG("464 " + nick + " :Password incorrect"))
# define ERR_CHANNELISFULL(nick, channel)										(IRC_MSG("471 " + nick + " " + channel + " :Cannot join channel (+l)"))
# define ERR_INVITEONLYCHAN(nick, channel)										(IRC_MSG("473 " + nick + " " + channel + " :Cannot join channel (+i)"))
# define ERR_BADCHANNELKEY(nick, channel)										(IRC_MSG("475 " + nick + " " + channel + " :Cannot join channel (+k) - Invalid channel key"))
# define ERR_INVALIDKEY(nick, channel, reason)									(IRC_MSG("475 " + nick + " " + channel + " :Invalid channel key (" + reason + ")"))
# define ERR_BADCHANMASK(nick, channel)											(IRC_MSG("476 " + nick + " " + channel + " :Bad Channel Mask"))
# define ERR_BADCHANNAME(nick, channel)											(IRC_MSG("479 " + nick + " " + channel + " :Invalid channel prefix"))
# define ERR_ALREADYCHANOPRIVS(nick, channel)									(IRC_MSG("482 " + nick + " " + channel + " :You're already a channel operator"))
# define ERR_CHANOPRIVSNEEDED(nick, channel)									(IRC_MSG("482 " + nick + " " + channel + " :You're not a channel operator"))
# define ERR_LASTCHANOP(nick, channel)											(IRC_MSG("485 " + nick + " " + channel + " :You're the last channel operator"))
# define ERR_UMODEUNKNOWNFLAG(nick)												(IRC_MSG("501 " + nick + " :Unknown MODE flag"))
# define ERR_INVALIDMODEPARAM(nick, channel, mode, error_msg)					(IRC_MSG("696 " + nick + " " + channel + " " + mode + " :" + error_msg))

// # define RPL_INVITING(nick, invited_nick, channel) (IRC_MSG("341 " + nick + " " + invited_nick + " " + channel)) -> si jamais RPL_INVITING marche pas

#endif