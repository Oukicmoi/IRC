/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:02:13 by gtraiman          #+#    #+#             */
/*   Updated: 2025/05/31 18:02:13 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

void Server::cmd_PASS(User* user, IRCMessage& msg)
{
    const std::vector<std::string>& params = msg.getParams();

    // 1. Vérification des paramètres
    if (params.empty())
    {
        sendToUser(user->getSocketFd(), ERR_NEEDMOREPARAMS(user->getNick(), "PASS"));
        return;
    }

    // 2. Vérification état utilisateur
    if (user->isRegistered())
    {
        sendToUser(user->getSocketFd(), ERR_ALREADYREGISTERED(user->getNick()));
        return;
    }

    // 3. Validation du mot de passe
    const std::string& givenPass = params[0];
    std::cout << "params : " << msg.getParams()[0] << std::endl;
    std::cout << "mdp : " << this->_mdp <<  std::endl;
    if (givenPass != this->_mdp)
    {
        sendToUser(user->getSocketFd(), ERR_PASSWDMISMATCH(user->getNick()));
        // Optionnel : close(user->getSocketFd());
        return;
    }
        std::cout << "here" << std::endl;


    // 4. Authentification réussie
    user->setPasswordValidated(true);
    std::cout << "║\t[Auth] Client " << user->getSocketFd() << " (" << user->getNick() << ") authenticated\n";
}
