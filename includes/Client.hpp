/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:48:40 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/14 19:30:49 by cpeterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Server;

class Client
{
	public:
		Client(Server *server);
		~Client();

		int				get_fd() const;
		void			set_fd(int fd);
		std::string		get_IPclient() const;
		void			set_IPclient(std::string IPclient);
		int				setUser(std::string name);
		std::string		getUser();
		int				setNick(std::string nick);
		std::string		getNick();
		int				setPerms(bool trueFalse);
		bool			getPerms();

		int				prvMsg(std::string input);
		int				joinChan(std::string target);
		int 			kickChan(std::string args);
		int				leaveChan(std::string target);
		int				inviteChan(std::string target);
		int				modChan(std::string target);
		int				topic(std::string target);

		int				setPassword(std::string command);
		bool			getPassword();

		void			receiveMsg(std::string msg);
		void			sendMsg(std::string msg);

		void			setRegistered(bool registered);
		bool			getRegistered();

		int			handleAddMode(std::string mode, std::string channel, std::string argument);
		int			handleRemoveMode(std::string mode, std::string channel, std::string argument);


	
	private:
		Client();
		Client(Client const &src);
		Client &operator=(Client const &rhs);
		Server			*_server;
		std::string		_username;
		std::string		_nickname;
		int				_clientFd;
		std::string		_IPclient;
		bool			_perms;
		bool			_password;
		bool			_registered;
		bool			_alreadyInChannel;
	
};

#endif