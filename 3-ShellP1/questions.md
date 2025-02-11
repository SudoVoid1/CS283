1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

   > **Answer**: It allows for variablity in the length of the input, it handles buffer overfull (i.e will error if user input is to big),it terminates the string for us, and keeps the spaces/pipes/etc all in the input. All of these factor, allow for both flexibility in length and precise inputs, which is exact what we want for a shell.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

   > **Answer**: On runtime, each command can have a different length of input. So making the buffer a fixed-sized array would be stupid becuase exceeding it would cause a overflow/error, or we would end up allocate a excess amount of memory.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

   > **Answer**: Commands executed by the shell have specificy paths to there executables, meaning that without trimming spaces, the shell will try and find a executable like " exe " instead of "exe" and it will either fail or execute the wrong thing.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

  > **Answer**:

  - > (Ouput redirection): Redirect stdout to a file, overriding the contents. Challenges would be file permission, creating the file (if it doesnt exist) and making sure something else isnt already writing/reading the file.
  - < (input redirection): Redirects stdin from a file.  
    Challenges would be similar to the above operation but with the added layer of making sure the file you are reading from exist.
  - > > (append redirection): Appends stdout to a file, preserving the content of the file.
    > > Challenges would again be similar to the above two with the added difficulty of moving the file pointer to the end of the file before writing, and the additional issue in the case where the file doesnt exist.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

  > **Answer**: Piping is more about connecting I/O between process, while redirection is more changing where a specific input/output goes (files/stdout/stderror/stdin). In other words, piping is alot more versatile then redirection since it allows you to take the stdout of one command and directly pass it to the stdin of another program, while with redirection you could technically achieve something similar but it would require more steps (like saving to a temp file etc.).

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

  > **Answer**: It's good to keep these outputs seperate as for a general user, they might not care about what is happening the background and having them combined might make outputs confusing when tracking progress of a program. Additionally, this allows people who do care about the error messages to have a specific avenue to look at what went wrong in a program exactly without the clutter of a programs output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

  > **Answer**: The shell will need to keep track of program return codes, and if not zero (indicating an error) redirect the stderr to stdout. It would also need to keep track of the streams to stdout and stderr in a file, so that they are output in the order of occurance when redirected together. Additionally, we problably want a option to hide the stderr unless specifically requested/on program error so we would need to save both streams seperately as well. For example, we could have the files
  > combined_stream, stdout_stream, sterror_stream. Allowing for the user to designate what they want to see/find and preserving the order in which things happen in a program.
