//Add your code here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define DELIM " \t\r\n\a"
#define MAX_SIZE 128

void print_error(){
    fprintf(stderr,"An error has occurred\n");
}

char *paths[MAX_SIZE] = {"/bin"};
int run_cd(char **args,int);
int set_path(char **args,int);
int run_exit(char **args,int);
int perform_command(char line[], int numArgs);
int execute_command(char *new_args[],char *redirect_args[]);
int command_redirect_parallel(char* line, char* split_point);
int command_parallel(char* ret, char* line);
int command_direct(char **args, int numArgs);

char *command_keyword[] = {"cd", "path","exit"};
int (*command_func[]) (char **,int) = {&run_cd,&set_path, &run_exit};

char **splitLine(char *line)
{
    char **input_tokens = (char **)malloc(sizeof(char *) * MAX_SIZE);
    if (!input_tokens)
    {
        print_error();
        free(input_tokens);
        exit(EXIT_FAILURE);
    }

    char *token_strok;
    int pos = 0, bufsize = MAX_SIZE;
    token_strok = strtok(line, DELIM);
    while (token_strok != NULL)
    {
        input_tokens[pos] = token_strok;
        pos ++;
        if (pos >= bufsize)
        {
            bufsize += 64;
            line = realloc(line, bufsize * sizeof(char *));
            if (!line) // Buffer Allocation Failed
            {
            print_error();
            exit(EXIT_FAILURE);
            }
        }
        token_strok = strtok(NULL, DELIM);
    }
    input_tokens[pos] = NULL;
    free(token_strok);
    return input_tokens;
}
int lineSeperate(char* line, char* args[], char* delim) {
    char *save_result;
    int args_total = 0;
    //args = malloc(MAX_SIZE*sizeof(char*));
    if (!args)
        args = malloc(MAX_SIZE*sizeof(char));
    args[args_total] = strtok_r(line, delim, &save_result);
    args_total++;
    while(1){
        args[args_total] = strtok_r(NULL, delim,&save_result);
        if (args[args_total] == NULL)
            break;
        args_total++;
    }
    if (args[0] == NULL)
        return 0;
    return args_total;
}
int execute_command(char *new_args[],char *redirect_args[])
{
    pid_t pid;
    char *full_path = malloc(sizeof(char)*MAX_SIZE);
    char *final_path = malloc(sizeof(char)*MAX_SIZE);
    if (paths[0] == NULL){
        print_error();
        return -1;
    }
    if (new_args == NULL)
        return -1;
    if (new_args[0] == NULL)
        return -1;
    int i = 0;
    while(paths[i] != NULL){
        strcat(strcat(strcpy(full_path, paths[i]),"/"), new_args[0]);
        if(access(full_path, X_OK) == 0){
            strcpy(final_path, full_path);
            break;
        }
        i++;
    }
    //Fork - Wait - Exec functions
    pid = fork();
    if (pid == 0) {
        if(redirect_args){
            //Used 0600 for permissions and others to only create and write to file
            //printf("redirect_args %s\n",redirect_args[0]);
            close(STDOUT_FILENO);
            int fd_out = open(redirect_args[0], O_RDWR | O_CREAT, 0666);//O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);//0600
            if(fd_out==-1)
            {
                print_error();
                return -1;
            }
            else{
                dup2(fd_out, STDOUT_FILENO);   //make stdout go to file
                //dup2(fd_out, 2);   //make stderr go to file
                if (execv(final_path, new_args) == -1) {
                    print_error();
                    free(full_path);
                    free(final_path);
                    exit(EXIT_FAILURE);
                }
                close(fd_out); 
            }
        }
        else{
            if (execv(final_path, new_args) == -1) {
                print_error();

                free(full_path);
                free(final_path);   
                exit(EXIT_FAILURE);
            }
        }
        
    } else if (pid < 0) {
        //Error forking
        print_error();                
        free(full_path);
        free(final_path);   
        exit(EXIT_FAILURE);
    }
    free(full_path);
    free(final_path);
    return pid;
}
int command_redirect_parallel(char* line, char* split_point)
{
    char* Args_command[MAX_SIZE];
    char* Args_parameter[MAX_SIZE];
    split_point[0] = '\0';
    split_point = split_point + 1;
    int argsNum = lineSeperate(line, Args_command,DELIM);
    if (argsNum == 0){
        print_error();
        return -1;
    }
    int restArgc = lineSeperate(split_point, Args_parameter,DELIM);
    if (restArgc != 1){
        print_error();
        return -1;
    }
    int pid = execute_command(Args_command, Args_parameter);
    return pid;
}
int command_parallel(char* ret, char* line){
    char** commands_array = malloc(MAX_SIZE*sizeof(char*));
    int commands_total = lineSeperate(line, commands_array,"&");
    char** paramater_each = malloc(MAX_SIZE*sizeof(char*));//50
    char* split_point = NULL;//malloc(100*sizeof(char));
    int pid[commands_total];
    for (int i = 0; i < commands_total; i++) {
        if ((split_point = strchr(commands_array[i], '>'))){
            pid[i] = command_redirect_parallel( commands_array[i],split_point);
            continue;
        }
        lineSeperate(commands_array[i], paramater_each, DELIM);
        pid[i] = execute_command(paramater_each,NULL);
    }
    for (int i = 0; i < commands_total; i++) {
        if(pid[i]>0)
        {
            int status;
            waitpid(pid[i], &status, WUNTRACED);
            if (status == 1)      
            {
                print_error();
            }     
        }
    }
    free(paramater_each);
    free(commands_array);
    return 0;
}

