1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation has a array of the child pid loops through the number of commands calling waitpid() on each pid (child) to ensure that the process is dead (executed). If you forgot to call waitpid() on the child processes, you would be left with zombie process, in which the parent cant tell if it completed or not. Also the piping wouldnt work right as the output of one child is the input for other, thus, if the next child doesnt know if the previous finished it work right.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

Since dup2() copies the source file descripter (fd) onto the target, if you don't close the original pipes you could theorically run out of available fd. As you now have descriptor leak in which you cant resuse that fd.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

If cd were implemented as an external command, it wouldnt work as intended. It would only change the current working directory (cwd) of the child process that executes it, not the parent as a whole. This means that when the child dies it will return to the original cwd, as the parent has no knowledge of the cd command.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

You could dynamically realloc more memory for more commands in the list. You could use the conventional way of doubling the size every time you reach a arbitrary capacity. This would require you to keep track of size of the command list and dealloc/alloc memory sparingly. However, one trade off for handling arbitrary piped commands is that you would have to limit the amount of processes that can run at a time, which could make the program slower for large amounts of piped commands.
