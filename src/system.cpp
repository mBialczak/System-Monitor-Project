#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

System::System()
    : osName_(LinuxParser::OperatingSystem()), kernel_(LinuxParser::Kernel()) {}

Processor& System::Cpu() { return cpu_; }

// Returns a container composed of the system's processes
std::vector<Process>& System::Processes() {
  std::vector<int> Pids{LinuxParser::Pids()};
  processes_.clear();
  processes_.reserve(Pids.size());
  for (auto pid : Pids) {
    processes_.emplace_back(Process(pid));
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

// Returns the system's kernel identifier (string)
const std::string& System::Kernel() const { return kernel_; }

// Returns the system's memory utilization
float System::MemoryUtilization() const {
  return LinuxParser::MemoryUtilization();
}

// Returns the operating system name
const std::string& System::OperatingSystem() const { return osName_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() const { return LinuxParser::UpTime(); }