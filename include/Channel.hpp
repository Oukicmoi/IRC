/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:26:03 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/18 23:23:55 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include "User.hpp"

class Channel
{

    private:
        std::string      _name;
        std::string      _topic;
        std::set<User*>  _members;
        std::set<User*>  _operators;
        // + clés, limites, modes…
        
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
};

#endif
 