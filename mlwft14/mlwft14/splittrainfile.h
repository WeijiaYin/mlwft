#ifndef _SPLITTRAINFILE_H
#define _SPLITTRAINFILE_H

// int splitTrainFile(int k, char *inputfile, char *outputFolder);
// int trainTrainFile(int k, char *folderPath, char *outputModel);
int kcrossvalidation(int k, char *trainFile, char *parameterFile, char *testFile);
#endif /* _SPLITTRAINFILE_H */