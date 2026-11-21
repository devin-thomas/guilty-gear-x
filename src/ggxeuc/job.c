/* Guilty Gear X US (SLUS-202.87): 4 functions ported from the X Plus
   decomp (ggxeuc/job.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/job.h"

void JobChange(int job) {
    JobMode = job;
}

void JobInit(void) {
    JobMode = 1;
}

void JobNext(void) {
    JobMode++;
}

void JobMain(void) {
    jobfunc[JobMode]();
}