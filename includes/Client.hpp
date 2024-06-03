/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:48:40 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/03 15:09:08 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Client
{
	public:
		Client();
		~Client();
		Client(Client const &src);

		Client &operator=(Client const &rhs);

		int				get_fd() const;
		void			set_fd(int fd);
		std::string		get_IPclient() const;
		void			set_IPclient(std::string IPclient);
		void			setUser(std::string name);
		void			setNick(std::string nick);
	
	private:
		std::string		_username;
		std::string		_nickname;
		int _clientFd;
		std::string _IPclient;
	
};

#endif