#ifndef _FORMAT_H
#define _FORMAT_H


int format(char *inputFile, char **outputFile);
int split(char *inputFile, char **trainFile, char **testFile, double percent);
#endif /* _FORMAT_H */