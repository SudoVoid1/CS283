1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

   > **Answer**: We dont use execvp direcelty because it will morph the shell process (dsh2) into the thing we are executing. By doing so once that process is over the shell wont continue on. Thus, by using fork we are able to return back to the parent process (the shell) from the child (the thing we are executing).

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

   > **Answer**: In my implementation, if the fork fails I return the error code ERR_EXEC_CMD. This symbolizes that the command was read correctly but wasnt able to be executed. I feel like this is fitting, because if fork() fails, the parent (the shell) needs to continue on like normal.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

   > **Answer**: The execvp() sys call searches for the given command in the PATH environment variable. Or if a direct path is given (starts with a slash) it will use that path directly for the executable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

   > **Answer**: The wait process prevent a "zombie" process by waiting for the child to finish up before continuing the parent (the shell in this case). If we didnt call wait, the shell would continue prompting on and not be able to access the children return code, or clean it up (in the case were the parent is killed before the child is done executing).

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

   > **Answer**: Returns the exit status of the child process. Important because it tells you if the child failed to execute something you will be accessing in the parent like memory, pointers, heaps,etc.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

   > **Answer**: I followed along wiht the demo showed in class were I had a "in_quote_mode" boolean that if true included the spaces in the argument. Without this boolean check, the parser will just skip the spaces in args like normal and provide potentially incorrect outputs.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

   > **Answer**: Not much logic had to be changed for parsing itself, other than the in_quote_mode logic (see above question). The main difficulty was changing how struct holds the input. Instead of having seperate exe and arg places (in the struct) like in the last assignment, I had to have the argv values point to the buffer itself, making sure to null terminate aways the whitespaces to allow for execv to parse the arguments correctly.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

  > **Answer**: Signals can be delivered to a process at any time, interrupting its current execution to handle the signal. While IPC communication requires more data to send that message. Signal are typically used to make simple terminate program process, keyboard interrupts, or errors.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

  > **Answer**: SIGINT is the lest destructive of the three, its default behavior is to terminate the current process, but it's behaviors can be modified or ignored by the process to allow it do cleanup or interupt the process to do something else. SIGKILL and SIGTERM both terminate the given process, but SIGTERM is more commonly used to allow the process to cleanup its mess, while SIGKILL is mostly used as a last resort when a process is unresponsive and needs to be killed without any cleanup or protections.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

  > **Answer**: SIGSTOP sends a signal to the OS to stop the current program. It cannoit be caught or ignored like SIGINT because its behavior cannot be changed by a user program. Thus, it will always carry out the default behavior to stop the program.
