/*
* CSCI3150 Assignment 2 - Writing a Simple Shell
* Feel free to modify the given code.
* Press Ctrl-D to end the program
*
*/

/* Header Declaration */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/* Function Declaration */
int getUserInput(char* input);
void tokenizeInput(char* input);

/* variable and constant Declaration */
int handleToken();
char *tokenList[255];
int tokenNum;
char cwd[255];
const char *PATH1 = "/bin/";
const char *PATH2 = "/usr/bin/";
const char *PATH3 = "./";
struct queue {char *command; struct queue* next;};
struct queue *head = NULL, *tail = NULL;

/* Functions */
int main(int argc,char* argv[])
{
    //initialize
    int isExit = 0;
    int i = 0;
    for (i = 0; i < 255; i++)     tokenList[i] = (char*)malloc(sizeof(char) * 255);
    getcwd(cwd, 255);
    //signal handler
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

     do {
        char rawInput[255];
        isExit = getUserInput(rawInput);
        if (isExit) break;
        if (strlen(rawInput) == 0)  //only \n read
        {
            continue;
        }
        tokenNum = 0;
        tokenizeInput(rawInput);
        isExit = handleToken();
        } while(isExit == 0);

    for (i = 0; i < 255; i++)     free(tokenList[i]);
    struct queue *ptr = head;
    while (ptr != NULL)
    {
        ptr = ptr->next;
        //need to free 
    }
    return 0;
}

/*
  GetUserInput()
  - To parse User Input and remove new line character at the end.
  - Copy the cleansed input to parameter.
  - Return 1 if encountered EOF (Ctrl-D), 0 otherwise.
*/
int getUserInput(char* input)
{
  char buf[255];
  char *s = buf;
  printf("[3150 Shell:%s]=> ",cwd);
  if(fgets(buf,255,stdin) == NULL)
  {
    putchar('\n');
    return 1;
  }
  // Remove \n
  for(;*s != '\n';s++);
  *s = '\0';

  strcpy(input,buf);
  return 0;
}
/*
  tokenizeInput()
  - Tokenize the string stored in parameter, delimiter is space
  - In given code, it only prints out the token.
  Please implement the remaining part.
*/

void tokenizeInput(char* input)
{
    //the input contains no '\n', save into queue
    struct queue *ptr = malloc(sizeof(struct queue));
    ptr->command = malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(ptr->command, input);
    ptr->next = NULL;
    if (head == NULL)
    //the queue is empty
    {
        head = ptr;
        tail = ptr;
    }
    else
    {
       tail->next = ptr;
       tail = ptr;
    }

    char buf[255];
    strcpy(buf,input);

    char* token = strtok(buf," ");
    while(token != NULL)
    {
        strcpy(tokenList[tokenNum], token);
        tokenNum++;
        token = strtok(NULL," ");
    }
    return;
}

/*
   handle the token list
   return:
   0 if success; 1 if exit
*/
int handleToken()
{
    //customized shell commend
    //gofolder
    if (strcmp(tokenList[0], "gofolder") == 0)
    {
        if (tokenNum != 2)
        {
            //error occurred
            printf("gofolder: wrong number of arguments\n");
            return 0;
        }
        char path[255];
        strcpy(path, tokenList[1]);
        if (chdir(path) == -1)
        {
            //error occurred
            printf("{%s}: cannot change directory\n", path);
            return 0;
        }
        getcwd(cwd, 255);
        return 0;
    }

    //log
    if (strcmp(tokenList[0], "log") == 0)
    {
        //the queue is at least length 1
        struct queue *ptr = head;
        int index = 0;
        while (ptr != NULL)
        {
            index ++;
            printf("[%d]: %s\n", index, ptr->command);
            ptr = ptr->next;
        }
        return 0;
    }

    //bye
    if (strcmp(tokenList[0], "bye") == 0)
    {
        if (tokenNum != 1)
        {
            printf("bye: wrong number of arguments\n");
            return 0;
        }
        return 1;
    }

    //fork and execute programs
    //first check if || or && is the first token
    if ((strcmp(tokenList[0], "||") == 0) || (strcmp(tokenList[0], "&&") == 0))
    {
        printf("Error: Invalid input command");
        return 0;
    }
    int pre = 0; 
    //the status of the previous command
    //0 for success, 1 for fail, -1 for undefined
    int i = 0;
    char *token;
    while (i < tokenNum)
    {
        //handle command
        token = tokenList[i];
        //handle // and &&
        if (strcmp(token, "||") == 0)
        {   

            if (pre)    return 0;   //skip all things after
            //if okay, the continue to handle
            i++;
            continue;
        }
        if (strcmp(token, "&&") == 0)
        {
            if (!pre)   return 0;    //skip all things after
            i++;
            continue;
        }
        //handle program
        //get argument list
        char *argument[tokenNum];
        int argn = 0;
        i++;
        char *buffer;
        while (i < tokenNum)
        {
            buffer = tokenList[i];
            if ((*(buffer) == '|' && *(buffer + 1) == '|' && *(buffer + 2) == '\0') || (*(buffer) == '&' && *(buffer + 1) == '&' && *(buffer + 2) == '\0'))
                break;
            argn++;
            argument[argn] = tokenList[i];
            i++;
        }
        argument[argn+1] = NULL;
        //check if there is / in the file name

        buffer = token;
        while (*(buffer) != '\0')
        {
            if (*(buffer) == '/') break;
            buffer++;
        }

        //command is a path
        if (*(buffer) != '\0')
        {
            char *temp = token + strlen(token) - 1;
            while (*(temp) != '/')
                temp --;
            temp++;
            argument[0] = token;    //name should be path
            int pid;
            if ((pid = fork()) == 0)
            {
                execv(token, argument);
                //error occurred
                printf("{%s}: command not found\n", token);
                exit(0);
            }
            int status;
            waitpid(pid, &status, WUNTRACED);
            if (WEXITSTATUS(status) != 0)
                pre = 0;
            else
                pre = 1;
            continue;
        }

        //command is a filename
        if (*(buffer) == '\0')
        {
            argument[0] = "";
            char path1[255];
            char path2[255];
            char path3[255];
            path1[0] = '\0';
            path2[0] = '\0';
            path3[0] = '\0';
            strcat(path1, PATH1);   strcat(path1, token);
            strcat(path2, PATH2);   strcat(path2, token);
            strcat(path3, PATH3);   strcat(path3, token);
            int pid;
            if ((pid = fork()) == 0)
            {
                argument[0] = path1;
                execv(path1, argument);
                argument[0] = path2;
                execv(path2, argument);
                argument[0] = path3;
                execv(path3, argument);
                printf("{%s}: command not found\n", token);
                exit(-1);
            }
            int status;
            waitpid(pid, &status, WUNTRACED);
            if (WEXITSTATUS(status) != 0)
                pre = 0;        //error occurred
            else
                pre = 1;
            continue;
        }
    }
    return 0;
}
