#include "../include/minishell.h"

/**
 * 	@brief	runs function depending on tokentype
 * 		and makes them fit for the executer
 * 	@param	head: token list
 * 	@return	1: if an parse error occures
 * 		0: if successful
 * 	@NORM	to many lines
 */
static int	check_tokens_via_type(t_token_list *head)
{
	t_token_list	*current;

	current = head;
	while (current != NULL)
	{
		//if (current->type == COMMAND)
		//	current->token = type_command(&current->token);
		if (current->type == ENVAR || current->type == ENVARU)
			current->token = type_envar(&current->token);
		else if (current->type == DQUOTE)
			current->token = type_dquote(&current->token);
		else if (current->type == SQUOTE)
			current->token = type_squote(&current->token);
		else if (current->type == REDIREC)
		{
			if (type_redirec(current->token) != 0)
				return (EXIT_FAILURE);
		}
		else if (current->type == PIPE)
		{
			if (type_pipe(current->token) != 0)
				return (EXIT_FAILURE);
		}
		if (current->token == NULL)
			return (EXIT_FAILURE);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

static int	ft_ft(t_token_list *a)
{
	if (a->type != ENVARU)
		return (0);
	if (a->next != NULL)
		return (0);
	printf("%s... you're turning into a penguin. Stop it.\n", a->token);
	return (1);
}

static int	get_command_types(t_token_list *head)
{
	t_token_list	*current;

	current = head;
	if (current->type == UNKNOWN)
	{
		current->type = COMMAND;
		current->token = type_command(&current->token);
		if (current->token == NULL)
			return (EXIT_FAILURE);
	}
	while (current != NULL)
	{
		if (current->type == PIPE && current->next != NULL)
		{
			// FIXME is it possible at this point that
			// a pipe is valid as last token?
			// if yes, the following code might result in crash
			current->next->type = COMMAND;
			current->next->token = type_command(&current->token);
			if (current->token == NULL)
				return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * 	@brief	checks input for errors and returns a rdy-to-run command string
 * 	@param	tokens: linked list from lexer
 * 	@return	ready-to-run command string
 */
char	*msh_parser(t_token_list *tokens)
{
	char	*command;

	// TODO left of here
	// caution: changed type default from COMMAND to UNKNOWN
	// Idk how to implement command specific parsing
	// without it being to tedious and overly specific
	// I need a better approach and maybe a diffrent parser structure
	if (get_command_types(tokens) != 0)
		return (NULL);
	if (check_tokens_via_type(tokens) != 0)
		return (NULL);
	if (ft_ft(tokens) != 0)
		return (NULL);
	/*
	if (check_commands(tokens) != 0)
		return (NULL);
	if (check_quotes(tokens) != 0)
		return (NULL);
	*/
	// return (ft_parse_error(tokens->token, ": command not found"));

	command = ft_list_to_str(tokens, ',');
	return(command);
}
