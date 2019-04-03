#include "stdafx.h"
#include "splittrainfile.h"
#include "stdio.h"
#include "io.h"
#include "svmpredict.h"
#include "svmtrain.h"

char line[10000];
char *filename = "./model/model.";
char splitname[100];
char numbertochar[10];
char numbertochar1[10];
int ln = 0;
int max;


int kcrossvalidation(int k, char *trainFile, char *parameterFile, char *testFile)
{
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
	FILE *parameters;
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
			svmTrain("temp_train", s, lineParameters);
			accuracy = svmpredict("temp_test", s);
			if (accuracy > maxAccuracy)
			{
				maxAccuracy = accuracy;
				strcpy(bestModel, s);
			}
		}

		accuracyTrain = svmpredict(testFile, bestModel);
		if (accuracyTrain > maxAccuracyOnTest)
		{
			maxAccuracyOnTest = accuracyTrain;
			strcpy(bestModelOnTest, bestModel);
			maxAccuracyOnTrain = maxAccuracy;
			strcpy(bestParameters, lineParameters);
		}

	}

	printf("best parameters:%s, accuracy on trainset: %lf, accuracy on testset: %lf", bestParameters, maxAccuracyOnTest, maxAccuracyOnTrain);

	fclose(parameters);
	return 0;
}

