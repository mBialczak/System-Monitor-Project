#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <iostream>  //TODO: remove after testing
#include <regex>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line, key;
  float MemTotal{0.0};
  float MemFree{0.0};
  std::ifstream stream{kProcDirectory + kMeminfoFilename};
  if (stream) {
    std::getline(stream, line);
    std::istringstream streamTotal(line);
    streamTotal >> key;
    if (key == "MemTotal:") streamTotal >> MemTotal;
    std::getline(stream, line);
    std::istringstream streamFree(line);
    streamFree >> key;
    if (key == "MemFree:") streamFree >> MemFree;
  }
  float result{0.0};
  if (MemTotal) result = (MemTotal - MemFree) / MemTotal;
  return result;
}
// Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream stream{kProcDirectory + kUptimeFilename};
  double seconds{};
  if (stream) {
    stream >> seconds;
  }
  return static_cast<long>(std::round(seconds));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream{kProcDirectory + kStatFilename};
  string line, key;
  int processes{};
  std::regex exp{R"(processes(\s)+(\d)+)"};
  if (stream) {
    while (std::getline(stream, line) && !std::regex_match(line, exp)) {
    };
    std::istringstream line_stream(line);
    line_stream >> key >> processes;
  }
  return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream{kProcDirectory + kStatFilename};
  string line, key;
  int processes{};
  std::regex exp{R"(procs_running(\s)+(\d)+)"};
  if (stream) {
    while (std::getline(stream, line) && !std::regex_match(line, exp)) {
    };
    std::istringstream line_stream(line);
    line_stream >> key >> processes;
  }
  return processes;
  ;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
