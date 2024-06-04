/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:48:40 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/04 18:25:34 by hbelle           ###   ########.fr       */
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
		int			setUser(std::string name);
		std::string	getUser();
		int			setNick(std::string nick);
		std::string	getNick();
		void			setPerms(bool perms);
		bool			getPerms();
		int			prvMsg(std::string input);
		int			joinChan(std::string target);

		void		receiveMsg(std::string msg);
	
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
	
};

#endif