#include "mpi.h"
#include "sdkinit.h"

int main()
{
    int MPI_rank, MPI_size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_size);

    sdk_init(MPI_rank, MPI_size);

    // init task struct
    struct task new_task;
    new_task.age = 1;
    new_task.gender = 1;
    new_task.Unit1 = 1;
    new_task.Unit2 = 1;
    new_task.HospAdmTime = 1;
    new_task.drug = 1;
    new_task.note = 1;
    new_task.start_time = 0;
    // set ICD code list
    const char *icd_codes[] = {"99984", "99989", "9999", "E0000", "E0008", "E0009", "E0010", "E0011"};
    new_task.ICD_list_size = 8;
    // set item ids
    new_task.feature_ids_size = 2;
    new_task.feature_ids = (struct Feature_ID *)malloc(new_task.feature_ids_size * sizeof(struct Feature_ID));
    memset(new_task.feature_ids, -1, new_task.feature_ids_size * sizeof(struct Feature_ID));
    strcpy(new_task.feature_ids[0].name, "HR");
    new_task.feature_ids[0].ITEMID[0] = 211;
    new_task.feature_ids[0].ITEMID[1] = 220045;
    strcpy(new_task.feature_ids[1].name, "O2Sat");
    new_task.feature_ids[1].ITEMID[0] = 50817;
    new_task.feature_ids[1].ITEMID[1] = 220277;
    task_init(&new_task, icd_codes);

    // start extraction
    struct extract_result *result;
    int result_size = 0;
    result_size = start_extract(new_task, &result);
    // process results
    printf("%d result\n", result_size);
    printf("first result HADM_ID: %d\n", result[0].HADM_ID);
    printf("first result data:\n");
    printf("age: %d, gender: %d, Unit1: %d, Unit2: %d, HospAdmiTime:%lld\n", result[0].sInfor.Age,
           result[0].sInfor.Gender, result[0].sInfor.Unit2, result[0].sInfor.Unit2, result[0].sInfor.HospAdmiTime);
    printf("data in hour 0:\n");
    int i = 0;
    for (i = 0; i < new_task.feature_ids_size; i++)
    {
        printf("%s: %f\n", new_task.feature_ids[i].name, result[0].features[0][i]);
    }
    printf("rank: %d finished\n", MPI_rank);

    // finish
    task_free(&new_task);
    MPI_Finalize();
    return 0;
}