/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlancar <dlancar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/04 14:13:26 by dlancar           #+#    #+#             */
/*   Updated: 2014/02/08 17:08:33 by dlancar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <libft.h>

#define MEM_SIZE 30000

static unsigned char		memory[MEM_SIZE] = {0};
static t_array				*loop_stack;
static unsigned int			pc = 0;

static void	init(void);
static void	run(t_array *prog);

static void	init(void)
{
	loop_stack = array_new(10, 10, sizeof(unsigned int), TRUE);
	ft_bzero(memory, MEM_SIZE);
}

static void	skip_block(t_array *prog)
{
	unsigned int	size;
	int				temp = 0;

	size = loop_stack->size;
	while (prog->it < prog->size && loop_stack->size >= size)
	{
		if (*(unsigned char *)array_get(prog, prog->it) == '[')
			array_add(loop_stack, &prog->it);
		else if (*(unsigned char *)array_get(prog, prog->it) == ']')
		{
			array_set(loop_stack, &temp, loop_stack->size - 1);
			loop_stack->size--;
		}
		prog->it++;
	}
}

/*
** VM spec : Circular memory of MEM_SIZE (30 000) unsigned char.
*/
static void	run(t_array	*prog)
{
	unsigned char	temp;

	pc = 0;
	while (prog->it < prog->size)
	{
		temp = *(unsigned char *)array_next(prog);
		switch (temp)
		{
			case '>' :
				pc = (pc + 1 < MEM_SIZE) ? pc + 1 : 0;
				break ;
			case '<' :
				pc = (pc) ? pc - 1 : MEM_SIZE;
				break ;
			case '+' :
				memory[pc]++;
				break ;
			case '-' :
				memory[pc]--;
				break ;
			case '.' :
				write(STDOUT, &memory[pc], 1);
				break ;
			case ',' :
				read(STDIN, &memory[pc], 1);
				break ;
			case '[' :
				if (!loop_stack->size)
					array_add(loop_stack, &prog->it);
				else if (pc != *(t_uint *)array_get(loop_stack, loop_stack->size - 1))
					array_add(loop_stack, &prog->it);
				if (!memory[pc])
					skip_block(prog);
				break ;
			case ']' :
				prog->it = *(unsigned int *)array_get(loop_stack, loop_stack->size - 1) - 1;
				loop_stack->size--;
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
	prog = array_new(100, 100, sizeof(unsigned char), TRUE);
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

