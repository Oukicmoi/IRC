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
		bool		_inviteOnly;	  // +i
		bool		_topicRestricted; // +t
		bool		_hasKey;
		int		 	_userLimit;	   // +l, -1 si pas de limite
		
		time_t	  	_creationTime;	// Pour 329 RPL_CREATIONTIME
		time_t 		_topicSetTime;
		
		std::string	_name;
		std::string _key;			 // +k
		std::string	_topic;
		std::string _topicSetter;

		std::set<User*>  _members;
		std::set<User*>  _operators;
		std::set<User*>  _inviteList;
		Server			&_server;

		
	public:
		Channel(const std::string& name, Server &server);

		// Accesseurs
		const std::string&	getName()  const;

		const std::set<User*>&	getMembers() const;
		// Membres
		bool  	addMember(User* u);
		void  	removeMember(User* u);
		bool  	isMember(User* u) const;

		// Opérateurs
		bool  	addOperator(User* u);
		void  	removeOperator(User* u);
		bool  	isOperator(User* u) const;



		// Broadcast
		void  	broadcast(const std::string& message, User* except = NULL) const;

		// GETTERS
		bool		userOnInviteList(User *user) { return (_inviteList.find(user) != _inviteList.end()); }
		bool		isInviteOnly() const { return _inviteOnly; }
		bool		isTopicRestricted() const { return _topicRestricted; }
		std::string	getKey() const { return _key; }
		bool		hasKey() const { return _hasKey; }
		int			getUserLimit() const { return _userLimit; }
		time_t	 	getCreationTime() const { return _creationTime; }
		int	 		getSize() const { return _members.size(); }

		//SETTERS
		void	setInviteOnly(bool b) { _inviteOnly = b; }
		void	setTopicRestricted(bool b) { _topicRestricted = b; }
		void	setKey(const std::string& k) { _key = k; }
		void	removeKey() { _key = ""; }
		void	setUserLimit(int n) { _userLimit = n; }
		void	removeUserLimit() { _userLimit = -1; }
		
		void	rmFromInviteList(User *user) { _inviteList.erase(user); }
		bool	canUserJoin(User *user, const std::string *password);
		bool 	userJoin(User *user, std::string *password = NULL);


		//// TOPIC

		void	setTopic(const std::string& topic);
		std::string	getTopic() const;

		void	setTopicSetter(const std::string& setter);
		std::string	getTopicSetter() const;

		void	setTopicSetTime(std::time_t t);
		std::time_t	getTopicSetTime() const;

		bool	isTopicProtected() const; // true si +t
		void	printOperatorse() const;
		void	printMembers() const;


		// MODE
		
		void	mode_invite(bool sign, std::string &userPrefix);
		void	mode_topicRestriction(bool sign, std::string &userPrefix);
		void	mode_key(bool sign, std::string &userPrefix, std::string *password = NULL);
		void	mode_operators(bool sign, User* user, User *target);
		void	mode_userLimit(bool sign, User *user, std::string *limit = NULL);

};

#endif
 