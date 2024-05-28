/* Created by Rajas Bhate */
#include "rush.h"

int main(int argc, char* argv[]){
  
  // Define error message
  char error_message[30] = "An error has occurred\n";
  if (argc > 1){
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
  }
  
  // Init paths array
  char* paths[MAX_PATHS] = {"/bin"};

  while (1){
    // Print prompt
    printf("rush> ");
    fflush(stdout);
    
    // Read input
    char input[MAX_CHARS];
    fgets(input, MAX_CHARS, stdin);
    if (strcmp(input, "\n") == 0){
      continue;
    }
    bool whitespace = is_only_whitespace(input);
    if (whitespace){      // If input is only whitespace -> continue
      continue;
    }
    
    
    // Parse into tokens
    char** tokens = parse_input(input);
    int numTokens = 0;
    for (int i = 0; tokens[i] != NULL; i++){
      numTokens++;
    }

    // exit
    if (strcmp(tokens[0], "exit") == 0){          
      if (numTokens > 1){
        write(STDERR_FILENO, error_message, strlen(error_message));
        continue;
      }
      else{ exit(0);}
    }

    // cd
    else if (strcmp(tokens[0], "cd") == 0){    
      if (numTokens != 2){
        write(STDERR_FILENO, error_message, strlen(error_message));
        continue;
      }else{
        if (chdir(tokens[1]) != 0){
          write(STDERR_FILENO, error_message, strlen(error_message));
          continue;
        }else{continue;}
      }
    }
    
    // Path
    else if (strcmp(tokens[0], "path") == 0){   
      if (numTokens == 1){
        for (int i = 0; i < MAX_PATHS; i++){
          paths[i] = NULL;
        }
        continue;
      }
      // Set new paths
      for (int i = 1; i < numTokens; i++){
        paths[i-1] = strdup(tokens[i]);
      }
      continue;
    }

    // Recombine tokens into commands, separated by "&"
    else{
      char* singleCommandTokens[MAX_COMMANDS];                              // Array of tokens for each command
      char singleCommandString[MAX_CHARS];                                  // Combine these tokens into a single string
      char** parallelCommandsArray = malloc(MAX_COMMANDS * sizeof(char*));  // Add this string to array
      int numExec = 0;
      
      // Initialize singleCommandTokens array
      memset(singleCommandTokens, 0, sizeof(singleCommandTokens));

      // Initialize parallelCommandsArray array  
      for (int i = 0; i < MAX_COMMANDS; i++) {
        parallelCommandsArray[i] = NULL;
      }

      for (int i = 0; i<numTokens; i++){
      // Copy tokens for one command to the array singleCommandTokens
        int j = 0; // Index for singleCommandTokens
        while(i < numTokens && j < MAX_COMMANDS){
          if ((tokens[i] != NULL) && (strcmp(tokens[i], "&") != 0)){
            singleCommandTokens[j] = strdup(tokens[i]);
            j++;
            i++;
          }else{
            break;
          }
        }

        // Concatenate singleCommandTokens into singleCommandString
        int k = 0;
        while (singleCommandTokens[k] != NULL){
          strcat(singleCommandString, singleCommandTokens[k]);
          strcat(singleCommandString, " ");
          k++;
        }

        // Add singleCommandString to parallelCommandsArray
        parallelCommandsArray[numExec] = strdup(singleCommandString);
        numExec++;
        
        // Clear singleCommandTokens array
        memset(singleCommandTokens, 0, sizeof(singleCommandTokens));

        // Clear singleCommandString
        memset(singleCommandString, 0, sizeof(singleCommandString));
      }

      // parallelCommandsArray array is now filled with commands (i.e. parallelCommandsArray[0] = "ls", parallelCommandsArray[1] = "echo hello" ...)

      int pidArr[MAX_COMMANDS];
      int numPids = 0; // Number of pids
      // Loop through parallelCommandsArray check commands
      for (int i = 0; i < numExec; i++){
        pid_t pid = fork();
        pidArr[numPids] = pid;
        numPids++;
        if (pid < 0){
          write(STDERR_FILENO, error_message, strlen(error_message));
          exit(1);
        }else if (pid == 0){
          // Child process
          char** argTokens = parse_input(parallelCommandsArray[i]);  // Parse command into tokens
          if (strcmp(argTokens[0], ">") == 0){
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
          }
          int redirect = isRedirect(argTokens);
          if (redirect == 0){
            // Execute command
            execute(error_message, paths, argTokens);
            exit(1);
          }else if (redirect == 1){
            int n = numFiles(argTokens);
            if (n != 1){
              write(STDERR_FILENO, error_message, strlen(error_message));
              exit(1);
            }
            char* filename = outputFile(argTokens);
            char* newCommandTokens[MAX_COMMANDS];
            // Initialize singleCommandTokens array
            memset(newCommandTokens, 0, sizeof(newCommandTokens));
            
            int k = 0;
            while (strcmp(argTokens[k], ">") != 0){
              newCommandTokens[k] = argTokens[k];
              k++;
            }
            executeRedirection(error_message, paths, newCommandTokens, filename);
            exit(1);
          }else{
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
          }
          exit(0);
        }
      }
      // Parent process
      for (int i=0; i<numPids; i++){
        int status;
        waitpid(pidArr[i], &status, 0);
      }
    }
  } 
  return EXIT_SUCCESS;
}