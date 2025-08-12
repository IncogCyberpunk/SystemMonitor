#include "process.hpp"
#include "colors.hpp"

#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <ctime>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/sysinfo.h>

std::vector<int> getAllPids() {
    std::vector<int> pids;
    DIR* dirPointer = opendir("/proc");

    if (dirPointer) {
        // NOTE: dirent is a structure that represents a directory entry
        struct dirent* entry; // entry is a pointer to a dirent structure
        // NOTE: we are looping over the directory using the while loop 
        while ((entry = readdir(dirPointer)) != nullptr) { // readdir retutrns a pointer representing a next directory entry and returns nullptr when there are no more entries
            std::string name = entry->d_name;
            // NOTE: `find_first_not_of` searches for the first character in the string that does not match any of the characters in the specified string (in this case, "0123456789"). If the search objective is satisified , it returns the index,else returns std::string::npos, which in this case means all characters are digits.  
            if (name.find_first_not_of("0123456789") == std::string::npos) { // std::string::npos is returned if the entry is only a number
                pids.push_back(std::stoi(name)); // push the PID (which is the directory name) into the vector 
            }
        }
        // Close the directory directory after finishing reading all entries
        closedir(dirPointer);
    }

    return pids;
}

// Read process info from /proc/<pid>/...
Process getProcessInfo(int pid) {
    Process p;
    p.pid = pid;

    // Read command name from /proc/<pid>/comm
    std::ifstream comm_file("/proc/" + std::to_string(pid) + "/comm");
    if (std::getline(comm_file, p.name)) // store the command name in p.name
        p.name.erase(p.name.find_last_not_of(" \n\r\t") + 1);// find the index of the last character in the p.name that is not one of ` \n\r\t` and erase everything after that, effectively trimming the string

    // Read CPU time from /proc/<pid>/stat
    std::ifstream stat_file("/proc/" + std::to_string(pid) + "/stat");
    std::string line;
    if (std::getline(stat_file, line)) {
        std::istringstream iss(line);
        std::string token;
        for (int i = 0; i < 13; ++i) iss >> token;  // skip to utime by dumping first 13 values in string token
    
        unsigned long utime, stime, cutime, cstime;
        iss >> utime >> stime >> cutime >> cstime;

        // NOTE: Static so that the values are preserved across function calls because we are calculating CPU Usage by keeping it in a infinite loop
        static long hz = sysconf(_SC_CLK_TCK); // store the no. of clock ticks per second
        static time_t last_time = time(nullptr); // Store the last time we calculated CPU usage , so that we can calculate the CPU percentage later

        // NOTE: Map is like a dictionary in python, it stores key-value pairs
        static std::map<int, unsigned long> last_cpu_total; // key is of type int (PID) and value is of type unsigned long (total CPU time) for the map & last_cpu_total is a map object
        
        unsigned long total_time = utime + stime + cutime + cstime;
        unsigned long delta_time = total_time - last_cpu_total[pid];
        last_cpu_total[pid] = total_time; // store key-value pair in the map
    
        double elapsed = difftime(time(nullptr), last_time);
        // FIX: Fix the way how CPU percentage is calculated and make it like the one in top command
        p.cpu_percent = (delta_time / (double)hz) / elapsed * 100.0;
    }

    // Read memory usage from /proc/<pid>/statm
    std::ifstream statm_file("/proc/" + std::to_string(pid) + "/statm");
    if (std::getline(statm_file, line)) {
        std::istringstream iss(line);
        long total_pages, resident_pages;
        iss >> total_pages >> resident_pages;
        p.mem_usage_kb = resident_pages * getpagesize() / 1024; // getpagesize() returns the size of a page in bytes, so we convert resident pages to KB
    }

    return p;
}

bool compareCpu(const Process& a, const Process& b) {
    return a.cpu_percent > b.cpu_percent;
}

void printProcesses(int num ) { // default arguments are only used in function declarations, not definitions
    auto pids = getAllPids();
    std::vector<Process> processes;

    for (int item : pids) { // iterate over all the PIDs
        processes.push_back(getProcessInfo(item));
    }

    std::sort(processes.begin(), processes.end(), compareCpu); // std::sort provided by <algorithm>, here sorts the vector of processes in descending order based on CPU usage
    std::cout << getColorCode("blue") // set the color to blue
              << "    Top " << num << " processes by CPU usage:" 
              << std::endl 
              << std::endl
              << getColorCode("yellow"); // reset the color to default
    std::cout<< "PID\tCPU%\tMEM (MB)\tCommand\n" << std::endl; 

    std::cout << getColorCode("reset"); // reset the color to default
    for (int i = 0; i < num && i < processes.size(); ++i) {
        const auto& p = processes[i];
        std::cout << p.pid << "\t"
                  << std::fixed << std::setprecision(1) << p.cpu_percent << "%\t"
                  << (p.mem_usage_kb / 1024.0) << "\t\t"
                  << p.name << "\n";
    }

    std::cout << std::endl;

}

