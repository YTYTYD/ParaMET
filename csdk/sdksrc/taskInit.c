#include "taskInit.h"

void task_free(struct task *target)
{
    free(target->feature_ids);
    free(target->ICD_list);
}

void task_init(struct task *tar, const char *input_icd_codes[])
{
    tar->ICD_list = (int *)malloc((tar->ICD_list_size + 1) * sizeof(int));
    memset(tar->ICD_list, -1, (tar->ICD_list_size + 1) * sizeof(int));
    int i = 0;
    for (i = 0; i < tar->ICD_list_size; i++)
        tar->ICD_list[i] = icd_code_binary_search(icd_codes, 0, icd_code_nums, (char *)input_icd_codes[i]);
}