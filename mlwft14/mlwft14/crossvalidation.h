#ifndef _CROSSVALIDATION_H
#define _CROSSVALIDATION_H

// int splitTrainFile(int k, char *inputfile, char *outputFolder);
// int trainTrainFile(int k, char *folderPath, char *outputModel);
int kcrossvalidation(int k, char *trainFile, char *parameterFile, char *testFile);
int loocrossvalidation(char *trainFile, char *parameterFile, char *testFile);
int nocrossvalidation(char *trainFile, char *parameterFile, char *testFile);
#endif /* _CROSSVALIDATION_H */