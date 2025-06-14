/* ************************************************************************** */
/*				                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:26:03 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/06 17:16:46 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Server;

#include "User.hpp"
#include "Server.hpp"

class Channel
{

	private:
		bool		_inviteOnly;
		bool		_topicRestricted;
		bool		_hasKey;
		int		 	_userLimit;
		
		time_t	  	_creationTime;
		time_t 		_topicSetTime;
		
		std::string	_name;
		std::string _key;
		std::string	_topic;
		std::string _topicSetter;

		std::set<User*>  _members;
		std::set<User*>  _operators;
		std::set<User*>  _inviteList;
		Server			&_server;

		
	public:
		Channel(const std::string& name, Server &server);

		const std::string	&getName() const { return (_name); };
		const std::string	&getTopic() const { return (_topic); };
		const std::string	&getTopicSetter() const { return (_topicSetter); }
		const time_t		&getTopicSetTime() const { return (_topicSetTime); }
		const std::string	&getKey() const { return (_key); }
		const time_t		&getCreationTime() const { return (_creationTime); }
		bool		isTopicRestricted() const { return (_topicRestricted); }
		bool		isEmpty() const { return (_members.empty()); }
		int	 		getSize() const { return (_members.size()); }
		bool  		isMember(User* user) const { return (_members.find(user) != _members.end()); }
		bool  		isOperator(User* user) const { return (_operators.find(user) != _operators.end()); }
		bool		userOnInviteList(User *user) { return (_inviteList.find(user) != _inviteList.end()); }
		bool		isInviteOnly() const { return (_inviteOnly); }
		bool		hasKey() const { return (_hasKey); }
		int			getUserLimit() const { return (_userLimit); }
		
		
		std::string	getNickList() const;
		
		void	setInviteOnly(bool invite) { _inviteOnly = invite; }
		void	setTopicRestricted(bool topicRestricted) { _topicRestricted = topicRestricted; }
		void	setTopic(const std::string& topic) { _topic = topic; }
		void	setTopicSetter(const std::string& setter) { _topicSetter = setter; }
		void	setTopicSetTime(std::time_t t) { _topicSetTime = t; }
		void	setKey(const std::string& key) { _key = key; }
		void	setUserLimit(int n) { _userLimit = n; }

		bool  	addMember(User* user);
		void  	removeMember(User* user);
		bool  	addOperator(User* user);
		void  	removeOperator(User* user);

		
		void	rmFromInviteList(User *user) { _inviteList.erase(user); }
		bool	canUserJoin(User *user, const std::string *password);
		bool 	userJoin(User *user, std::string *password = NULL);
		
		
		// MODES
		void	mode_invite(bool sign, std::string &userPrefix);
		void	mode_topicRestriction(bool sign, std::string &userPrefix);
		void	mode_key(bool sign, User *user, std::string *password = NULL);
		void	mode_operators(bool sign, User* user, User *target);
		void	mode_userLimit(bool sign, User *user, std::string *limit = NULL);
		
		void	applyMode(char mode, bool sign, User *user, IRCMessage &msg);
		void	applyModes(User* user, IRCMessage &msg);
		void	sendModesToUser(User* user);

		void	sendWelcomeInfo(User *user);
		void  	broadcast(const std::string& message, User* except = NULL) const;
		
		void	printOperatorse() const;
		void	printMembers() const;
};

#endif
 