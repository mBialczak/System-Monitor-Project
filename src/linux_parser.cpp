#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>

using std::getline;
using std::ifstream;
using std::istringstream;
using std::regex;
using std::regex_match;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// Returns name of the operating system from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Reads Kernel data
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Returns the list of all processes pids
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  string filename;
  for (auto& entry : std::filesystem::directory_iterator(kProcDirectory)) {
    filename = entry.path().filename();
    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
      int pid = stoi(filename);
      pids.emplace_back(pid);
    }
  }
  return pids;
}

// Reads the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal{0.0};
  float MemFree{0.0};
  float result{0.0};
  string path{kProcDirectory + kMeminfoFilename};
  MemTotal = findValueByKey<float>(filterMemTotalString, path);
  MemFree = findValueByKey<float>(filterMemFreeString, path);
  if (MemTotal) result = (MemTotal - MemFree) / MemTotal;
  return result;
}
// Reads the system uptime
long LinuxParser::UpTime() {
  double seconds = getValueFromFile<double>(kProcDirectory + kUptimeFilename);
  return static_cast<long>(std::round(seconds));
}

// Reads the number of jiffies for the system
long LinuxParser::Jiffies(const vector<string>& stats) {
  return ActiveJiffies(stats) + IdleJiffies(stats);
}

//  Reads the number of active jiffies for a Process
long LinuxParser::ProcessActiveJiffies(const vector<string>& stats) {
  long uTime{};
  long sTime{};
  if (!stats.empty()) {
    uTime = std::stod(stats[pUtime_]);
    sTime = std::stod(stats[pStime_]);
  }
  return uTime + sTime;
}

// Reads the number of active jiffies for the system
long LinuxParser::ActiveJiffies(const vector<string>& stats) {
  long user{};
  long nice{};
  long system{};
  long irq{};
  long softirq{};
  long steal;
  if (!stats.empty()) {
    user = stol(stats[kUser_]);
    nice = stol(stats[kNice_]);
    system = stol(stats[kSystem_]);
    irq = stol(stats[kIRQ_]);
    softirq = stol(stats[kSoftIRQ_]);
    steal = stol(stats[kSteal_]);
  }
  return user + nice + system + irq + softirq + steal;
}

// Reads the number of idle jiffies for the system
long LinuxParser::IdleJiffies(const std::vector<string>& stats) {
  long idle{};
  long iowait{};
  if (!stats.empty()) {
    idle = stol(stats[kIdle_]);
    iowait = stol(stats[kIOwait_]);
  }
  return idle + iowait;
}

// Reads the CPU utilization statistics
vector<string> LinuxParser::CpuUtilization() {
  ifstream stream{kProcDirectory + kStatFilename};
  string line, key, value;
  vector<string> util_values;
  if (stream && getline(stream, line)) {
    istringstream line_stream(line);
    line_stream >> key;
    while (line_stream >> value) {
      util_values.emplace_back(value);
    }
  }
  return util_values;
}
// Reads process statistics
vector<string> LinuxParser::ReadProcStats(int pid) {
  ifstream stream{kProcDirectory + to_string(pid) + kStatFilename};
  string line, value;
  vector<string> stats;
  const int neededStats = 21;
  if (stream && getline(stream, line)) {
    istringstream line_stream(line);
    for (int i = 0; i <= neededStats; i++) {
      line_stream >> value;
      stats.emplace_back(value);
    }
  }
  return stats;
}

// Reads the total number of processes
int LinuxParser::TotalProcesses() {
  string path{kProcDirectory + kStatFilename};
  return findValueByKey<int>(filterProcesses, path);
  ;
}

// Reads the number of running processes
int LinuxParser::RunningProcesses() {
  string path{kProcDirectory + kStatFilename};
  return findValueByKey<int>(filterRunningProcesses, path);
}

// Reads the command associated with a process
string LinuxParser::Command(int pid) {
  return getValueFromFile<string>(kProcDirectory + to_string(pid) +
                                  kCmdlineFilename);
}

// Reads and return the memory used by a process
float LinuxParser::Ram(int pid) {
  string path{kProcDirectory + to_string(pid) + kStatusFilename};
  float ram = findValueByKey<float>(filterProcMem, path);
  return ram /= 1024;
}

//  Reads the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string path{kProcDirectory + to_string(pid) + kStatusFilename};
  return findValueByKey<string>(filterUID, path);
}

// Reads the user associated with a process
string LinuxParser::User(int pid) {
  ifstream stream{kPasswordPath};
  string line, username;
  if (stream) {
    string user_id = Uid(pid);
    regex exp{"(.)*(:" + user_id + ":)(.)*"};
    if (stream) {
      while (getline(stream, line) && !regex_match(line, exp)) {
      }
    }
    // find ":" , which ends the username in the line
    auto pos = line.find(":");
    username = line.substr(0, pos);
  }
  return username;
}

// Return process UpTime
long LinuxParser::UpTime(int pid) {
  vector<string> stats{LinuxParser::ReadProcStats(pid)};
  long sysUpTimeInSeconds = LinuxParser::UpTime();
  long startTime{};
  if (!stats.empty()) {
    startTime = stol(stats[pStartTime_]);
  }
  long startTimeInSseconds = startTime / sysconf(_SC_CLK_TCK);
  long upTime = sysUpTimeInSeconds - startTimeInSseconds;
  return upTime;
}
