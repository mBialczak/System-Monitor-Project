#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <iostream>  //TODO: remove after testing
#include <regex>
#include <string>
#include <vector>

using std::getline;
using std::ifstream;
using std::istringstream;
using std::regex;
using std::regex_match;
using std::stof;  // TODO: remove if redundant
using std::stol;  // TODO: remove if redundant
using std::string;
using std::to_string;  // TODO: remove if redundant
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// Read Kernel data
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

// BONUS TODO: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  float MemTotal{0.0};
  float MemFree{0.0};
  float result{0.0};
  ifstream stream{kProcDirectory + kMeminfoFilename};
  if (stream) {
    getline(stream, line);
    istringstream streamTotal(line);
    streamTotal >> key;
    if (key == "MemTotal:") streamTotal >> MemTotal;
    getline(stream, line);
    istringstream streamFree(line);
    streamFree >> key;
    if (key == "MemFree:") streamFree >> MemFree;
    if (MemTotal) result = (MemTotal - MemFree) / MemTotal;
  }
  return result;
}
// Read and return the system uptime
long LinuxParser::UpTime() {
  ifstream stream{kProcDirectory + kUptimeFilename};
  double seconds{};
  if (stream) {
    stream >> seconds;
  }
  return static_cast<long>(std::round(seconds));
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies(const vector<string>& stats) {
  return ActiveJiffies(stats) + IdleJiffies(stats);
}

//  Read and return the number of active jiffies for a Process
long LinuxParser::ProcessActiveJiffies(const vector<string>& stats) {
  long uTime{};
  long sTime{};
  if (!stats.empty()) {
    uTime = std::stod(stats[pUtime_]);
    sTime = std::stod(stats[pStime_]);
  }
  return uTime + sTime;
}

// Read and return the number of active jiffies for the system
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

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(const std::vector<string>& stats) {
  long idle{};
  long iowait{};
  if (!stats.empty()) {
    idle = stol(stats[kIdle_]);
    iowait = stol(stats[kIOwait_]);
  }
  return idle + iowait;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  ifstream stream{kProcDirectory + kStatFilename};
  string line, key, value;
  vector<string> util_values;
  if (stream) {
    getline(stream, line);  // TODO: optimize possibly
  }
  istringstream line_stream(line);
  line_stream >> key;
  while (line_stream >> value) {
    util_values.emplace_back(value);
  }
  return util_values;
}
// Read process statistics
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

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  ifstream stream{kProcDirectory + kStatFilename};
  string line, key;
  int processes{};
  regex exp{R"(processes(\s)+(\d)+)"};
  if (stream) {
    while (getline(stream, line) && !regex_match(line, exp)) {
    };
    istringstream line_stream(line);
    line_stream >> key >> processes;
  }
  return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  ifstream stream{kProcDirectory + kStatFilename};
  string line, key;
  int processes{};
  regex exp{R"(procs_running(\s)+(\d)+)"};
  if (stream) {
    while (getline(stream, line) && !regex_match(line, exp)) {
    };
    istringstream line_stream(line);
    line_stream >> key >> processes;
  }
  return processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line, command;
  ifstream stream{kProcDirectory + to_string(pid) + kCmdlineFilename};
  if (stream.is_open()) {
    getline(stream, line);
  }
  istringstream line_stream(line);
  line_stream >> command;
  return command;
}

// Read and return the memory used by a process DEBUG:
float LinuxParser::Ram(int pid) {
  ifstream stream{kProcDirectory + to_string(pid) + kStatusFilename};
  string line, key;
  float ram{};
  regex exp{R"((VmRSS:).*)"};
  if (stream) {
    while (getline(stream, line) && !regex_match(line, exp)) {
    }
  }
  istringstream line_stream(line);
  line_stream >> key >> ram;
  ram /= 1024;
  // round to one digit after decimal
  // int ramRounded = round(ram);
  return ram;
}

//  Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  ifstream stream{kProcDirectory + to_string(pid) + kStatusFilename};
  string line, key, uid;
  regex exp{R"((Uid:).*)"};
  if (stream) {
    while (getline(stream, line) && !regex_match(line, exp)) {
    }
  }
  istringstream line_stream(line);
  line_stream >> key >> uid;
  return uid;
}

// Read and return the user associated with a process
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
