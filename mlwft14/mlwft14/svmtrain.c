#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "svm.h"
#include "svmtrain.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

static void print_null(const char *s) {}

static void exit_with_help()
{
	printf(
		"Usage: svm-train [options] training_set_file [model_file]\n"
		"options:\n"
		"-s svm_type : set type of SVM (default 0)\n"
		"	0 -- C-SVC		(multi-class classification)\n"
		"	1 -- nu-SVC		(multi-class classification)\n"
		"	2 -- one-class SVM\n"
		"	3 -- epsilon-SVR	(regression)\n"
		"	4 -- nu-SVR		(regression)\n"
		"-t kernel_type : set type of kernel function (default 2)\n"
		"	0 -- linear: u'*v\n"
		"	1 -- polynomial: (gamma*u'*v + coef0)^degree\n"
		"	2 -- radial basis function: exp(-gamma*|u-v|^2)\n"
		"	3 -- sigmoid: tanh(gamma*u'*v + coef0)\n"
		"	4 -- precomputed kernel (kernel values in training_set_file)\n"
		"-d degree : set degree in kernel function (default 3)\n"
		"-g gamma : set gamma in kernel function (default 1/num_features)\n"
		"-r coef0 : set coef0 in kernel function (default 0)\n"
		"-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)\n"
		"-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)\n"
		"-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)\n"
		"-m cachesize : set cache memory size in MB (default 100)\n"
		"-e epsilon : set tolerance of termination criterion (default 0.001)\n"
		"-h shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)\n"
		"-b probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)\n"
		"-wi weight : set the parameter C of class i to weight*C, for C-SVC (default 1)\n"
		"-v n: n-fold cross validation mode\n"
		"-q : quiet mode (no outputs)\n"
	);
	exit(1);
}

static void exit_input_error(int line_num)
{
	fprintf(stderr, "Wrong input format at line %d\n", line_num);
	exit(1);
}

char *parse_command_line(/*int argc, char **argv,*/ char *parameters/*, char *model_file_name*/);
void read_problem(const char *filename);

struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *model;
struct svm_node *x_space;
int nr_fold;

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input)
{
	int len;

	if (fgets(line, max_line_len, input) == NULL)
		return NULL;

	while (strrchr(line, '\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *)realloc(line, max_line_len);
		len = (int)strlen(line);
		if (fgets(line + len, max_line_len - len, input) == NULL)
			break;
	}
	return line;
}

char* svmTrain(char *input_file_name, char *model_file_name, char *parameters)
{

	const char *error_msg;
	char command1[1024];
	char *command;
	command = parse_command_line(parameters);
	strcpy(command1, command);
	read_problem(input_file_name);
	error_msg = svm_check_parameter(&prob, &param);

	if (error_msg)
	{
		fprintf(stderr, "ERROR: %s\n", error_msg);
		exit(1);
	}
	
	model = svm_train(&prob, &param);
	if (svm_save_model(model_file_name, model))
	{
		fprintf(stderr, "can't save model to file %s\n", model_file_name);
		exit(1);
	}
	svm_free_and_destroy_model(&model);

	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
	free(x_space);
	free(line);

	return command1;
}


char *parse_command_line(char *parameters)
{
	int offset = 0;
	char command[1024];
	int i = 0;
	char pr[100];
	char *results[1024];
	char *result;
	char *result1;
	void(*print_func)(const char*) = NULL;	// default printing to stdout

											// default values
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 1;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	strcpy(pr, parameters);
	result = strtok(pr, " ");
	while (result != NULL)
	{
		printf("%s\n", result);
		results[i] = result;
		i++;
		result = strtok(NULL, " ");
	}
	// parse options
	for (int j = 0; j<i; j++)
	{
		result1 = strtok(results[j], "=");
		switch (result1[0])
		{
		case 's':
			param.svm_type = atoi(strtok(NULL, "="));
			break;
		case 't':
			param.kernel_type = atoi(strtok(NULL, "="));
			break;
		case 'd':
			param.degree = atoi(strtok(NULL, "="));
			break;
		case 'g':
			param.gamma = atof(strtok(NULL, "="));
			break;
		case 'r':
			param.coef0 = atof(strtok(NULL, "="));
			break;
		case 'n':
			param.nu = atof(strtok(NULL, "="));
			break;
		case 'm':
			param.cache_size = atof(strtok(NULL, "="));
			break;
		case 'c':
			param.C = atof(strtok(NULL, "="));
			break;
		case 'e':
			param.eps = atof(strtok(NULL, "="));
			break;
		case 'p':
			param.p = atof(strtok(NULL, "="));
			break;
		case 'h':
			param.shrinking = atoi(strtok(NULL, "="));
			break;
		case 'b':
			param.probability = atoi(strtok(NULL, "="));
			break;
		case 'q':
			print_func = &print_null;
			i--;
			break;
		case 'w':
			++param.nr_weight;
			param.weight_label = (int *)realloc(param.weight_label, sizeof(int)*param.nr_weight);
			param.weight = (double *)realloc(param.weight, sizeof(double)*param.nr_weight);
			param.weight_label[param.nr_weight - 1] = atoi(&result1[1]);
			param.weight[param.nr_weight - 1] = atof(strtok(NULL, "="));
			break;
		default:
			fprintf(stderr, "Unknown option: -%c\n", result1[0]);
			exit_with_help();
		}
	}
	offset += sprintf(command+offset, "svm_type:%d, kernel_type:%d, degree:%d, gamma:%lf, coef0:%lf, cache_size:%lf, eps:%lf, c:%lf, nr_weight:%d ", param.svm_type, param.kernel_type, param.degree, param.gamma, param.coef0, param.cache_size, param.eps, param.C, param.nr_weight);

	for (int f = 0; f < param.nr_weight; f++)
	{
		offset += sprintf(command + offset, "weight_lable%d:%d, weight%d:%lf", f-1, param.weight_label[f-1], f, param.weight[f-1]);
	}

	offset += sprintf(command + offset, "nu:%lf, p:%lf, shrinking:%d, probability:%d", param.nu, param.p, param.shrinking, param.probability);

	svm_set_print_string_function(print_func);
	
	return command;
}

// read in a problem (in svmlight format)

void read_problem(const char *filename)
{
	int max_index, inst_max_index, i;
	size_t elements, j;
	FILE *fp = fopen(filename, "r");
	char *endptr;
	char *idx, *val, *label;

	if (fp == NULL)
	{
		fprintf(stderr, "can't open input file %s\n", filename);
		exit(1);
	}

	prob.l = 0;
	elements = 0;

	max_line_len = 1024;
	line = Malloc(char, max_line_len);
	while (readline(fp) != NULL)
	{
		char *p = strtok(line, " \t"); // label

									   // features
		while (1)
		{
			p = strtok(NULL, " \t");
			if (p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
				break;
			++elements;
		}
		++elements;
		++prob.l;
	}
	rewind(fp);

	prob.y = Malloc(double, prob.l);
	prob.x = Malloc(struct svm_node *, prob.l);
	x_space = Malloc(struct svm_node, elements);

	max_index = 0;
	j = 0;
	for (i = 0; i<prob.l; i++)
	{
		inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
		readline(fp);
		prob.x[i] = &x_space[j];
		label = strtok(line, " \t\n");
		if (label == NULL) // empty line
			exit_input_error(i + 1);

		prob.y[i] = strtod(label, &endptr);
		if (endptr == label || *endptr != '\0')
			exit_input_error(i + 1);

		while (1)
		{
			idx = strtok(NULL, ":");
			val = strtok(NULL, " \t");

			if (val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int)strtol(idx, &endptr, 10);
			if (endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
				exit_input_error(i + 1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val, &endptr);
			if (endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(i + 1);

			++j;
		}

		if (inst_max_index > max_index)
			max_index = inst_max_index;
		x_space[j++].index = -1;
	}

	if (param.gamma == 0 && max_index > 0)
		param.gamma = 1.0 / max_index;

	if (param.kernel_type == PRECOMPUTED)
		for (i = 0; i<prob.l; i++)
		{
			if (prob.x[i][0].index != 0)
			{
				fprintf(stderr, "Wrong input format: first column must be 0:sample_serial_number\n");
				exit(1);
			}
			if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
			{
				fprintf(stderr, "Wrong input format: sample_serial_number out of range\n");
				exit(1);
			}
		}

	fclose(fp);
}
