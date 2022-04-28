//
// wyshell.c
// Author: Zachary Crimmel
// Date: Apr 27, 2022
//
// COSC 3750, Homework 9
//
// This is a shell program that was written in c
// Collaborated with Ian Moon on this Homework
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "wyscanner.h"

struct word
{
    struct word *next, *prev;
    char *string;
};

typedef struct word Word;

struct node
{
    struct node *node, *prev;
    char *command;
    Word *arg_list;
    int input, output, error;
    char *in_file, *out_file, *err_file;
};

typedef struct node Node;

int rtn = 0;
char *prtn;
char buff[4096];

void addToList(char *input, Node *list)
{
    Word *tmp;
    tmp = list->arg_list;
    if (tmp == NULL)
    {
        tmp = calloc(1, sizeof(Word));
        tmp->string = strdup(input);
        list->arg_list = tmp;
    }
    else
    {
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = calloc(1, sizeof(Word));
        tmp->next->prev = tmp;
        tmp->next->string = strdup(input);
    }
    list->arg_list = tmp;
}

void free_command(Node *list)
{
    list->command = NULL;
}

void free_words(Node *list)
{
    list->arg_list = NULL;
}

int main()
{
    Node *Head = NULL, *current = NULL;
    Word *commands = NULL;
    while (1)
    {
        printf("$> ");
        prtn = fgets(buff, 4096, stdin);
        if (prtn == NULL)
        {
            return 0;
        }
        rtn = parse_line(buff);
        int prevUse = 0;
        int ambigOutUse = 0;
        int ambigInUse = 0;
        int eol = 0;
        int count = 1;
        int amp = 1;
        char *arguments[100] = {lexeme, NULL};
        // current = calloc(1, sizeof(Node));
        while (rtn != EOL)
        {
            switch (rtn)
            {
            case WORD:
                if (Head == NULL)
                {
                    Head = calloc(1, sizeof(Node));
                    current = Head;
                    // printf("head created");
                }
                if (current->command == NULL || prevUse == 1)
                {
                    prevUse = 0;
                    current->command = strdup(lexeme);
                    // printf("String duplicated");
                }
                else
                {
                    arguments[count] = lexeme;
                    arguments[count + 2] = arguments[count + 1];
                    count++;
                    if (eol == 1)
                    {
                        break;
                    }
                    addToList(lexeme, current);
                    // printf("Command added to list");
                }
                // commands = head;
                break;
            case REDIR_OUT:
                if (ambigOutUse == 1)
                {
                    printf("Ambiguous output redirect\n");
                    eol = 1;
                    break;
                }
                ambigOutUse = 1;
                printf(">\n");
                break;
            case REDIR_IN:
                if (ambigInUse == 1 || ambigOutUse == 1)
                {
                    printf("Ambiguous output redirect\n");
                    eol = 1;
                    break;
                }
                ambigInUse = 1;
                break;
            case PIPE:
                if (fork() == 0)
                {
                    int status_code = execvp(current->command, arguments);
                    if (status_code == -1)
                    {
                        printf("Terminated Incorrectly\n");
                    }
                    if (amp == 0)
                    {
                        return 0;
                    }
                }

                prevUse = 1;
                break;
            case SEMICOLON:
                if (fork() == 0)
                {
                    int status_code = execvp(current->command, arguments);
                    if (status_code == -1)
                    {
                        printf("Terminated Incorrectly\n");
                    }
                    if (amp == 0)
                    {
                        return 0;
                    }
                }

                prevUse = 1;
                break;
            case ERROR_CHAR:
                break;
            case QUOTE_ERROR:
                break;
            case SYSTEM_ERROR:
                return 0;
            case AMP:
                amp = 0;
                break;
            default:
                break;
            }
            rtn = parse_line(NULL);
            if (eol == 1)
            {
                break;
            }
            if(amp == 0){
                wait(NULL);
            }
        }
        // Create a child to run the command in
        if (prevUse == 0)
        {
            if (fork() == 0)
            {
                int status_code = execvp(current->command, arguments);
                if (status_code == -1)
                {
                    printf("Terminated Incorrectly\n");
                }
                if (amp == 1)
                {
                    return 0;
                }
            }
        }
        /*
            First attempt at printing the output
            commands = calloc(1, sizeof(Word));
            commands = current->arg_list;
            while(commands != NULL){
                commands = commands->prev;
            }
            while (commands != NULL)
            {
                printf("%s\n", commands->string);
                commands = commands->next;
            }
            */
        current = Head;
        Node *tmp;
        free_command(current);
        free_words(current);
        tmp = current;
        free_command(tmp);
    }
}
