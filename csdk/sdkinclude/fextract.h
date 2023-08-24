#include <stdarg.h>
#include "defines.h"
#include "sInfoExtract.h"
#include "taskInit.h"
#include "featureExtract.h"
#include "noteExtract.h"

struct extract_result
{
    int HADM_ID;
    double **features;
    struct StaticInformation sInfor;
    char **drugs;
    char **Strategy;
    char **GSN;
    char **NDC;
    char **Note;
};

void extract(int HADM_ID, int SUBJECT_ID, struct task e_task, struct extract_result *ext_result);
