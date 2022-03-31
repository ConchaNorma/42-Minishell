/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarnell <aarnell@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 18:23:57 by aarnell           #+#    #+#             */
/*   Updated: 2022/03/30 21:24:40 by aarnell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	put_err(t_exec *vars, char *str1, char *str2)
{
	if (vars->exit_status != 258)
		str1 = ft_strjoin("minishell: ", vars->tm_cmd->cmd[0]);
	else
		str1 = ft_strjoin("minishell: syntax error near unexpected token ", \
				str2);
	if (vars->exit_status == 1)
	{
		str2 = ft_strjoin(str1, ": ");
		free(str1);
		str1 = str2;
		str2 = ft_strjoin(str1, vars->tm_cmd->cmd[1]);
		free(str1);
		str1 = str2;
	}
	if (vars->exit_status == 127)
	{
		str2 = ft_strjoin(str1, ": command not found");
		free(str1);
		str1 = str2;
	}
	if (vars->exit_status == 127 || vars->exit_status == 258)
		ft_putendl_fd(str1, 2);
	else
		perror(str1);
	free(str1);
}

int	ft_errfrex(t_exec *vars, t_err tp, int ex_st, char *err)
{
	vars->exit_status = ex_st;
	if (tp == ER || tp == ERFR || tp == ERFREX)
	{
		if (vars->exit_status != 127 && vars->exit_status != 258)
			vars->exit_status = 1;
		put_err(vars, NULL, err);
	}
	if (tp == FR || tp == ERFR)
		clean_base_struct(vars, 0);
	if (tp == ERFREX || tp == FREX)
		clean_base_struct(vars, 1);
	if (tp == ERFREX || tp == FREX)
		exit(vars->exit_status);
	return (1);
}

static void	clean_lstrdr(t_cmd *c_cmd)
{
	t_redir	*c_rdr;
	t_redir	*t_rdr;

	c_rdr = c_cmd->v_rdr;
	while (c_rdr)
	{
		t_rdr = c_rdr->next;
		if (c_rdr->file)
			free(c_rdr->file);
		free(c_rdr);
		c_rdr = t_rdr;
	}
}

static void	clean_lstcmd(t_exec *vars)
{
	t_cmd	*c_cmd;
	t_cmd	*t_cmd;

	if (vars->cmds)
	{
		c_cmd = vars->cmds;
		while (c_cmd)
		{
			t_cmd = c_cmd->next;
			if (c_cmd->v_rdr)
				clean_lstrdr(c_cmd);
			if (c_cmd->cmd)
				ft_frmtrx(c_cmd->cmd);
			free(c_cmd);
			c_cmd = t_cmd;
		}
	}
}

void	clean_base_struct(t_exec *vars, int ext)
{
	if (vars->str)
		free(vars->str);
	vars->str = NULL;
	if (vars->path)
		free(vars->path);
	vars->path = NULL;
	if (vars->cmds)
		clean_lstcmd(vars);
	vars->cmds = NULL;
	vars->pid = -1;
	if (vars->pids)
		free(vars->pids);
	vars->pids = NULL;
	vars->ofd[0] = -1;
	vars->ofd[1] = -1;
	vars->pfd[0] = -1;
	vars->pfd[1] = -1;
	vars->tfd[0] = -1;
	vars->tfd[1] = -1;
	vars->st = 1;
	if (ext)
		ft_frmtrx(vars->envp);
}
