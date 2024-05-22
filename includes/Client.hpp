/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:48:40 by hbelle            #+#    #+#             */
/*   Updated: 2024/05/22 17:07:56 by hbelle           ###   ########.fr       */
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
	
	private:
		int _clientFd;
		std::string _IPclient;
	
};

#endif