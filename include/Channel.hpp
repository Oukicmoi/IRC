/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:26:03 by gtraiman          #+#    #+#             */
/*   Updated: 2025/06/02 23:30:12 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include "User.hpp"

class Channel
{

    private:
        std::string      _name;
        std::string      _topic;
        std::set<User*>  _members;
        std::set<User*>  _operators;
        bool        _inviteOnly;      // +i
        bool        _topicRestricted; // +t
        std::string _key;             // +k
        int         _userLimit;       // +l, -1 si pas de limite
        time_t      _creationTime;    // Pour 329 RPL_CREATIONTIME
        
    public:
        Channel(const std::string& name);

        // Accesseurs
        const std::string& getName()  const;
        const std::string& getTopic() const;

        const std::set<User*>& getMembers() const;
        // Membres
        bool   addMember(User* u);
        void   removeMember(User* u);
        bool   isMember(User* u) const;

        // Opérateurs
        bool   addOperator(User* u);
        void   removeOperator(User* u);
        bool   isOperator(User* u) const;

        // Topic
        void   setTopic(const std::string& t);

        // Broadcast
        void   broadcast(const std::string& message, User* except = NULL) const;

        // GETTERS
        bool        isInviteOnly() const { return _inviteOnly; }
        bool        isTopicRestricted() const { return _topicRestricted; }
        std::string getKey() const { return _key; }
        int         getUserLimit() const { return _userLimit; }
        time_t      getCreationTime() const { return _creationTime; }

        //SETTERS
        void setInviteOnly(bool b) { _inviteOnly = b; }
        void setTopicRestricted(bool b) { _topicRestricted = b; }
        void setKey(const std::string& k) { _key = k; }
        void removeKey() { _key = ""; }
        void setUserLimit(int n) { _userLimit = n; }
        void removeUserLimit() { _userLimit = -1; }
};

#endif
 