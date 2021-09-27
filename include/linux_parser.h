#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Filters for search
const std::string filterProcesses("processes");
const std::string filterRunningProcesses("procs_running");
const std::string filterMemTotalString("MemTotal:");
const std::string filterMemFreeString("MemFree:");
const std::string filterUID("Uid:");
const std::string filterProcMem("VmRSS:");

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
// Process states
enum ProccessStates {
  pUtime_ = 13,
  pStime_,
  pCuTime_,
  pCsTime_,
  pStartTime_ = 21
};

std::vector<std::string> CpuUtilization();
long Jiffies(const std::vector<std::string>& stats);
long ActiveJiffies(const std::vector<std::string>& stats);
long IdleJiffies(const std::vector<std::string>& stats);

// Processes
std::string Command(int pid);
float Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
std::vector<std::string> ReadProcStats(int pid);
long ProcessActiveJiffies(const std::vector<std::string>& stats);
};  // namespace LinuxParser

// helper templates

// finds a value of type T in a file wiht line starting
// with the given keyFilter string
template <typename T>
T findValueByKey(std::string const& keyFilter, std::string const& filename) {
  std::string line, key;
  T value;

  std::ifstream stream(filename);
  if (stream) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

// Find a value of type T in a file starting with
// the value (without) key
template <typename T>
T getValueFromFile(std::string const& filename) {
  std::string line;
  T value;

  std::ifstream stream(filename);
  if (stream) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};
#endif