#include "sdkinit.h"

struct ID_node *HADM_IDs;
int HADM_IDs_size = 0;

void sdk_init(int MPI_rank, int MPI_size)
{
    if (init(MPI_size) != 0)
    {
        MPI_Finalize();
        return;
    }
    // ID读取
    id_extract(&HADM_IDs, &HADM_IDs_size, MPI_rank, MPI_size);
    printf("rank:%d, %d\n", MPI_rank, HADM_IDs_size);

    // 读取表到内存中
    hash_table_len = ID_LEN;
    struct timeval t_read_btime, t_read_etime;
    ADMISSIONS_table_read();
    PATIENTS_table_read();
    PRESCRIPTION_table_read();
    FEATURES_TABLE = (struct EVENTS_DATA_TABLE *)malloc(hash_table_len * sizeof(struct EVENTS_DATA_TABLE));
    memset(FEATURES_TABLE, 0, hash_table_len * sizeof(struct EVENTS_DATA_TABLE));
    LABEVENTS_table_read();
    CHARTEVENTS_table_read();
    OUTPUTEVENTS_table_read();
#ifdef MIMICIV
    ICUSTAY_table_read();
#elif defined(MIMICIII)
    TRANSFER_table_read();
    NOTE_table_read();
#endif
}

void sdk_finish(struct task task, struct extract_result **result, int result_size)
{
}

int start_extract(struct task task, struct extract_result **result)
{
    int i, counter = 0;
    *result = (struct extract_result *)malloc(HADM_IDs_size * sizeof(struct extract_result));
    memset(*result, 0, HADM_IDs_size * sizeof(struct extract_result));
    for (i = 0; i < HADM_IDs_size; i++)
    {
        if (task.ICD_list_size == 0 || is_in_array(HADM_IDs[i].ICD_CODE, task.ICD_list) != 0)
        {
            extract(HADM_IDs[i].HADM_ID, HADM_IDs[i].SUBJECT_ID, task, &(*result)[counter]);
            (*result)[counter].HADM_ID = HADM_IDs[i].HADM_ID;
            counter += 1;
        }
    }
    return counter;
}