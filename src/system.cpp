#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System()
    : osName_(LinuxParser::OperatingSystem()),
      kernel_(LinuxParser::Kernel()),
      pids_(LinuxParser::Pids()) {
  for (const auto& pid : pids_) {
    processes_.emplace_back(Process(pid));
  }
}

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  // get current pids list and compare to previous
  vector<int> newPids{LinuxParser::Pids()};
  // if newPids != pids need to update all processes
  if (newPids != pids_) {
    processes_.clear();
    for (const auto& pid : newPids) {
      processes_.emplace_back(Process(pid));
    }
    pids_ = newPids;
  }
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() const { return kernel_; }

// Return the system's memory utilization
float System::MemoryUtilization() const {
  return LinuxParser::MemoryUtilization();
}

// Return the operating system name
std::string System::OperatingSystem() const { return osName_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() const { return LinuxParser::UpTime(); }