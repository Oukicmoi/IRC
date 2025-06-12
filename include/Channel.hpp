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


#include "User.hpp"

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

		
	public:
		Channel(const std::string& name);

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
		bool		isInviteOnly() const { return _inviteOnly; }
		bool		isTopicRestricted() const { return _topicRestricted; }
		std::string	getKey() const { return _key; }
		int			getUserLimit() const { return _userLimit; }
		time_t	 	getCreationTime() const { return _creationTime; }

		//SETTERS
		void	setInviteOnly(bool b) { _inviteOnly = b; }
		void	setTopicRestricted(bool b) { _topicRestricted = b; }
		void	setKey(const std::string& k) { _key = k; }
		void	removeKey() { _key = ""; }
		void	setUserLimit(int n) { _userLimit = n; }
		void	removeUserLimit() { _userLimit = -1; }


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

};

#endif
 