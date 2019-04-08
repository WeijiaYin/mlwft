// mlwft14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "svm.h"
#include "svmscale.h"
#include "format.h"
#include "stdio.h"
#include "svmtrain.h"
#include "svmpredict.h"
#include "string.h"
#include "crossvalidation.h"
#include "io.h"
#include "malloc.h"


/*char inputFile[200];
char *trainFile;
char *testFile;
double proportions;
char *outputFile;
char *outputFile1;
char *svmModel;
char normalize[50];
char train[50];
char test[50];
char a[100];
char b[100];
char k[3];
*/


int main()
{
/*	searchMethod();
	printf("Input the path of the train dataset and test dataset, split by ,\n");
	scanf("%s", &inputFile);
	trainFile = strtok(inputFile, ",");
	testFile = strtok(NULL, ",");
	format(trainFile, &outputFile);
	if (testFile == NULL)
	{
		printf("only one file provided, input the proportions to split the file\n");
		scanf("%lf", &proportions);
		split(trainFile, &outputFile, &outputFile1, proportions);
	}
	else
	{
		format(testFile, &outputFile1);
	}
	strcpy(a, outputFile);
	strcpy(b, outputFile1);
	trainFile = a;
	testFile = b;
	printf("Do you want to normalize data? Input Yes if yes, input No if no\n");
	scanf("%s", &normalize);
	if (strcmp(normalize, "Yes") == 0)
	{
		strcpy(train, trainFile);
		outputFile = strtok(train, ".");
		strcat(outputFile, "_train.scale");
		scale(trainFile, outputFile);
		trainFile = outputFile;
		strcpy(test, testFile);
		outputFile1 = strtok(test, ".");
		strcat(outputFile1, "_test.scale");
		scale(testFile, outputFile1);
		testFile = outputFile1;
	}


	printf("Input a k, for crossvalidation, 1-no crossvalidation,2~10-regularvalidation,LOO-leave-one-out crossvalidation\n");
	scanf("%s", &k);
	if (strcmp(k, "1") == 0)
	{
		nocrossvalidation(trainFile, "parameters.txt", testFile);
	}
	else if (strcmp(k, "2") == 0 || strcmp(k, "3") == 0 || strcmp(k, "4") == 0 || strcmp(k, "5") == 0 || strcmp(k, "6") == 0 || strcmp(k, "7") == 0 || strcmp(k, "8") == 0 || strcmp(k, "9") == 0 || strcmp(k, "10") == 0)
	{
		int a = atoi(k);
		kcrossvalidation(a, trainFile, "parameters.txt", testFile);
	}
	else if (strcmp(k, "LOO"))
	{
		loocrossvalidation(trainFile, "parameters.txt", testFile);
	}
	
//	splitTrainFile(3, "diabetes_split_train.scale", "asdsf");
//	trainTrainFile(3, ".\\train", "sdfad");
//	kcrossvalidation(3, "diabetes_split_train.scale", "parameters.txt", "diabetes_split_test.scale");
//	loocrossvalidation("diabetes_split_train.scale", "parameters.txt", "diabetes_split_test.scale");
//	nocrossvalidation("diabetes_split_train.scale", "parameters.txt", "diabetes_split_test.scale");
//	svmTrain();
//	svmpredict();
*/
	seperateClass("glass.scale.txt", "oout.txt");
    return 0;
}



