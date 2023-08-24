#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "mpi.h"

#include "defines.h"
#include "consts.h"
#include "globals.h"
#include "utils.h"
#include "init.h"
#include "taskInit.h"
#include "admissionsPRead.h"

#include "drugRead.h"
#include "drugExtract.h"
#include "eventsRead.h"
#include "featureExtract.h"
#include "patientsPRead.h"
#ifdef MIMICIV
#include "icustaysExtractPRead.h"
#elif defined(MIMICIII)
#include "transfersPRead.h"
#include "noteRead.h"
#include "noteExtract.h"
#endif
#include "sInfoExtract.h"
#include "fextract.h"


// ID列表及数量
extern struct ID_node *HADM_IDs;
extern int HADM_IDs_size;

void sdk_init(int MPI_rank, int MPI_size);
void sdk_finish();
int start_extract(struct task task, struct extract_result **result);