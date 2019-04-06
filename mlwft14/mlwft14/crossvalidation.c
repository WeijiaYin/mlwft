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
	char line[10000];
	char splitname[100];
	char accuracyname[100];
	char numbertochar[10];
	char numbertochar1[10];
	int count = countLine(trainFile);
	int countParameters = countLine(parameterFile);
	int number = count / k;
	FILE *p;
	FILE *p1;
	char bestModel[1000];
	double accuracy;
	double accuracyTrain;
	double maxAccuracy = 0.0;
	double maxAccuracyOnTest = 0.0;
	double maxAccuracyOnTrain = 0.0;
	char bestModelOnTest[1000];
	char bestConfusionMatrix[1000];
	FILE *parameters;
	char *command;
	char lineParameters[100000];
	char bestParameters[100000];
	parameters = fopen(parameterFile, "r");
	for (int h = 0; h < countParameters; h++)
	{
		strcpy(accuracyname, outcome);
		itoa(h, numbertochar1, 10);
		fgets(lineParameters, 10000, parameters);
		strcat(accuracyname, numbertochar1);
		for (int i = 1; i <= k; i++)
		{
			FILE *tf = fopen(trainFile, "r");
			strcpy(splitname, filename);
			strcat(splitname, numbertochar1);
			itoa(i, numbertochar, 10);
			strcat(splitname, numbertochar);
			char *s = splitname;
			p = fopen("temp_train", "w");
			p1 = fopen("temp_test", "w");
			for (int j = 0; j < count; j++)
			{
				if (j <= i * number && j >= (i - 1) * number)
				{
					fgets(line, 10000, tf);
					fprintf(p1, "%s", line);
				}
				else
				{
					fgets(line, 10000, tf);
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
	double ac = svmpredict(trainFile, bestModelOnTest, "train.outcome");

	FILE *outcomeFile;
	int line_count;
	outcomeFile = fopen("outcome.csv", "w");
	fprintf(outcomeFile, "best parameters:%s\n");
	fprintf(outcomeFile, "%s\n", bestParameters);
	fprintf(outcomeFile, "accuracy on Train: ");
	fprintf(outcomeFile, "%lf\n", ac);
	fprintf(outcomeFile, "confusion matrix on Train:\n");
	FILE *trainOutcomeFile;
	trainOutcomeFile = fopen("train.outcome", "r");
	line_count = countLine("train.outcome");
	for (int i = 0; i < line_count; i++)
	{
		fgets(line, 100000, trainOutcomeFile);
		fprintf(outcomeFile, "%s", line);
	}

	fprintf(outcomeFile, "\n");
	fprintf(outcomeFile, "accuracy on Test: ");

	fprintf(outcomeFile, "%lf\n", maxAccuracyOnTest);
	fprintf(outcomeFile, "confusion matrix on Test:\n");
	FILE *confusionMatrixOnTest;
	confusionMatrixOnTest = fopen(bestConfusionMatrix, "r");
	line_count = countLine(bestConfusionMatrix);
	for (int i = 0; i < line_count; i++)
	{
		fgets(line, 100000, confusionMatrixOnTest);
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
	char line[10000];
	char splitname[100];
	char numbertochar[10];
	char numbertochar1[10];
	int count = countLine(trainFile);
	int k = count;
	int countParameters = countLine(parameterFile);
	int number = count / k;
	FILE *p;
	FILE *p1;
	char bestModel[1000];
	double accuracy;
	double accuracyTrain;
	double maxAccuracy = 0.0;
	double maxAccuracyOnTest = 0.0;
	double maxAccuracyOnTrain = 0.0;
	char bestModelOnTest[1000];
	FILE *parameters;
	char *command;
	char lineParameters[100000];
	char bestParameters[100000];
	parameters = fopen(parameterFile, "r");
	for (int h = 0; h < countParameters; h++)
	{
		fgets(lineParameters, 10000, parameters);
		for (int i = 1; i <= k; i++)
		{
			FILE *tf = fopen(trainFile, "r");
			strcpy(splitname, filename);
			itoa(h, numbertochar1, 10);
			strcat(splitname, numbertochar1);
			itoa(i, numbertochar, 10);
			strcat(splitname, numbertochar);
			char *s = splitname;
			p = fopen("temp_train", "w");
			p1 = fopen("temp_test", "w");
			for (int j = 0; j < count; j++)
			{
				if (j <= i * number && j >= (i - 1) * number)
				{
					fgets(line, 10000, tf);
					fprintf(p1, "%s", line);
				}
				else
				{
					fgets(line, 10000, tf);
					fprintf(p, "%s", line);

				}
			}
			fclose(p);
			fclose(p1);
			fclose(tf);
			command = svmTrain("temp_train", s, lineParameters);
			accuracy = svmpredict("temp_test", s, "");
			if (accuracy > maxAccuracy)
			{
				maxAccuracy = accuracy;
				strcpy(bestModel, s);
			}
		}

		accuracyTrain = svmpredict(testFile, bestModel, "");
		if (accuracyTrain > maxAccuracyOnTest)
		{
			maxAccuracyOnTest = accuracyTrain;
			strcpy(bestModelOnTest, bestModel);
			maxAccuracyOnTrain = maxAccuracy;
			strcpy(bestParameters, command);
		}

	}

	printf("best parameters:%s, best model:%s accuracy on trainset: %lf, accuracy on testset: %lf", bestParameters, bestModelOnTest, maxAccuracyOnTrain, maxAccuracyOnTest);

	fclose(parameters);
	return 0;
}

int nocrossvalidation(char *trainFile, char *parameterFile, char *testFile)
{
	char line[10000];
	char splitname[100];
	char numbertochar[10];
	char numbertochar1[10];
	int countParameters = countLine(parameterFile);
	FILE *p;
	FILE *p1;
	char bestModel[1000];
	double accuracy;
	double accuracyTrain;
	double maxAccuracy = 0.0;
	double maxAccuracyOnTest = 0.0;
	double maxAccuracyOnTrain = 0.0;
	char bestModelOnTest[1000];
	FILE *parameters;
	char *command;
	char lineParameters[100000];
	char bestParameters[100000];
	parameters = fopen(parameterFile, "r");
	for (int h = 0; h < countParameters; h++)
	{
		fgets(lineParameters, 10000, parameters);
		FILE *tf = fopen(trainFile, "r");
		strcpy(splitname, filename);
		itoa(h, numbertochar1, 10);
		strcat(splitname, numbertochar1);
		char *s = splitname;
		command = svmTrain(trainFile, s, lineParameters);
		accuracy = svmpredict(testFile, s, "");
		if (accuracy > maxAccuracy)
		{
			maxAccuracy = accuracy;
			strcpy(bestModel, s);
			strcpy(bestParameters, command);
		}
	}

	printf("best parameters:%s, best model:%s, accuracy on testset: %lf", bestParameters, bestModel, maxAccuracy);

	fclose(parameters);
	return 0;
}