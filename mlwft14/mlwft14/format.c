#include "stdafx.h"
#include "format.h"
#include "string.h"
#include "malloc.h"

int format(char *inputFile, char **outputFile)
{
	int g;
	int p = 0;
	int i = 0;
	int flag = 0;
	char *result = NULL;
	char *results[1000];
	char convert[100][100];
	char line[10000];
	double e = 0;

	FILE *csvFile = fopen(inputFile, "r");
	*outputFile = strtok(inputFile, ".");
	strcat(*outputFile, ".train");
	FILE *svmFile = fopen(*outputFile, "w");
	if (csvFile == NULL) {
		printf("Can not load file");
		return 1;
	}

	fgets(line, 10000, csvFile);
	result = strtok(line, ",");
	while (result != NULL)
	{
		result = strtok(NULL, ",");
		results[i] = result;
		i++;
	}

	while (!feof(csvFile))
	{
		i = 1;
		fgets(line, 1000, csvFile);
		result = strtok(line, ",");
		for (int h = 0; h < p; h++)
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
	char line[10000];
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
	fgets(line, 10000, svmFile);
	while (!feof(svmFile))
	{
		if (i < part)
			fprintf(svmFile_train, "%s", line);
		else
		{
			fprintf(svmFile_test, "%s", line);
		}
		i++;
		fgets(line, 10000, svmFile);
	}
	if (feof(svmFile))
	{
		fgets(line, 10000, svmFile);
		fprintf(svmFile_test, "%s", line);
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
	fp = fopen(inputFile, "r");
	while ((c = fgetc(fp)) != EOF) 
	{
		if (c == '\n') line++; 
		lc = c; 
	}
	fclose(fp); 
	if (lc != '\n') line++;
	return line;
}


typedef struct node
{
	char data[100000];
	int class;
	struct node *pNext;        
}Node, *pNode;


pNode CreateList(char *inputFile, int *i)
{
	char class[10000];
	char line[10000];
	FILE *f = fopen(inputFile, "r");
	fgets(line, sizeof(line), f);
	strcpy(class, line);
	strtok(class, " ");
	pNode pHead = (pNode)malloc(sizeof(Node));
	strcpy(pHead->data, line);
	pHead -> class = atoi(class);
	pHead -> pNext = NULL;
	pNode pTail = pHead;
	*i = *i + 1;;

	while (!feof(f))
	{
		fgets(line, sizeof(line), f);
		strcpy(class, line);
		strtok(class, " ");
		pNode pNew = (pNode)malloc(sizeof(Node));
		strcpy(pNew->data, line);
		pNew -> class = atoi(class);
		pTail->pNext = pNew;
		pNew->pNext = NULL;
		pTail = pNew;
		*i = *i +1;
	}
	pTail->pNext = pHead;
	fclose(f);
	return pTail;
}

/*int seperateClass(char *input, char *output)
{
	int k = 0;
	int i = 0;
	int class;
	pNode p;
	pNode temp;
	pNode pTail = NULL;            
	pTail = CreateList(input, &i);
	FILE *outputFile = fopen(output, "w");
	fprintf(outputFile, "%s", pTail->pNext->data);
	class = pTail->pNext->class;
	temp = pTail->pNext->pNext;
	pTail->pNext = temp;
	p = pTail;
	while (p->pNext != p)
	{
		for (int j = i-1; j > 0; j--) {
			if (p->pNext->class != class)
			{
				fprintf(outputFile, "%s", p->pNext->data);
				class = p->pNext->class;
				temp = p->pNext->pNext;
				p->pNext = temp;
				k++;
			}
			else
				p = p->pNext;
			}
		if (k != 0)
		{
			i = i - k;
			k = 0;
		}
		else
		{
			for (int t = 0; t < i-1; t++)
			{
				fprintf(outputFile, "%s", p->pNext->data);
				temp = p->pNext->pNext;
				p->pNext = temp;
				p = p->pNext;
			}
		}
	}
	fclose(outputFile);
	return 0;
}*/


int seperateClass(char *input, char *output)
{
	FILE *inputFile;
	FILE *outputFile;
	char line[100000];
	char lines[500][1000];
	int i = 0;

	inputFile = fopen(input, "r");
	outputFile = fopen(output, "w");
	fgets(line, sizeof(line), inputFile);
	while (!feof(inputFile))
	{
		strcpy(lines[i], line);
		fgets(line, sizeof(line), inputFile);
		i++;
	}
	srand(time(NULL));
	char temp[10000];
	for (int j = 0; j < i; j++)
	{
		int rnd = rand() % i;
		strcpy(temp, lines[j]);
		strcpy(lines[j], lines[rnd]);
		strcpy(lines[rnd], temp);
	}

	for (int t = 0; t < i; t++)
	{
		fprintf(outputFile, "%s", lines[t]);
	}
	
	fclose(inputFile);
	fclose(outputFile);
	return 0;
}
