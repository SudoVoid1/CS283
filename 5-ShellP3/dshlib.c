#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
extern void print_dragon();

int exec_cmd(cmd_buff_t *cmd)
{
    if (execvp(cmd->argv[0], cmd->argv) < 0)
    {
        return ERR_EXEC_CMD;
    }
    return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    // If there is no args for cd
    if (cmd->argv[1] == NULL)
    {
        return BI_EXECUTED;
    }
    else
    {
        if (chdir(cmd->argv[1]) == -1)
        {
            return -1;
        }
    }
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
    else if (strcmp("cd", input) == 0 || strcmp("CD", input) == 0)
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
    if (cmd_buff->_cmd_buffer)
    {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}

int free_cmd_list(command_list_t *cmd_lst)
{
    if (cmd_lst == NULL)
        return OK;
    for (int i = 0; i < cmd_lst->num; i++)
    {
        free_cmd_buff(&cmd_lst->commands[i]);
    }

    cmd_lst->num = 0;

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argc = 0;

    for (int i = 0; i < CMD_ARGV_MAX; i++)
    {
        // Initilize argv as NULL, as they will later point to buffer or signify the end of the args
        cmd_buff->argv[i] = NULL;
    }

    return OK;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{

    if (cmd_buff->_cmd_buffer == NULL)
    {
        cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
        if (cmd_buff->_cmd_buffer == NULL)
        {
            return ERR_MEMORY;
        }
    }
    // If it already is allocated but is empty just clear it
    clear_cmd_buff(cmd_buff);
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    int rc = OK;
    if (alloc_cmd_buff(cmd_buff) == ERR_MEMORY)
    {
        return ERR_MEMORY;
    }

    // Sanity check the input isnt empty
    if (cmd_line == NULL || *cmd_line == '\0')
    {
        return WARN_NO_CMDS;
    }
    else
    {
        // Copy input into buff
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

        // Vars to handle quotes
        bool in_quote_mode = false;
        char *arg_start = NULL;

        while (*ptr != '\0' && cmd_buff->argc < CMD_ARGV_MAX - 1) // last arg need to be NULL for execv
        {
            // Skip spaces
            while (*ptr && isspace(*ptr) && !in_quote_mode)
            {
                ptr++;
            }

            if (*ptr == '\0') // end of buffer
            {
                break;
            }
            if (*ptr == '"')
            {
                in_quote_mode = !in_quote_mode;
                ptr++;
                arg_start = ptr;
            }
            else
            {
                arg_start = ptr;
            }
            // Find end of the argument
            while (*ptr != '\0' && (in_quote_mode || !isspace(*ptr)))
            {
                // if its a quote it and in quote mode, we have found the end of the arg
                //  switch mode and null terminate that part like normal
                if (*ptr == '"' && in_quote_mode)
                {
                    in_quote_mode = false;
                    *ptr = '\0';
                    ptr++;
                    break;
                }
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
    }
    return rc;
}

int build_cmd_list(char *cmd_line, command_list_t *cmdlist)
{
    int rc = OK;
    if (cmdlist == NULL)
    {
        return ERR_MEMORY;
    }
    cmdlist->num = 0;

    char *saveptr;
    char *cmd_part = strtok_r(cmd_line, "|", &saveptr);

    while (cmd_part != NULL)
    {
        if (cmdlist->num > CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }

        cmd_buff_t *cmd_buff = &cmdlist->commands[cmdlist->num];
        rc = build_cmd_buff(cmd_part, cmd_buff);
        if (rc != OK)
        {
            return rc;
        }

        cmdlist->num++;
        cmd_part = strtok_r(NULL, "|", &saveptr);
    }

    return rc;
}

int execute_pipeline(command_list_t *clist)
{
    // Sanity Check (should have been caught already)
    if (clist == NULL || clist->num == 0)
    {
        return WARN_NO_CMDS;
    }

    int num_commands = clist->num;
    int pipes[num_commands - 1][2]; // Array of pipes
    pid_t pids[num_commands];       // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < num_commands - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Iterate through commands
    for (int i = 0; i < num_commands; i++)
    {
        cmd_buff_t *cmd = &clist->commands[i];
        // Match built-in commands
        int rc = match_command(cmd->argv[0]);

        if (rc == BI_CMD_EXIT)
        {
            free_cmd_list(clist);
            exit(OK_EXIT);
        }
        else if (rc == BI_CMD_DRAGON)
        {
            // Allow the dragon to be forked, but not exec (since its a built-in function)
            pids[i] = fork();
            if (pids[i] == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pids[i] == 0)
            { // Child process for dragon command
                // Set up output pipe for all except last process
                if (i < num_commands - 1)
                {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }

                // Close all pipes in child
                for (int j = 0; j < num_commands - 1; j++)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                print_dragon();
                exit(OK);
            }

            continue;
        }
        else if (rc == BI_CMD_CD)
        {
            if (exec_built_in_cmd(cmd) == -1)
            {
                return ERR_CMD_ARGS_BAD;
            }
            continue;
        }
        else if (rc == BI_NOT_BI)
        {
            pids[i] = fork();

            if (pids[i] == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pids[i] == 0)
            { // Child process
              // Set up input pipe for all except first process
                if (i > 0)
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }

                // Set up output pipe for all except last process
                if (i < num_commands - 1)
                {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }

                // Close all pipes in child
                for (int j = 0; j < num_commands - 1; j++)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // Execute external command
                int exec_result = exec_cmd(cmd);
                if (exec_result != OK)
                {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < num_commands - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes
    for (int i = 0; i < num_commands; i++)
    {
        if (match_command(clist->commands[i].argv[0]) == BI_NOT_BI)
        {
            waitpid(pids[i], NULL, 0);
        }
    }

    return OK;
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
    // cmd_buff_t cmd;
    command_list_t *cmd_list = malloc(sizeof(command_list_t));

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
        rc = build_cmd_list(cmd_buff, cmd_list);
        // rc = build_cmd_buff(cmd_buff, &cmd);

        if (rc == ERR_MEMORY)
        {
            return ERR_MEMORY;
        }
        else if (rc == WARN_NO_CMDS)
        {
            printf(CMD_WARN_NO_CMD);
            free_cmd_list(cmd_list);
            return WARN_NO_CMDS;
        }
        else if (rc == ERR_TOO_MANY_COMMANDS)
        {

            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }
        else if (rc == ERR_CMD_OR_ARGS_TOO_BIG)
        {
            break;
        }
        else
        {
            rc = execute_pipeline(cmd_list);
            // rc = match_command(cmd.argv[0]);
        }
    }
    free_cmd_list(cmd_list);
    return OK;
}
