#pragma once

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <sched.h>

void sigsegv_handler(int sig) {
    //fprintf(stderr, "Another process is trying to read my memory. Exiting...\n");
    exit(EXIT_FAILURE);
}

void disablePTRACE() {
    // Disable debugging of this process
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
}

void changeProcessName() {
    // Set the process name to "systemd"
    prctl(PR_SET_NAME, "systemd", 0, 0, 0);
}

void protect() {
    
    //signal(SIGSEGV, sigsegv_handler);
    
    //disablePTRACE();
    
    //changeProcessName();
}
