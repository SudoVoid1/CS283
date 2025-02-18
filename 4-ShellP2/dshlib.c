#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

int exec_cmd(cmd_buff_t *cmd)
{
    return -1;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    return BI_EXECUTED;
}

Built_In_Cmds match_command(const char *input)
{
    if (strcmp(EXIT_CMD, input) == 0)
    {
        return BI_CMD_EXIT;
    }
    else if (strcmp("dragon", input) == 0)
    {
        return BI_CMD_DRAGON;
    }
    else if (strcmp("cd", input) == 0)
    {
        return BI_CMD_CD;
    }
    else
    {
        return BI_NOT_BI;
    }
}

int free_cmd_buff(cmd_buff_t *cmd_buff)
{
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    return OK;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL)
    {
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;

    for (int i = 0; i < CMD_ARGV_MAX; i++)
    {
        // Initilize argv as NULL, as they will later point to buffer or signify the end of the args
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    int rc = OK;
    if (alloc_cmd_buff(cmd_buff) == ERR_MEMORY)
    {
        return ERR_MEMORY;
    }
    // First test if user input was empty.
    if (cmd_line == NULL || *cmd_line == '\0')
    {
        return WARN_NO_CMDS;
    }
    // else if (strcmp(cmd_line, EXIT_CMD) == 0)
    // {
    //     cmd_buff->argc = 1;
    //     strcpy(cmd_buff->argv[0], EXIT_CMD);
    // }
    // else if (strcmp(cmd_line, "dragon") == 0)
    // {
    //     cmd_buff->argc = 1;
    //     strcpy(cmd_buff->argv[0], "dragon");
    // }
    else
    {
        // Copy the raw string into the struct.
        strcpy(cmd_buff->_cmd_buffer, cmd_line);
        cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0';

        // Putting the exe in argv[0]
        char *ptr = cmd_buff->_cmd_buffer;
        while (isspace(*ptr))
        {
            ptr++;
        }

        char *exe_start = ptr;
        char *exe_end = ptr;

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
        *exe_end = '\0';
        cmd_buff->argv[0] = exe_start;
        cmd_buff->argc = 1;

        // Do all the args
        ptr = exe_end + 1;
        size_t args_combined_length = 0;
        while (*ptr != '\0' && cmd_buff->argc < CMD_ARGV_MAX - 1) // last arg need to be NULL for execv
        {
            // Skip spaces
            while (*ptr && isspace(*ptr))
            {
                ptr++;
            }

            if (*ptr == '\0') // end of buffer
            {
                break;
            }

            char *arg_start = ptr;

            // Find end of the argument
            while (*ptr != '\0' && !isspace(*ptr))
            {
                ptr++;
            }

            size_t arg_len = ptr - arg_start;

            // Making sure that combined a arglength + the new arg length doesnt excede max
            if (args_combined_length + arg_len > ARG_MAX)
            {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            args_combined_length += arg_len;

            *ptr = '\0';
            cmd_buff->argv[cmd_buff->argc++] = arg_start;

            ptr++;
        }
        // Setting last arg to be NULL, shouldnt be neccesary as they are already null
        // cmd_buff->argv[cmd_buff->argc] = NULL;
    }
    return rc;
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX);
    int rc = 0;
    cmd_buff_t cmd;

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // parse input and put it in cmd_buff_t struct
        rc = build_cmd_buff(cmd_buff, &cmd);

        if (rc == ERR_MEMORY)
        {
            return ERR_MEMORY;
        }
        else if (rc == WARN_NO_CMDS)
        {
            printf(CMD_WARN_NO_CMD);
            return WARN_NO_CMDS;
        }
        else if (rc == ERR_TOO_MANY_COMMANDS)
        {
            // NOT applicable for this assignment
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }
        else if (rc == ERR_CMD_OR_ARGS_TOO_BIG)
        {
            clear_cmd_buff(&cmd);
        }
        else
        {
            rc = match_command(cmd.argv[0]);

            // if existing command (exit,cd, dragon)
            // else fork/exec external command

            if (rc == BI_CMD_EXIT)
            {
                exit(OK_EXIT);
            }
            else if (rc == BI_CMD_DRAGON)
            {
                printf("RARW");
                // print_dragon();
            }
            else if (rc == BI_CMD_CD)
            {
                printf("CD");
                exec_built_in_cmd(&cmd);
            }
            else if (rc == BI_NOT_BI)
            {
                exec_cmd(&cmd);
            }

            // Since the command has been exec we can clear the cmd_buff now!
            clear_cmd_buff(&cmd);
        }
    }
    free_cmd_buff(&cmd);
    return OK;
}
// TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

// TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
// the cd command should chdir to the provided directory; if no directory is provided, do nothing

// TODO IMPLEMENT if not built-in command, fork/exec as an external command
// for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
