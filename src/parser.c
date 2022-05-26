#include "../include/minishell.h"

static int	check_tokens_via_type2(t_token_list *cur)
{
	if (cur->type == HEREDOC)
	{
		if (!cur->next)
		{
			ft_parse_error("Please enter a delimiter after: ", cur->token);
			return (EXIT_FAILURE);
		}
		cur->next->token = type_heredoc(&cur->next->token, cur->next->token);
	}
	if (cur->type == REDIREC)
		if (type_redirec(cur->token) != 0)
			return (EXIT_FAILURE);
	if (cur->type == PIPE)
		if (type_pipe(cur->token) != 0)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * 	@brief	runs function depending on tokentype
 * 			and makes them fit for the executer
 * 	@param	head: token list
 * 	@return	1: if an parse error occures
 * 			0: if successful
 */
static int	check_tokens_via_type(t_data *data)
{
	t_token_list	*current;

	current = data->tokens;
	while (current != NULL)
	{
		if (current->type == ENVAR)
			current->token = insert_envar(&current->token, data);
		else if (current->type == DQUOTE)
			current->token = type_dquote(&current->token, data);
		else if (current->type == SQUOTE)
			current->token = type_squote(&current->token);
		if (check_tokens_via_type2(current) != 0)
			return (EXIT_FAILURE);
		if (current->token == NULL)
			return (EXIT_FAILURE);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * 	@brief	the very first token and every token after a pipe gets
 * 			converted to COMMAND unless it's a BUILTIN
 * 	@param	head: head of token list
 * 	@return Success or failure
 */
static int	get_command_types(t_data *data)
{
	t_token_list	*current;

	current = data->tokens;
	if (current->type != BUILTIN)
	{
		current->type = COMMAND;
		current->token = type_command(&current->token, data);
		if (current->token == NULL)
			return (EXIT_FAILURE);
	}
	while (current != NULL)
	{
		if (current->type == PIPE)
		{
			if (current->next == NULL || current->next->type == PIPE)
				return ((int)ft_parse_error("invalid pipe syntax", NULL) + 1);
			if (current->next->type != BUILTIN)
			{
				current->next->type = COMMAND;
				current->next->token = type_command(&current->next->token, data);
			}
			if (current->next->token == NULL)
				return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

// static char	**ft_list_to_array(t_token_list *head)
// {
// 	t_token_list	*tmp;
// 	char			**cmd_line;
// 	int				i;

// 	tmp = head;
// 	i = 0;
// 	while (tmp)
// 	{
// 		tmp = tmp->next;
// 		i++;
// 	}
// 	cmd_line = malloc(sizeof(char *) * i + 1);
// 	tmp = head;
// 	i = 0;
// 	while (tmp)
// 	{
// 		cmd_line[i] = ft_strdup(tmp->token);
// 		tmp = tmp->next;
// 		i++;
// 	}
// 	cmd_line[i] = NULL;
// 	return (cmd_line);
// }

/**
 * 	@brief	checks input for errors and returns a rdy-to-run command string
 * 	@param	head: linked list from lexer
 * 	@return	ready-to-run command string
 */
char	*msh_parser(t_data *data)
{
	char	*command;
	// char	**cmd_line;

	if (check_tokens_via_type(data) != 0)
		return (NULL);
	if (get_command_types(data) != 0)
		return (NULL);
	if (check_redirections(data->tokens) != 0)
		return (NULL);
	command = ft_list_to_str(data->tokens, ',');
	// cmd_line = ft_list_to_array(data->tokens);
	return (command);
}
