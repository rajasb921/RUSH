// Helper functionss
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CHARS 256
#define MAX_TOKENS 256
#define MAX_PATHS 256
#define MAX_COMMANDS 256

// Parse input into tokens
char** parse_input(char* input){
  char** tokens = malloc(MAX_TOKENS * sizeof(char*));
  int token_count = 0;
  char* token = strtok(input, " \t\r\n\f\v");
  
  while (token != NULL) {
    tokens[token_count] = token;
    token_count++;
    token = strtok(NULL, " \t\n\r\f\v");
  }
  
  return tokens;
}

// Execv without redirection
void execute(char* error_message, char** paths, char** argTokens){
    // Child process
    for (int i = 0; i < MAX_PATHS; i++){
      if (paths[i] != NULL){
        char* path = malloc(strlen(paths[i]) + strlen(argTokens[0]) + 2);
        strcpy(path, paths[i]);
        strcat(path, "/");
        strcat(path, argTokens[0]);
        if (access(path, X_OK) == 0){
          execv(path, argTokens);
          write(STDERR_FILENO, error_message, strlen(error_message));
          exit(1);
        }
      }
    }
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}

// Execv with redirection
void executeRedirection(char* error_message, char** paths, char** argTokens, char* filename){
    // Child process
    for (int i = 0; i < MAX_PATHS; i++){
      if (paths[i] != NULL){
        char* path = malloc(strlen(paths[i]) + strlen(argTokens[0]) + 2);
        strcpy(path, paths[i]);
        strcat(path, "/");
        strcat(path, argTokens[0]);
        if (access(path, X_OK) == 0){
          // Open the file
          int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
          if (fd < 0) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
          }
          dup2(fd, STDOUT_FILENO);
          close(fd);
          execv(path, argTokens);
          write(STDERR_FILENO, error_message, strlen(error_message));
          exit(1);
        }
      }
    }
}


// Check redirection
int isRedirect(char** argTokens){
  int result = 0;
  int i = 0;
  while (argTokens[i] != NULL){
    if (strcmp(argTokens[i], ">") == 0){
      result++;
    }
    i++;
  }
  return result;
}

// Get output file
char* outputFile(char** argTokens){
  int i = 0;
  while (argTokens[i] != NULL){
    if (strcmp(argTokens[i], ">") == 0){
      return argTokens[i+1];
    }
    i++;
  }
  return NULL;
}

// Check num outputfiles
int numFiles(char** argTokens){
  int i = 0;
  int count = 0;
  while (argTokens[i] != NULL){
    if (strcmp(argTokens[i], ">") == 0){
      i++;
      break;
    }
    i++;
  }
  while (argTokens[i] != NULL){
    count++;
    i++;
  }
  return count;
}

// Check whitespace
bool is_only_whitespace(const char *input) {
  while (*input != '\n' && *input != '\0') {
    if (*input != ' ' && *input != '\t' && *input != '\r' && *input != '\f' && *input != '\v') {
      return false; // Found a non-whitespace character
    }
    input++;
  }
  return true; // Only whitespace characters found
}