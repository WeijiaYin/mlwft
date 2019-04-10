#include "stdafx.h"
#include "crossvalidation.h"
#include "stdio.h"
#include "io.h"
#include "svmpredict.h"
#include "svmtrain.h"


char *filename = "./model/model.";
char *outcome = "./outcome/outcome.";


int kcrossvalidation(int k, char *trainFile, char *parameterFile, char *testFile)
{
	char line[1024];
	char splitname[100];
	char accuracyname[100];
	char numbertochar[10];
	char numbertochar1[10];
	int count = countLine(trainFile);
	int countParameters = countLine(parameterFile);
	int number = count / k;
	FILE *p;
	FILE *p1;
	char bestModel[100];
	double accuracy;
	double accuracyTrain;
	double maxAccuracy = 0.0;
	double maxAccuracyOnTest = 0.0;
	double maxAccuracyOnTrain = 0.0;
	char bestModelOnTest[100];
	char bestConfusionMatrix[100];
	FILE *parameters;
	char *command;
	char lineParameters[1024];
	char bestParameters[1024];
	int h ,i, j= 0;
	char *s;
	double ac;
	FILE *outcomeFile;
	int line_count;
	FILE *confusionMatrixOnTest;
	FILE *trainOutcomeFile;

	rmdir("model");
	mkdir("model");
	rmdir("outcome");
	mkdir("outcome");
	parameters = fopen(parameterFile, "r");
	for (h = 0; h < countParameters; h++)
	{
		strcpy(accuracyname, outcome);
		itoa(h, numbertochar1, 10);
		fgets(lineParameters, sizeof(lineParameters), parameters);
		strcat(accuracyname, numbertochar1);
		maxAccuracy = 0;
		for (i = 1; i <= k; i++)
		{
			FILE *tf = fopen(trainFile, "r");
			strcpy(splitname, filename);
			strcat(splitname, numbertochar1);
			itoa(i, numbertochar, 10);
			strcat(splitname, numbertochar);
			s = splitname;
			p = fopen("temp_train", "w");
			p1 = fopen("temp_test", "w");
			for (j = 0; j < count; j++)
			{
				if (j <= i * number && j >= (i - 1) * number)
				{
					fgets(line, sizeof(line), tf);
					fprintf(p1, "%s", line);
				}
				else
				{
					fgets(line, sizeof(line), tf);
					fprintf(p, "%s", line);

				}
			}
			fclose(p);
			fclose(p1);
			fclose(tf);
			command = svmTrain("temp_train", s, lineParameters);
			accuracy = svmpredict("temp_test", s, "temp_outcome");
			if (accuracy > maxAccuracy)
			{
				maxAccuracy = accuracy;
				strcpy(bestModel, s);
			}
		}

		accuracyTrain = svmpredict(testFile, bestModel, accuracyname);
		if (accuracyTrain > maxAccuracyOnTest)
		{
			maxAccuracyOnTest = accuracyTrain;
			strcpy(bestModelOnTest, bestModel);
			strcpy(bestConfusionMatrix, accuracyname);
			maxAccuracyOnTrain = maxAccuracy;
			strcpy(bestParameters, command);
		}

	}

	printf("best parameters:%s, best model:%s accuracy on trainset: %lf, accuracy on testset: %lf", bestParameters, bestModelOnTest, maxAccuracyOnTrain, maxAccuracyOnTest);
	ac = svmpredict(trainFile, bestModelOnTest, "train.outcome");

	outcomeFile = fopen("outcome.csv", "w");
	fprintf(outcomeFile, "best parameters:\n");
	fprintf(outcomeFile, "%s\n", bestParameters);
	fprintf(outcomeFile, "accuracy on Train: ");
	fprintf(outcomeFile, "%lf\n", ac);
	fprintf(outcomeFile, "confusion matrix on Train:\n");
	trainOutcomeFile = fopen("train.outcome", "r");
	line_count = countLine("train.outcome");
	for (i = 0; i < line_count; i++)
	{
		fgets(line, sizeof(line), trainOutcomeFile);
		fprintf(outcomeFile, "%s", line);
	}

	fprintf(outcomeFile, "\n");
	fprintf(outcomeFile, "accuracy on Test: ");

	fprintf(outcomeFile, "%lf\n", maxAccuracyOnTest);
	fprintf(outcomeFile, "confusion matrix on Test:\n");
	confusionMatrixOnTest = fopen(bestConfusionMatrix, "r");
	line_count = countLine(bestConfusionMatrix);
	for (i = 0; i < line_count; i++)
	{
		fgets(line, sizeof(line), confusionMatrixOnTest);
		fprintf(outcomeFile, "%s", line);
	}


	fclose(confusionMatrixOnTest);
	fclose(trainOutcomeFile);
	fclose(parameters);
	fclose(outcomeFile);
	return 0;
}


