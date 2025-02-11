#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int fill_command(char *cmd, command_t *command)
{
    command->args[0] = '\0'; // args start as being empty (incase there is none)

    // Skip spaces in the front of cmd
    while (isspace(*cmd))
    {
        cmd++;
    }

    char *exe_start = cmd;
    char *exe_end = cmd;

    // Find the end of the exe portion
    while (*exe_end != '\0' && !isspace(*exe_end))
    {
        exe_end++;
    }

    size_t exe_len = exe_end - exe_start;
    if (exe_len > EXE_MAX)
    {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    strncpy(command->exe, exe_start, exe_len);
    command->exe[exe_len] = '\0';

    // Skip spaces after exe until you get to the args
    cmd = exe_end;
    while (*cmd && isspace(*cmd))
    {
        cmd++;
    }

    char *arg_start = cmd;
    size_t args_combined_length = 0;
    while (*arg_start != '\0')
    {
        size_t arg_len = strcspn(arg_start, " ");
        // Have to keep checking if a new arg will put it over the limit
        if (args_combined_length + arg_len > ARG_MAX)
        {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strncat(command->args, arg_start, arg_len);
        args_combined_length += arg_len;

        // Skip pointer to end of new arg
        arg_start += arg_len;
        // Place a space in between args
        if (*arg_start != '\0')
        {
            strcat(command->args, " ");
            args_combined_length++;
            arg_start++;
        }
    }
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    // Clear out clist and reset count
    memset(clist, 0, sizeof(command_list_t));
    int count = 1;
    int rc = 0;

    char *cmd_start = cmd_line;
    char *cmd_end;
    while (*cmd_start != '\0')
    {
        if (count > CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }

        char *pipe_loc = strchr(cmd_start, PIPE_CHAR);
        // If a pipe exist then take just that section
        if (pipe_loc != NULL)
        {
            cmd_end = pipe_loc;
        }
        else
        {
            // If it doesnt exist we are at the last command segment (or there are none)
            cmd_end = cmd_start + strlen(cmd_start);
        }

        // Temp str setup for cmd section
        size_t cmd_len = cmd_end - cmd_start;
        char temp[SH_CMD_MAX];
        strncpy(temp, cmd_start, cmd_len);
        temp[cmd_len] = '\0';

        if ((rc = fill_command(temp, &clist->commands[count])) != 0)
        {
            return rc;
        }

        count++;
        cmd_start = cmd_end;
        if (pipe_loc)
        {
            // Skip the pip char
            cmd_start++;
            // Skip excess spaces
            while (isspace(*cmd_start))
            {
                cmd_start++;
            }
        }
    }

    clist->num = count;
    return OK;
}