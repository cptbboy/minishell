#include "../include/minishell.h"

static int	has_pipe(char *cmd_line)
{
	int	i;

	if (!cmd_line)
		return (0);
	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '|' && cmd_line[i + 1])
			return (1);
		i++;
	}
	return (0);
}

static char	***get_cmds(char *cmd_line)
{
	char	**cmd_array;
	char	***cmds;
	int		i;
	int		j;

	cmd_array = ft_split(cmd_line, '|');
	if (!cmd_array)
		ft_error(strerror(errno));
	i = 0;
	while (cmd_array[i])
		i++;
	cmds = malloc(sizeof(char **) * i + 1);
	if (!cmds)
		ft_error(strerror(errno));
	j = 0;
	while (j < i)
	{
		cmds[j] = ft_split(cmd_array[j], ',');
		j++;
	}
	cmds[j] = NULL;
	free_2d_array(cmd_array);
	i = 0;
	return (cmds);
}

static void	pipe_exec_helper(char ***cmds, t_data *data, int *myfd, int *fd, int i)
{
	if (cmds[i + 1])
	{
		dup2(*myfd, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		data->status = msh_executer(data, cmds[i]);
	}
	else
	{
		dup2(*myfd, STDIN_FILENO);
		close(fd[0]);
		data->status = msh_executer(data, cmds[i]);
	}
	exit(1);
}

int	pipe_exec(t_data *data)
{
	char	***cmds;
	int		i;
	int		fd[2];
	int		ret;
	int		myfd;

	if (has_pipe(data->command))
	{
		cmds = get_cmds(data->command);
		i = 0;
		myfd = 0;
		while (cmds[i])
		{
			if (pipe(fd) == -1)
				ft_error(strerror(errno));
			data->pid = fork();
			if (data->pid == -1)
				ft_error(strerror(errno));
			if (data->pid == 0)
			{
				pipe_exec_helper(cmds, data, &myfd, fd, i);
				ft_error(strerror(errno));
			}
			else
			{
				waitpid(data->pid, NULL, 0);
				close(fd[1]);
				myfd = fd[0];
			}
			i++;
		}
		free_3d_array(cmds);
		ret = data->status;
	}
	else
		ret = msh_executer(data, NULL);
	return (ret);
}