int loocrossvalidation(char *trainFile, char *parameterFile, char *testFile)
{
	int count = countLine(trainFile);
	kcrossvalidation(count, trainFile, parameterFile, testFile);
	return 0;
}

int nocrossvalidation(char *trainFile, char *parameterFile, char *testFile)
{
	char line[1024];
	char splitname[50];
	char numbertochar1[10];
	int countParameters = countLine(parameterFile);
	char bestModel[50];
	double accuracy;
	double maxAccuracy = 0.0;
	double maxAccuracyOnTrain = 0.0;
	char bestModelOnTest[50];
	FILE *parameters;
	char *command;
	char lineParameters[1024];
	char bestParameters[1024];
	FILE *tf;
	int h = 0;
	char *s;
	FILE *outcomeFile;
	char testConfusion[50];
	char bestTestComfusion[50];
	FILE *confusionTrain;
	FILE *confusionTest;
	int num;

	rmdir("model");
	mkdir("model");
	rmdir("outcome");
	mkdir("outcome");
	parameters = fopen(parameterFile, "r");
	for (h = 0; h < countParameters; h++)
	{
		itoa(h, numbertochar1, 10);
		strcpy(testConfusion, outcome);
		strcat(testConfusion, numbertochar1);
		fgets(lineParameters, sizeof(lineParameters), parameters);
		tf = fopen(trainFile, "r");
		strcpy(splitname, filename);
		strcat(splitname, numbertochar1);
		s = splitname;
		command = svmTrain(trainFile, s, lineParameters);
		accuracy = svmpredict(testFile, s, testConfusion);
		if (accuracy > maxAccuracy)
		{
			maxAccuracy = accuracy;
			strcpy(bestModel, s);
			strcpy(bestParameters, command);
			strcpy(bestTestComfusion, testConfusion);
		}
	}

	maxAccuracyOnTrain = svmpredict(trainFile, bestModel, "train.outcome");

	printf("best parameters:%s, best model:%s, accuracy on testset: %lf", bestParameters, bestModel, maxAccuracy);
	outcomeFile = fopen("outcome.csv", "w");
	fprintf(outcomeFile, "best parameters:\n");
	fprintf(outcomeFile, "%s\n", bestParameters);
	fprintf(outcomeFile, "best accuracy on Train:");
	fprintf(outcomeFile, "%lf\n", maxAccuracyOnTrain);
	confusionTrain = fopen("train.outcome", "r");
	num = countLine("train.outcome");
	for (h = 0; h < num; h++)
	{
		fgets(line, sizeof(line), confusionTrain);
		fprintf(outcomeFile, "%s", line);
	}
	fclose(confusionTrain);
	fprintf(outcomeFile, "best accuracy on Test:");
	fprintf(outcomeFile, "%lf\n", maxAccuracy);
	confusionTest = fopen(bestTestComfusion, "r");
	num = countLine(bestTestComfusion);
	for (h = 0; h < num; h++)
	{
		fgets(line, sizeof(line), confusionTest);
		fprintf(outcomeFile, "%s", line);
	}

	fclose(confusionTest);
	fclose(outcomeFile);
	fclose(parameters);
	return 0;
}