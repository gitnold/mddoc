//some header file definitions.
#ifndef notes_header_h
#define notes_header_h

#include <stddef.h>
#define TMP_SIZE 20 

typedef struct {
  char* start;
  char* end;
  unsigned int len;
}MarkdownBlock;

MarkdownBlock getCode(size_t offset, char* markdown, MarkdownBlock block);
int writeSourceFile(MarkdownBlock block, const char *outputFile);
int compileNoteFile(const char* filename);
void extractCodeBLocks(char* markdown, const char *outputFile);
void runCompiler(char *outputFile, char *binaryName);
MarkdownBlock advance(MarkdownBlock block);
void printHelp();
char* readFile(const char *s);


#endif