int set_path(char **args, int numArgs)
{

    if(args[1] == NULL){
        int j = 0;
        while(paths[j] != NULL){
            //paths[j] = NULL;
            free(paths[j]);
            j++;
        }
        paths[0]="";
    }
    int i = 0;
    while(args[i+1] != NULL){
        //printf("%ld\n",strlen(args[i + 1]));
        paths[i] = malloc(strlen(args[i+1]) + 1);// malloc(sizeof(char*) * strlen(args[i + 1]));
        strcpy(paths[i], args[i + 1]);
        //printf("%s;", paths[i]);
        i++;
    }
    return 0;
}
int run_cd(char **args, int numArgs)
{
    if(numArgs != 2){
            print_error();
            return 1;
    }else{
        if(chdir(args[1]) !=0)
        {
            print_error();
            return 1;
        }
    }
    return 0;
}
int run_exit(char **args, int numArgs)
{
    if(args[1] == NULL)
        exit(0);
    else
    {
        print_error();
        return 1;
    }
    return 0;
}
int numCommand() // Function to return number of builtin commands
{
    return sizeof(command_keyword)/sizeof(char *);
}

//This checks and runs the args
int command_direct(char **args, int numArgs){
    if(args[0] == NULL){
        return 1;
    }
    for (int i=0; i< numCommand(); i++) 
    {
        if(strcmp(args[0], command_keyword[i])==0) 
            return (*command_func[i])(args,numArgs); 
    }
    if(paths[0] == NULL){
        print_error();
    }
    else{
        int pid = execute_command(args, NULL);
        if(pid>0)
        {
            int status;
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    return 0;
}
int perform_command(char line[], int numArgs)
{
    char *split_point=NULL;
    char **input_tokens;
    int result=0;
    if ((split_point=strchr(line, '&'))){
                
        return command_parallel(split_point, line);
    }
    else if ((split_point=strchr(line,'>')))
    {
        int pid = command_redirect_parallel(line,split_point);
        if(pid>0)
        {
            int status;
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            return 1;
        }
        return 0;
    }
    else{
        char *input;
        input=line;
        input_tokens = splitLine(input);
        result= command_direct(input_tokens,numArgs);
        free(input_tokens);
    }
    free(split_point);
    return result;
}
int readfile(char filename[MAX_SIZE], int numArgs)
{
    FILE *fptr;
    //char line[200];//200
    fptr = fopen(filename, "r");
    int result=1;
    if (fptr == NULL)
    {
        print_error();
        exit(EXIT_FAILURE);
        return 1;
    }
    else
    {
        size_t line_size = MAX_SIZE;
        char *line = malloc(sizeof(char) * MAX_SIZE);
        while(fgets(line, line_size, fptr)!= NULL)
        {
            if ((strcmp(line, "\n") == 0) || (strcmp(line, "") == 0))
                continue;
            //omit the last \n of the string
            if (line[strlen(line) - 1] == '\n')
                line[strlen(line) - 1] = '\0';
            //exit if EOF is read
            if (line[0] == EOF){
                if(line)
                {
                    free(line);
                }
                return 1;
            }
            //printf("%s\n",line);
            //input = trim(line);
            ////trim_input_tokens(
            /*printf("=input=%s\n",input);
            printf("=0=%s\n",input_tokens[0]);
            printf("=1=%s\n",input_tokens[1]);
            printf("=2=%s\n",input_tokens[2]);
            printf("=3=%s\n",input_tokens[3]);*/
            result=perform_command(line,numArgs);
        }
        fseek (fptr, 0, SEEK_END);
        int size = ftell(fptr);
        if (0 == size) {
            print_error();
            if(line)
            {
                free(line);
            }
            return 0;
        }
        if(line)
        {
            free(line);
        }
            
    }
    fclose(fptr);
    //free(input);
    //free(input_tokens);
    return result;
}

int main(int argc, char *argv[]) {
    //printf("%d",argc);
    if (argc == 2)
    {
        exit(readfile(argv[1],argc));
    }
    else{
        //printf("\nInvalid Number of Arguments");
        print_error();
        exit(1);
    }
    exit(0);
}
