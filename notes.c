#include "notes.h"
#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*TODO: 
    * add better error handling with the errno value.
    * check file extensions
    * use the no of elements in the array to see if the blocks
      are formatted correctly. total no of elements should be even.
    * try using directories instead of everything in one directory.Then check no of files in dir.
    * write file once instead of multiple times.
    * brainstorm test cases.
*/

MarkdownBlock block = {0};
bool checkFileExists(const char *file);

int main(int argc, char *argv[]) {
    //handle some logic here in the main function.
    char *filename, *outputFile,*binaryName;
    filename = argv[1];
    char tmp[TMP_SIZE] = "a";
    int response;
    char str_response[TMP_SIZE];
    
    switch (argc) {
        case 1: 
            printf("Too few arguments given.!!\n");
            printHelp();
            return EXIT_FAILURE;
            break;
        case 2:
            if (checkFileExists("main.c") == false) {
                outputFile = "main.c";
                binaryName = "main";
            } else if (checkFileExists("main.c") == true) {
                printf("Default file already exists! Do you want to overwrite it ir create a new one?\n");
                printf("Overwrite [ 1 ]\nCreate new [ 2 ].\nEnter number ->>>");
                scanf("%d", &response);

                switch (response) {
                    case 1:
                        remove("main.c");
                        remove("main");
                        binaryName = "main";
                        break;
                    case 2:
                        printf("\nEnter name of newfile [no spaces]>>");
                        fgets(str_response, 20, stdin);
                        outputFile = str_response;
                        strncpy(tmp, outputFile, strlen(outputFile));
                        binaryName = strtok(tmp, ".");
                        break;
                    default:
                        printf("\nWrong input supplied!! Must be 1 or 2!!\n");

                }
            }
            break;
        case 3: 
            outputFile = argv[2];
            strncpy(tmp, outputFile, strlen(outputFile));
            binaryName = strtok(tmp, ".");
            printf("%s", binaryName);
            break;
        case 4:
            outputFile = argv[2];
            binaryName = argv[3];
            break;
        default:
            printf("Too many arguments supplied");
    }


    if (strcmp("-h", argv[1]) == 0) {
        printHelp();
        return EXIT_SUCCESS;
    }


    char* markdown = readFile(filename);
    extractCodeBLocks(markdown, outputFile);
    runCompiler(outputFile, binaryName); 

    return EXIT_SUCCESS;
}

bool checkFileExists(const char *file) {
    struct stat fileinfo;
    if (stat(file, &fileinfo) == 0) {
        return true;
    } else {
        return false;
    }

}


void printHelp(void) {
    printf("Welcome to Doc Serializer ^_^.\n All your pretty markdown and code in one file..\n\n");
    printf("Usage ->\t mddoc [-h] [foo.md foo.c]\n");

}

char* readFile(const char *filepath) {

  FILE *fptr;
  fptr = fopen(filepath, "r");

  if (fptr == NULL) {
    perror("Error opening file");
    exit(1);
  }

//move pointer to end of file.
  fseek(fptr, 0, SEEK_END);

  //tell current position of pointer in bytes = file size.
  size_t file_size = ftell(fptr);

  //return pointer to start of file.
  rewind(fptr);

  char *md_buffer = malloc(file_size + 1);
  size_t size_read = fread(md_buffer, 1, file_size, fptr);

  if (size_read != file_size) {
    perror("Error in reading file! Incomplete read!");
    free(md_buffer);
    fclose(fptr);
    exit(1);
  }

  md_buffer[file_size] = '\0';

  fclose(fptr);
  return md_buffer;

}


void extractCodeBLocks(char* markdown,  const char *outputFile) {
    size_t file_size = strlen(markdown);
    unsigned int count = 0;
    unsigned int offsets[10] = {0};

    for (size_t i = 0; i < file_size; i++) {
        if (markdown[i] == '`' && (markdown[i+1] == markdown[i+2])) {
            offsets[count] = i;
            if (count % 2 == 0) {
                MarkdownBlock md = getCode(offsets[count], markdown, block);
                int status = writeSourceFile(md, outputFile);
                if (status != 0) break; 
            }

            count++;
        }
    }
    free(markdown);
}

MarkdownBlock getCode(size_t offset, char* markdown,  MarkdownBlock block) {

    if (block.start == NULL) {
        block.start = &markdown[offset+3];
        block.end = &markdown[offset+3];
        block = advance(block);
        return block;
    } else {
        block = advance(block);
        return block;
    }
}

MarkdownBlock advance(MarkdownBlock block) {
    while (*(block.end) != '`' && *(block.end) != '\0') {
        block.end++;
        block.len++;
    }
    return block;
}

int writeSourceFile(MarkdownBlock block, const char *outputFile) {
    //use the blocks or the markdown string directly.
    FILE *sourceFile;
    unsigned short status = 0;
    sourceFile = fopen(outputFile, "a");

    while (block.start != block.end) {
        if (fputc(*(block.start), sourceFile) != EOF) {
            block.start++;

        }else if (fputc(*(block.start), sourceFile) == EOF){
            perror("Error writing to file!!");
            status = 1;
            break;

        } else {
            status=1;
            break;
        }
    }
    fclose(sourceFile);

    return status;

}

void runCompiler(char *outputFile, char *binaryName) {
    pid_t pid;
    pid = fork();
    // * read on zombie processes.
    if (pid == -1) {
        perror("Failed to create child process");
    }
    if (pid == 0) {
        printf("[ -INFO - ] Compiling extracted source file....");
        char *argv[] = {"gcc", outputFile, "-Wall", "-Wextra", "-o", binaryName, NULL };
        char *envp[] = { "PATH=/usr/bin:/bin", NULL };
        int err = execve("/usr/bin/gcc", argv, envp);

        if (err == -1) {
            printf("Could not run gcc. Make sure its in /usr/bin/gcc");
            return;    
        }

    } else {
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus)) {
            int statusCode = WEXITSTATUS(wstatus);
            if (statusCode == 0) {
                printf("Successfully compiled extracted code\n");
            }
        }
    }
}
