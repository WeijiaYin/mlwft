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


int kcrossvalidation(int l, int k, char *inputfile, char *outputFolder)
{
	int count = countLine(inputfile);
	int number = count / k;
	//	FILE *tf = fopen(inputfile, "r");
	FILE *p;
	FILE *p1;
	char *bestModel;
	double accuracy;
	double maxAccuracy = 0.0;

	for (int i = 1; i <= k; i++)
	{
		FILE *tf = fopen(inputfile, "r");
		strcpy(splitname, filename);
		itoa(l, numbertochar1, 10);
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
		svmTrain("temp_train", s, "adsasdsa");
		accuracy = svmpredict("temp_test", s);
		if (accuracy > maxAccuracy)
		{
			maxAccuracy = accuracy;
			bestModel = s;
		}
		fclose(p);
		fclose(p1);
		fclose(tf);
	}
	return 0;
}

/*int splitTrainFile(int k, char *inputfile, char *outputFolder)
{
	int count = countLine(inputfile);
	int number = count / k;
	FILE *tf = fopen(inputfile, "r");
	FILE *p;
	for (int i = 1; i <= k; i++)
	{
		strcpy(splitname,filename);
		itoa(i, numbertochar, 10);
		strcat(splitname , numbertochar);
		char *s = splitname;
		p = fopen(s, "w");
		int a = i * number;
		while (ln <= a)
		{
			fgets(line, 10000, tf);
			ln++;
			fprintf(p, "%s", line);
		}
		fclose(p);
	}
	fclose(tf);
	return 0;
}

int trainTrainFile(int k, char *folderPath, char *outputModel)
{

	int j = 0;
	char fileNames[10][10];
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;
	char p[700] = { 0 };
	strcpy(p, folderPath);
	strcat(p, "\\*");
	if ((hFile = _findfirst(p, &fileInfo)) != -1)
	{
		do
		{
			strcpy(fileNames[j], fileInfo.name);
			j++;
		} while (_findnext(hFile, &fileInfo) == 0);
		_findclose(hFile);
	}

	for (int i = 0; i < k; i++)
	{

	}
	
	

}
*/

