/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlancar <dlancar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/04 14:13:26 by dlancar           #+#    #+#             */
/*   Updated: 2014/02/04 16:23:31 by dlancar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <libft.h>

#define MEM_SIZE 30000
#define STACK    255

static unsigned char		memory[MEM_SIZE] = {0};
static t_array	*loop_stack;
static unsigned char		*pc = 0;

static void	init(void)
{
	loop_stack = array_new(10, 10, sizeof(unsigned int), 0);
}

/*
** VM spec : Circular memory of MEM_SIZE unsigned char.
*/
static void	run(t_array	*prog)
{
	unsigned char	temp;

	pc = memory;
	while (prog->it < prog->size)
	{
		temp = *(unsigned char *)array_next(prog);
		switch (temp)
		{
			case '>' :
				pc = (pc + 1 < memory + MEM_SIZE) ? pc + 1 : memory;
				break ;
			case '<' :
				pc = (pc - 1 >= memory) ? pc - 1 : memory + MEM_SIZE;
				break ;
			case '+' :
				(*pc)++;
				break ;
			case '-' :
				(*pc)--;
				break ;
			case '.' :
				write(STDOUT, pc, 1);
				break ;
			case ',' :
				read(STDIN, pc, 1);
				break ;
			case '[' :
				array_add(loop_stack, &prog->it);
				break ;
			case ']' :
				if (*pc)
					prog->it = *(unsigned int *)array_get(loop_stack, loop_stack->size - 1);
				else
				{
					temp = 0;
					array_set(loop_stack, &temp, loop_stack->size + 1);
				}
				break ;
			default :
				break ;
		}
	}
}

int			main(int argc, char **argv)
{
	int		fd;
	char	*temp;
	t_array	*prog;

	if (argc < 2)
	{
		ft_printf("Please at least one argument.\n");
		return (1);
	}
	init();
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		ft_error();
	prog = array_new(100, 100, sizeof(unsigned char), 0);
	while (ft_get_next(fd, &temp, '\n'))
	{
		while (*temp)
		{
			array_add(prog, temp);
			temp++;
		}
	}
	close(fd);
	run(prog);
	array_free(&prog);
	array_free(&loop_stack);
	return (0);
}

