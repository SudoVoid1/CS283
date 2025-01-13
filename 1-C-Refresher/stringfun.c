#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

// prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);

// prototypes for functions to handle required functionality
int count_words(char *, int, int);
int reverse_words(char *, int, int);
int print_words(char *, int, int);
int replace_words(char *, int, int, char *, char *);
// add additional prototypes here

int setup_buff(char *buff, char *user_str, int len)
{
    char *buffPtr = buff;
    int user_str_len = 0;
    int prevSpace = 0;

    while (*user_str != '\0' && user_str_len < len)
    {
        if (*user_str == '\t' || *user_str == ' ')
        {
            if (prevSpace == 0) // Only add a space if previous character wasn't space/tab
            {
                *buffPtr = ' ';
                buffPtr++;
                user_str_len++;
                prevSpace = 1;
            }
        }
        else
        {
            *buffPtr = *user_str;
            buffPtr++;
            user_str_len++;
            prevSpace = 0;
        }
        user_str++;
    }

    // if the user_str_len becomes greater than len the loop ends but the user_str may not be at \0.
    // If that is the case then the str provided is to long for the buff, thus returning -1.
    if (*user_str != '\0')
    {
        printf("Error: The user supplied string is too large");
        return -1;
    }

    int numberOfDots = len - user_str_len;
    // add dots if needed
    while (numberOfDots > 0)
    {
        *buffPtr = '.';
        buffPtr++;
        numberOfDots--;
    }
    return user_str_len;
}

void print_buff(char *buff, int len)
{
    printf("Buffer:  ");
    for (int i = 0; i < len; i++)
    {
        putchar(*(buff + i));
    }
    putchar('\n');
}

void usage(char *exename)
{
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int buff_len, int str_len)
{
    if (str_len > buff_len)
    {
        return -1;
    }
    int wordCounter = 0;
    char *buffPtr = buff;
    int insideWord = 0;
    for (int i = 0; i < str_len; i++)
    {
        // Need to check if string is full of all spaces/tabs as they are not a word
        if (*buffPtr != ' ' && *buffPtr != '\t')
        {
            if (insideWord == 0)
            {
                wordCounter++;
                insideWord = 1;
            }
        }
        else
        {
            insideWord = 0;
        }
        buffPtr++;
    }

    return wordCounter;
}
// ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
int reverse_words(char *buff, int buff_len, int user_str_len)
{
    if (user_str_len > buff_len)
    {
        return -1;
    }
    if (user_str_len == 1)
    {
        if (*buff == ' ')
        {
            return -1;
        }
        // 1 char words are already reversed
        else
        {
            return 0;
        }
    }
    else
    {
        char *start = buff;
        char *end = buff + user_str_len - 1;
        while (start < end)
        {
            char temp = *start;
            *start = *end;
            *end = temp;
            start++;
            end--;
        }
        return 0;
    }
}
int print_words(char *buff, int buff_len, int str_len)
{
    if (str_len > buff_len)
    {
        return -1;
    }
    else if (str_len == 0)
    {
        return 0; // No words
    }
    else
    {
        char *buffPtr = buff;
        char word[256];
        char *wordPtr = word;
        int wordCounter = 0;
        int charCounter = 0;

        printf("Word Print\n");
        printf("----------\n");

        while (buffPtr < buff + str_len)
        {
            if (*buffPtr != ' ' && *buffPtr != '\t')
            {
                *wordPtr = *buffPtr;
                wordPtr++;
                charCounter++;
            }
            else if (charCounter > 0)
            {
                *wordPtr = '\0';
                wordCounter++;
                printf("%d. %s (%d)\n", wordCounter, word, charCounter);
                // Reset after a word ends
                wordPtr = word;
                charCounter = 0;
            }
            buffPtr++;
        }
        // handle cases at the end of the buffer
        if (charCounter > 0)
        {
            *wordPtr = '\0';
            wordCounter++;
            printf("%d. %s (%d)\n", wordCounter, word, charCounter);
        }

        return 0;
    }
}
int replace_words(char *buff, int buff_len, int str_len, char *oldword, char *newword)
{
    printf("Not Implemented\n");
    return -1;
}
int main(int argc, char *argv[])
{

    char *buff;         // placehoder for the internal buffer
    char *input_string; // holds the string provided by the user on cmd line
    char opt;           // used to capture user option from cmd line
    int rc;             // used for return codes
    int user_str_len;   // length of user supplied string

    // TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?

    // This is safe because C shortcuts or statements, meaning that if the first condition
    //  argc < 2  is true it doesnt evaluate the second part of the or since (T v anything) is true.
    // Therefore, to get to the second condition there must be more than 2 arguements passed and thus argv[1] must exist.

    if ((argc < 2) || (*argv[1] != '-'))
    {
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1] + 1); // get the option flag

    // handle the help flag and then exit normally
    if (opt == 'h')
    {
        usage(argv[0]);
        exit(0);
    }

    // WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    // TODO:  #2 Document the purpose of the if statement below

    // Since the string we are storing is the second arguement, if the number of arguements is less than 3
    // the user hasnt provided a string and the default help message is shown. (first arguement is 0, program options are 1, and provided string is 2 -> equaling a argc of 3)

    if (argc < 3)
    {
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; // capture the user input string

    // TODO:  #3 Allocate space for the buffer using malloc and
    //           handle error if malloc fails by exiting with a
    //           return code of 99
    //  CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if (buff == NULL)
    {
        printf("Failed to allocate memory\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ); // see todos
    if (user_str_len < 0)
    {
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(3);
    }

    switch (opt)
    {
    case 'c':
        rc = count_words(buff, BUFFER_SZ, user_str_len); // you need to implement
        if (rc < 0)
        {
            printf("Error counting words, rc = %d\n", rc);
            exit(2);
        }
        printf("Word Count: %d\n", rc);
        break;

        // TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //        the case statement options

    case 'r':
        rc = reverse_words(buff, BUFFER_SZ, user_str_len);
        if (rc < 0)
        {
            printf("Error reversing words, rc = %d\n", rc);
            exit(3);
        }
        break;
    case 'w':
        rc = print_words(buff, BUFFER_SZ, user_str_len);
        if (rc < 0)
        {
            printf("Error printing words, rc = %d", rc);
            exit(3);
        }
        break;
    case 'x':
        if (!(argc == 5))
        {
            printf("Error replacing words, not enough arguements given\n");
            exit(1);
        }
        else
        {
            rc = replace_words(buff, BUFFER_SZ, user_str_len, argv[3], argv[4]);
            if (rc < 0)
            {
                printf("Error replacing words, rc = %d\n", rc);
                exit(3);
            }
        }
        break;

    default:
        usage(argv[0]);
        exit(1);
    }

    // TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff, BUFFER_SZ);
    free(buff);
    exit(0);
}

// TODO:  #7  Notice all of the helper functions provided in the
//           starter take both the buffer as well as the length.  Why
//           do you think providing both the pointer and the length
//           is a good practice, after all we know from main() that
//           the buff variable will have exactly 50 bytes?

// Well the buffer length is a defined size, but if you ever wanted to change it, it could break functionality in these helper functions or require the user to change hardcoded things multiple times.
// In this scenerio, you would only need to change the defination at the top and it would be passed down to all functions.
// Additionally, by being able to check the size of the buffer against user input (in each helper function) it ensures that there wont be a buffer overflow in which
// unintented memory could be accessed and cause other issues.