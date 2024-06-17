/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:56:50 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/17 16:44:49 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

/**
 * For debug purposes, change the value to 1 with the following command: make debug
*/
# ifndef DEBUG
# define DEBUG	0
#endif

/**
 * all the includes needed for the project
*/
#include <iostream> // Fournit les fonctionnalités d'entrée/sortie standard
#include <string> // Fournit la classe string pour manipuler les chaînes de caractères
#include <vector> // Fournit la classe vector pour manipuler des tableaux dynamiques
#include <map> // Fournit la classe map pour manipuler des tableaux associatifs
#include <list> // Fournit la classe list pour manipuler des listes
#include <algorithm> // Fournit des fonctions pour divers algorithmes comme le tri, la recherche, etc.
#include <iterator> // Fournit des classes d'itérateurs pour manipuler les conteneurs STL
#include <sstream> // Fournit la classe stringstream pour manipuler les flux de chaînes
#include <fstream> // Fournit les classes ifstream et ofstream pour lire et écrire dans des fichiers
#include <sys/types.h> // Fournit des types de données pour les appels système
#include <sys/socket.h> // Fournit les fonctions pour la programmation de sockets
#include <netinet/in.h> // Fournit des définitions pour les structures de données utilisées dans les appels de système de réseau
#include <arpa/inet.h> // Définit des opérations sur les adresses Internet
#include <netdb.h> // Fournit des fonctions pour la résolution de noms de domaine et de protocoles
#include <unistd.h> // Fournit des fonctions pour les opérations de système d'exploitation de base
#include <sys/stat.h> // Fournit des fonctions pour obtenir des informations sur les fichiers
#include <fcntl.h> // Fournit des fonctions pour manipuler les descripteurs de fichiers
#include <poll.h> // Fournit des fonctions pour surveiller plusieurs descripteurs de fichiers en attente d'événements
#include <signal.h> // Fournit des fonctions pour gérer les signaux dans un programme
#include <errno.h> // Fournit des macros pour signaler les erreurs
#include <regex.h> // Fournit des fonctions pour manipuler les expressions régulières
#include <cstring> // Fournit des fonctions pour manipuler les chaînes de caractères
#include <ctime> // Fournit des fonctions pour manipuler les dates et les heures
#include <string> // Fournit la classe string pour manipuler les chaînes de caractères

/**
 * police defines
*/
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define INVERT "\033[7m"
#define HIDDEN "\033[8m"
#define STRIKE "\033[9m"
#define DEFAULT "\033[0m"
#define LIGHT "\033[1m"
#define DARK "\033[2m"
#define BOLD "\033[1m"
#define BLACK "\033[0;90m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"
#define BBLACK "\033[1;30m"
#define BRED "\033[1;31m"
#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BBLUE "\033[1;34m"
#define BMAGENTA "\033[1;35m"
#define BCYAN "\033[1;36m"
#define BWHITE "\033[1;37m"
#define BRESET "\033[1;0m"


/**
 * regex defines
*/
#define REGEXNICKNAME "^[a-zA-Z][a-zA-Z0-9_.-]{0,8}$"


# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "error.hpp"
# include "response.hpp"

// Parsing
int handle_input(int ac, char **av);

// Utils
std::string getCurrentTime();
int	handleMode(std::string mode);

#endif