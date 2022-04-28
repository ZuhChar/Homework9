//
// wyshell.c
// Author: Zachary Crimmel
// Date: Apr 20, 2022
//
// COSC 3750, Homework 8
//
// This is a shell program that was written in c
// Collaborated with Ian Moon on this Homework
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
                    printf(":--: %s\n", lexeme);
                    // printf("String duplicated");
                }
                else
                {
                    if (eol == 1)
                    {
                        break;
                    }
                    addToList(lexeme, current);
                    printf(" --: %s\n", lexeme);
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
                printf("<\n");
                break;
            case PIPE:
                printf("|\n");
                prevUse = 1;
                break;
            case SEMICOLON:
                printf(";\n");
                prevUse = 1;
                break;
            default:
                break;
            }
            rtn = parse_line(NULL);
            if (eol == 1)
            {
                break;
            }
        }
        if (eol != 1)
        {
            printf(" --: EOL\n");
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
