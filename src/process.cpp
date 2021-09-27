#include "process.h"

#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::ostringstream;
using std::string;
using std::vector;

Process::Process(int pid)
    : pid_(pid),
      cpuUtilization_(CalcCPUutilization()),
      ram_(ReadRam(pid)),
      command_(LinuxParser::Command(pid_)),
      user_(ReadUser(pid)),
      upTime_(LinuxParser::UpTime(pid_)){};

//  Returns this process's ID
int Process::Pid() const { return pid_; }

// Returns this process's CPU utilization
float Process::CpuUtilization() const { return cpuUtilization_; }

// Returns this process's memory utilization
const string& Process::Ram() const { return ram_; }

// Returns the command that generated this process
const string& Process::Command() const { return command_; }

// Returns the user (name) that generated this process
const string& Process::User() const { return user_; }

// Return the age of this process (in seconds)
long Process::UpTime() const { return upTime_; }

// Compares CPU utilization of two processes
bool Process::operator>(const Process& other) const {
  return cpuUtilization_ > other.cpuUtilization_;
}

// Calculates actual process CPU utilization
float Process::CalcCPUutilization() const {
  vector<string> stats{LinuxParser::ReadProcStats(pid_)};
  long SystemUpTime = LinuxParser::UpTime();
  long procActiveJiffies = LinuxParser::ProcessActiveJiffies(stats);
  // convert total proc Active Jiffies to seconds
  float procTotalTime =
      static_cast<float>(procActiveJiffies) / sysconf(_SC_CLK_TCK);
  // Read process start time
  float procStartTime{};
  if (!stats.empty()) {
    procStartTime = std::stof(stats[LinuxParser::pStartTime_]);
  }
  // Convert process start time to seconds
  procStartTime /= sysconf(_SC_CLK_TCK);
  // elapsed time in second since the process started;
  float seconds = SystemUpTime - procStartTime;
  return procTotalTime / seconds;
}

// Reads the RAM consumption of the process
string Process::ReadRam(int pid) const {
  ostringstream stream;
  stream << std::right << std::setw(ramWidth) << std::setfill(' ')
         << std::setprecision(1) << std::fixed << LinuxParser::Ram(pid);
  return stream.str();
}

// Reads the name of the user activating the process
string Process::ReadUser(int pid) const {
  ostringstream stream;
  stream << std::left << std::setw(userWidth) << std::setfill(' ')
         << LinuxParser::User(pid);
  return stream.str();
}