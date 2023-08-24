#pragma once
#include "mpi.h"
#include <utils.h>
#include <stdlib.h>
#include <defines.h>
#include "featureExtract.h"
#include "icdCodes.h"
#include "idExtract.h"

struct task
{
    int task_id;
    struct Feature_ID *feature_ids;
    int feature_ids_size;
    int *ICD_list;
    int ICD_list_size;
    // 是否输出对应数据, bool
    int age;
    int gender;
    int Unit1;
    int Unit2;
    int HospAdmTime;
    int ICULOS;
    int drug;
    int note;
    // 提取开始时间, 为0表示从第一条记录开始, 为1表示从入院时间开始, 2表示从二者最小值中选择
    int start_time;
};

/**
 * 特征列表读取
 */
void feature_read(struct task *tar);

/**
 * 任务完成时释放任务
 */
void task_free(struct task *target);

/**
 * 初始化任务
 */
void task_init(struct task *tar, const char *icd_codes[]);