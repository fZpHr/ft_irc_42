/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:56:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/05/22 15:35:12 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

/**
 * @brief: main function
 * 
 * @param ac the number of arguments
 * @param av the arguments passed to the program (port and password)
 * 
*/
int main(int ac, char **av)
{
	if (handle_input(ac, av) == 1)
		return (1);
	return (0);
}