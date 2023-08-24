#include "fextract.h"

/**
 * 根据两个ID进行提取
 */
void extract(int HADM_ID, int SUBJECT_ID, struct task e_task, struct extract_result *ext_result)
{
    // 病人基本信息
    struct StaticInformation sInfor;
    sInfor.begintime = 0;
    sInfor.endtime = 0;
    sInfo_extract(&sInfor, HADM_ID, SUBJECT_ID);
#ifdef MIMICIII
    // 医嘱信息
    struct NOTE *notes;
    int notes_size = 0;
    if (e_task.note)
    {
        note_extract(HADM_ID, &notes, &notes_size);
    }
#endif
    // 药物信息
    struct DRUG *drugs = NULL;
    int drugs_size = 0;
    if (e_task.drug)
    {
        drug_extract(HADM_ID, &drugs, &drugs_size, &sInfor);
    }
    // 化验信息
    struct Feature *features;
    int features_size = 0;
    feature_extract(HADM_ID, SUBJECT_ID, &features_size, &features, e_task.feature_ids, e_task.feature_ids_size, &sInfor);

    // 确定开始时间, 此时sInfor.HospAdmiTime中存储的是入院时间
    switch (e_task.start_time)
    {
    case 0:
        break;
    case 1:
        sInfor.begintime = sInfor.HospAdmiTime;
        break;
    case 2:
        if (sInfor.HospAdmiTime < sInfor.begintime)
        {
            sInfor.begintime = sInfor.HospAdmiTime;
        }
        break;
    default:
        break;
    }
    // 计算住院时长时长
    sInfor.HospAdmiTime = sInfor.endtime / 3600 - sInfor.begintime / 3600;

    int i = 0, j = 0, k = 0;
    (*ext_result).sInfor = sInfor;
    (*ext_result).features = (double **)malloc(sizeof(double **) * sInfor.HospAdmiTime);
    if (e_task.note)
        (*ext_result).Note = (char **)malloc(sizeof(char **) * sInfor.HospAdmiTime);
    if (e_task.drug)
    {
        (*ext_result).Strategy = (char **)malloc(sizeof(char **) * sInfor.HospAdmiTime);
        (*ext_result).GSN = (char **)malloc(sizeof(char **) * sInfor.HospAdmiTime);
        (*ext_result).NDC = (char **)malloc(sizeof(char **) * sInfor.HospAdmiTime);
    }

    // 从开始时间到结束时间遍历
    int drug_index = 0, note_index = 0, feature_index = 0;
    for (i = 0; i < sInfor.HospAdmiTime; i++)
    {
        long long ctime = sInfor.begintime / 3600 + (long long)i; // 当前时间(小时)
        //----------------------------------------------------------------------------------------
        // 遍历给定的特征ID表

        // 暂存每个特征数据查询到的数据的总和, 用于求均值
        (*ext_result).features[i] = (double *)malloc(e_task.feature_ids_size * sizeof(double));
        int *feature_data_counter = (int *)malloc(e_task.feature_ids_size * sizeof(int));
        memset((*ext_result).features[i], 0, e_task.feature_ids_size * sizeof(double));
        memset(feature_data_counter, 0, e_task.feature_ids_size * sizeof(int));
        for (j = 0; j < e_task.feature_ids_size; j++)
        {
            // 遍历当前时间点的特征数据, 找到匹配的第一个数据
            for (k = feature_index; k < features_size && features[k].time / 3600 <= ctime; k++)
            {
                // 判断当前数据是否是指定itemID(有多个对应ID)的数据
                if (is_in_array(features[k].ITEMID, e_task.feature_ids[j].ITEMID) && features[k].time / 3600 == ctime)
                {
                    (*ext_result).features[i][j] += features[k].value;
                    feature_data_counter[j] += 1;
                }
            }
        }
        // 输出
        for (j = 0; j < e_task.feature_ids_size; j++)
            if (feature_data_counter[j] != 0)
                (*ext_result).features[i][j] /= (double)feature_data_counter[j];
        free(feature_data_counter);
        //----------------------------------------------------------------------------------------
        // 移动指针到下一个时间点的数据
        while (feature_index < features_size && features[feature_index].time / 3600 <= ctime)
            feature_index++;
        // 输出药品信息
        if (e_task.drug)
        {
            char *Strategy = malloc(1024 * 1024);
            Strategy[0] = '\0';
            char *GSN = malloc(6 * 1000);
            GSN[0] = '\0';
            char *NDC = malloc(11 * 1000);
            NDC[0] = '\0';
            char trans_buff[32];
            for (drug_index = 0; drug_index < drugs_size; drug_index++)
            {
                if (drugs[drug_index].STARTDATE / 3600 <= ctime && drugs[drug_index].ENDDATE / 3600 >= ctime)
                {
                    if (strlen(Strategy) != 0)
                    {
                        strcat(Strategy, ",");
                        strcat(GSN, ",");
                        strcat(NDC, ",");
                    }
                    strcat(Strategy, drugs[drug_index].PROD_STRENGTH);
                    my_itoa(drugs[drug_index].GSN, trans_buff);
                    strcat(GSN, trans_buff);
                    sprintf(trans_buff, "%lld", drugs[drug_index].NDC);
                    strcat(NDC, trans_buff);
                }
            }
            (*ext_result).Strategy[i] = (char *)malloc(strlen(Strategy) + 1);
            strcpy((*ext_result).Strategy[i], Strategy);
            (*ext_result).GSN[i] = (char *)malloc(strlen(GSN) + 1);
            strcpy((*ext_result).GSN[i], GSN);
            (*ext_result).NDC[i] = (char *)malloc(strlen(NDC) + 1);
            strcpy((*ext_result).NDC[i], NDC);
            free(Strategy);
            free(GSN);
            free(NDC);
        }

#ifdef MIMICIII
        if (e_task.note)
        {
            // 输出医嘱信息
            if (notes[note_index].DATE / 3600 == ctime)
            {
                if (strlen(notes[note_index].TEXT) < 90 * 1024 * 1024)
                {
                    (*ext_result).Note[i] = (char *)malloc(strlen(notes[note_index].TEXT) + 1);
                    strcpy((*ext_result).Note[i], notes[note_index].TEXT);
                }
                if (note_index < notes_size)
                    note_index++;
            }
        }
#endif
    }

    // 释放
#ifdef MIMICIII
    if (e_task.note)
    {
        // 释放医嘱文本内存
        for (i = 0; i < notes_size; i++)
            free(notes[i].TEXT);
        free(notes);
    }
#endif
    if (e_task.drug)
    {
        if (drugs != NULL)
            free(drugs);
    }
    free(features);
}
