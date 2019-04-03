#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "svm.h"


int searchMethod()
{
	HINSTANCE hLib = LoadLibrary(_T("libsvm.dll"));
	if ((unsigned)hLib <= HINSTANCE_ERROR) {
		return 1;
	}

	svm_train = (Svm_train)GetProcAddress(hLib, "svm_train");
	svm_cross_validation = (Svm_cross_validation)GetProcAddress(hLib, "svm_cross_validation");
	svm_save_model = (Svm_save_model)GetProcAddress(hLib, "svm_save_model");
	svm_load_model = (Svm_load_model)GetProcAddress(hLib, "svm_load_model");

	svm_get_svm_type = (Svm_get_svm_type)GetProcAddress(hLib, "svm_get_svm_type");
	svm_get_nr_class = (Svm_get_nr_class)GetProcAddress(hLib, "svm_get_nr_class");
	svm_get_labels = (Svm_get_labels)GetProcAddress(hLib, "svm_get_labels");
	svm_get_sv_indices = (Svm_get_sv_indices)GetProcAddress(hLib, "svm_get_sv_indices");
	svm_get_nr_sv = (Svm_get_nr_sv)GetProcAddress(hLib, "svm_get_nr_sv");
	svm_get_svr_probability = (Svm_get_svr_probability)GetProcAddress(hLib, "svm_get_svr_probability");

	svm_predict_values = (Svm_predict_values)GetProcAddress(hLib, "svm_predict_values");
	svm_predict = (Svm_predict)GetProcAddress(hLib, "svm_predict");
	svm_predict_probability = (Svm_predict_probability)GetProcAddress(hLib, "svm_predict_probability");

	svm_free_model_content = (Svm_free_model_content)GetProcAddress(hLib, "svm_free_model_content");
	svm_free_and_destroy_model = (Svm_free_and_destroy_model)GetProcAddress(hLib, "svm_free_and_destroy_model");
	svm_destroy_param = (Svm_destroy_param)GetProcAddress(hLib, "svm_destroy_param");

	svm_check_parameter = (Svm_check_parameter)GetProcAddress(hLib, "svm_check_parameter");
	svm_check_probability_model = (Svm_check_probability_model)GetProcAddress(hLib, "svm_check_probability_model");

	svm_set_print_string_function = (Svm_set_print_string_function)GetProcAddress(hLib, "svm_set_print_string_function");

	return 0;
}

