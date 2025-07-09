#include "process.hpp"
#include <iostream>
#include <fstream>
#include <sstream> // NOTE: Header that provides string stream functionality
#include <string>
#include <sys/sysinfo.h>
#include <tuple>
#include <unistd.h> // unix standard header - gives access to POSIX operating system API , such as sleep, usleep, etc.
#include <vector>
#include <iomanip> // for std::setprecision 

struct CpuStats {
  long user, nice, system, idle;
};

struct MemStats {
  long available, total;
};

struct sysinfo getSysInfo() {
  struct sysinfo info;
  sysinfo(&info);
  return info;
}

CpuStats getCpuStats() {
  std::ifstream file("/proc/stat");
  std::string line;
  std::getline(file, line);

  // NOTE: std::istringstream helps parse strings as if they were input streams
  // eg: act like cin , so use >> to read from the parsed string
  // NOTE: std::ostringstream is used to read from a string as if it were a
  // stream eg: act like cout, so use << to write to the parsed string

  // NOTE: here streamBuffer (object of istringstream) calls the constructor of
  // std::istringstream, which takes a string as an argument and initializes the
  // stream with that string.
  std::istringstream streamBuffer(line);
  std::string cpu;

  CpuStats stats;
  // extracting the values from the streamBuffer stream
  streamBuffer >> cpu >> stats.user >> stats.nice >> stats.system >> stats.idle;
  return stats;
};

// NOTE: std::tuple is used to return multiple values from a function
std::tuple<double, double, double> calcCpuUsage(CpuStats before,
                                                CpuStats after) {
  double total = (after.user - before.user) + (after.nice - before.nice) +
                 (after.system - before.system) + (after.idle - before.idle);

  double idle = (after.idle - before.idle);
  double used = total - idle;

  double percentage = ((used / total) * 100.0);
  return {percentage, total, used};
}

// WARN: Memory stats using /proc/meminfo
//
// MemStats getMemStatus() {
//     std::ifstream file("/proc/meminfo");
//     std::string line;
//
//     MemStats mem;
//     bool totalFound = false;
//     bool availableFound = false;
//
//     while (std::getline(file, line)) {
//         if (line.find("MemTotal:") != std::string::npos) {
//             std::istringstream streamBuffer(line);
//             std::string ignore;
//             streamBuffer >> ignore >> mem.total;
//             mem.total /= 1024; // Convert KB to MB
//             totalFound = true;
//         }
//
//         if (line.find("MemAvailable:") != std::string::npos) {
//             std::istringstream streamBuffer(line);
//             std::string ignore;
//             streamBuffer >> ignore >> mem.available;
//             mem.available /= 1024; // Convert KB to MB
//             availableFound = true;
//         }
//
//         // Exit early if both values are found
//         if (totalFound && availableFound) {
//             break;
//         }
//     }
//
//     return mem;
// }

MemStats getMemStatus() {
  struct MemStats mem;
  mem.available = getSysInfo().freeram / (1024 * 1024); // Convert bytes to MB
  mem.total = getSysInfo().totalram / (1024 * 1024);    // Convert bytes to MB
  return mem;
}

std::tuple<int, int> getUptime() {

  // WARN: Uptime using /proc/uptime
  // std::ifstream file("/proc/uptime");
  // long uptimeSeconds, idleSeconds;
  // file >> uptimeSeconds >> idleSeconds;
  // return uptimeSeconds;

  // NOTE: get uptime using sysinfo() system call
  long uptimeSeconds = getSysInfo().uptime; // returns the uptime in seconds
  int hours = uptimeSeconds / (60 * 60);
  int minutes = ((int)uptimeSeconds - hours * 60 * 60) / (60);
  return {hours, minutes};
}

std::vector<double> getLoadAvg() {
  // NOTE: The kernel stores the load averages in a format that uses 16 bits for the fractional part , so to convert them to readable numbers like 2.63, you must divide by 2^16 i.e.65536.0
    struct sysinfo info = getSysInfo();
    std::vector<double> loadAvg;
    for (int i = 0; i < 3; ++i) {
        loadAvg.push_back(info.loads[i]/65536.0);
    }
    return loadAvg;

  // WARN: Alternatively, you can read from /proc/loadavg

  // std::ifstream file("/proc/loadavg");
  // std::vector<double> loadAvg(3);
  // file >> loadAvg[0] >> loadAvg[1] >> loadAvg[2];
  // return loadAvg;
}

std::tuple<int,int,int> processInfo() {
  std::ifstream file("/proc/loadavg");
  float val1, val2, val3;
  std::string processValues;
  int val4,val5,val6;
  file >> val1 >> val2 >> val3 >> processValues >> val6;

  std::istringstream streamBuffer(processValues);
  char slash;
  // NOTE: when reading the input stream string with >> , it uses delimiter based on the variable you're reading into (eg: here slash is a char , so it reads the input stream `streamBuffer` containing string `processValues` of format 3/123 and seperates the two int values using the slash which acts as a delimiter since it's a char and we do >> char >>)
  streamBuffer >> val4 >> slash >> val5 ; 
  return {val4,val5,val6};
}

int main() {
  while (true) {
    CpuStats before, after;
    before = getCpuStats();
    sleep(1);
    after = getCpuStats();

    MemStats mem = getMemStatus();
    // NOTE: auto is used to deduce the type of the variables from the return
    // type of the function
    auto [percentage, total, used] = calcCpuUsage(before, after);

    auto [hours, minutes] = getUptime();

    // Get Load Averages
    std::vector<double> loadAverages=getLoadAvg();

    // Process related information 
    auto [ runningProcessCount, totalProcesses , recentProcessPID ] = processInfo();

    std::cout << "\033[2J\033[H"; // Ansi escape code to clear the screen and
                                  // move cursor to top left position
    std::cout << "++++++ BASIC SYSTEM MONITOR ++++++ " << std::endl
              << "CPU Usage: " << percentage << "%\n"
              << "Memory Usage: " << (mem.total - mem.available) << " / "
              << mem.total << " MB\n"
              << "Uptime: " << hours << " hours " << minutes << " minutes" << std::endl
              << "Most recent process PID:" << recentProcessPID << std::endl
              << "Number of processes since boot: " << totalProcesses << std::endl
              << "Currently running processes: " << runningProcessCount << std::endl 
              << "Load Average: " << std::fixed << std::setprecision(2) << loadAverages[0] << " , " << loadAverages[1] << " , " << loadAverages[2] << std::endl // setprecision to set total significant digits to 2
              << std::endl
              << "-------------------------------------" << std::endl
              << "-------------------------------------" << std::endl;
                       // for the buffer to fill up itself by auto.
    printProcesses(30);  // Show top 10 processes
    usleep(500000); // use nanosleep/usleep than sleep which only works for whole seconds
    std::cout.flush(); // Make sure output appears immediately , without waiting
  }

  return 0;
}
