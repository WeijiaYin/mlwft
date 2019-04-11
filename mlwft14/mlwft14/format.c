#include "stdafx.h"
#include "format.h"
#include "string.h"
#include "malloc.h"

int format(char *inputFile, char **outputFile)
{
	int g = 0;
	int p = 0;
	int i = 0;
	int h = 0;
	int flag = 0;
	int num = 0;
	int t = 0;
	char *result = NULL;
	char convert[1000][100];
	char line[1024];
	double e = 0;
	FILE *csvFile;
	FILE *svmFile;

	num = countLine(inputFile);
	csvFile = fopen(inputFile, "r");
	*outputFile = strtok(inputFile, ".");
	strcat(*outputFile, ".train");
	svmFile = fopen(*outputFile, "w");
	if (csvFile == NULL) {
		printf("Can not load file");
		exit(1);
	}

	fgets(line, sizeof(line), csvFile);
	result = strtok(line, ",");

	for (t = 1; t< num; t++)
	{
		i = 1;
		fgets(line, sizeof(line), csvFile);
		if (line == '\n')
		{
			printf("there can't be blank line in file");
			exit(1);
		}
		result = strtok(line, ",");
		for (h = 0; h < p; h++)
		{
			if (strcmp(convert[h], result) == 0)
			{
				flag = 1;
				g = h;
			}
		}
		if (flag == 1)
			flag = 0;
		else
		{
			strcpy(convert[p], result);
			g = p;
			p++;
		}
		fprintf(svmFile, "%d ", g);
		while (result != NULL)
		{
			result = strtok(NULL, ",");
			if (result != NULL) {
				sscanf(result, "%lf", &e);
				if (e != 0)
					fprintf(svmFile, " %d:%s", i, result);
			}
			i++;
		}
	}
	fclose(csvFile);
	fclose(svmFile);
	return 0;
}

int split(char *inputFile, char **trainFile, char **testFile, double percent)
{
	int lineNumber;
	int part;
	FILE *svmFile;
	FILE *svmFile_train;
	FILE *svmFile_test;
	int i = 0;
	char line[1024];
	char train[100];
	char test[100];

	lineNumber = countLine(inputFile);
	part = percent * lineNumber;
	svmFile = fopen(inputFile, "r");
	strcpy(train, strtok(inputFile, "."));
	strcpy(test, strtok(inputFile, "."));
	*trainFile = train;
	strcat(*trainFile, "_split.train");
	*testFile = test;
	strcat(*testFile, "_split.test");
	svmFile_train = fopen(*trainFile, "w");
	svmFile_test = fopen(*testFile, "w");
	fgets(line, sizeof(line), svmFile);
	for (i = 0; i < lineNumber; i++)
	{
		if (i < part)
			fprintf(svmFile_train, "%s", line);
		else
		{
			fprintf(svmFile_test, "%s", line);
		}
		fgets(line, sizeof(line), svmFile);
	}

	fclose(svmFile);
	fclose(svmFile_test);
	fclose(svmFile_train);
	return 0;
}

int countLine(char *inputFile)
{
	FILE * fp = NULL; 
	int c, lc = 0; 
	int line = 0; 
	fp = fopen(inputFile, "r+");
	if (fp == NULL)
	{
		printf("There is no file:%s", inputFile);
		exit(1);
	}
	while ((c = fgetc(fp)) != EOF) 
	{
		if (c == '\n') line++; 
		lc = c; 
	}
	if (lc != '\n')
	{
		line++;
		fprintf(fp, "%c", '\n');
	}
	fclose(fp); 
	return line;
}

int shuffleClass(char *input, char *output)
{
	FILE *inputFile;
	FILE *outputFile;
	char line[1024];
	char temp[1024];
	char outputFileName[50];
	char **lines = NULL;
	int num = 0;
	int i = 0;
	int j = 0;
	int t = 0;
	int rnd = 0;

	num = countLine(input);
	lines = (char **)malloc(num * sizeof(char*));
	inputFile = fopen(input, "r");
	if (inputFile == NULL)
	{
		printf("Can't open file");
	}
	outputFile = fopen(output, "w");
	fgets(line, sizeof(line), inputFile);
	for (i = 0; i < num; i++)
	{
		lines[i] = (char *)malloc(sizeof(line) * sizeof(char));
		strcpy(lines[i], line);
		fgets(line, sizeof(line), inputFile);
	}
	srand(time(NULL));
	for (j = 0; j < num; j++)
	{
		rnd = rand() % num;
		strcpy(temp, lines[j]);
		strcpy(lines[j], lines[rnd]);
		strcpy(lines[rnd], temp);
	}

	for (t = 0; t < num; t++)
	{
		fprintf(outputFile, "%s", lines[t]);
		free(lines[t]);
	}
	
	fclose(inputFile);
	fclose(outputFile);
	free(lines);
	return num;
}
