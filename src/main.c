#include "../include/minishell.h"

/*	print username + current directory
 *	to be used as prompt
 */
static char	*msh_dir(void)
{
	char	cwd[1024];
	char	**split_dir;
	char	*username;
	int	count;
	char	*promptline;

	getcwd(cwd, sizeof(cwd));
	split_dir = ft_split(cwd, '/');
	count = 0;
	while (split_dir[count])
		count++;
	count--;
	username = getenv("USER");
	// FIXME really messy solution, might result in leaks
	username = ft_strjoin(username, " ");
	promptline = ft_strjoin(username, split_dir[count]);
	promptline = ft_strjoin(promptline, " % ");
	while (count >= 0)
	{
		free(split_dir[count]);
		count--;
	}
	free(split_dir);
	return (promptline);
}

/*	Basic loop of a shell
 *	1. Read the command from stdin
 *	2. Separate the command string into a programm and arguments
 *	3. Run the parsed command
 */
static void	msh_loop(void)
{
	// TODO we'll probably need a data struct to hold all the data
	t_token_list	*tokens;
	char		*line;
	char		*command;
	int		status;
	char		*promptline;

	status = 1;
	while (status)
	{
		promptline = msh_dir();
		line = readline(promptline);
		free(promptline);
		if (ft_strlen(line))
		{
			add_history(line);
			tokens = msh_lexer(line);
			ft_print_list(tokens);
		}
		// else //to check for leaks
		// 	status = 0;

		// TODO lexer, parser, executer
		//tokens = msh_lexer(line);
		//command = msh_parser(tokens);
		//status = msh_execute(command);
		free(line);
		//free(tokens);
		//free(command);
	}
}

/*	1. Loading config files (if any)
 *	2. Command loop
 *	3. Shutdown/Cleanup
 */
int	main(int argc, char *argv[])
{
	if (argc != 1)
		ft_error(strerror(E2BIG));
	init_signal_handling();
	msh_loop();
	// while (1) //to check for leaks
	// 	pause();
	return (EXIT_SUCCESS);
}
