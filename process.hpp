#pragma once
#include <sstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <iomanip>
#include <algorithm>
#include <map>
#include <ctime>



struct Process {
    int pid;
    std::string name;
    double cpu_percent;
    long mem_usage_kb;
};

std::vector<int> getAllPids() ;
Process getProcessInfo(int pid) ;

bool compareCpu(const Process& a, const Process& b) ;


void printProcesses(int num = 10) ;
