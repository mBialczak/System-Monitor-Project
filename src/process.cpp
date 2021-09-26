#include "process.h"

#include <unistd.h>

#include <cctype>
#include <chrono>
#include <iomanip>  // TODO: define if needed
#include <sstream>  // TODO: define if needed
#include <string>
#include <thread>
#include <vector>

#include "linux_parser.h"

using std::ostringstream;  // TODO: define if needed
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using namespace std::literals::chrono_literals;

Process::Process(int pid)
    : pid_(pid),
      cpuUtilization_(CalcCPUutilization()),
      ram_(ReadRam(pid)),
      command_(LinuxParser::Command(pid_)),
      user_(ReadUser(pid)),
      upTime_(LinuxParser::UpTime(pid_)){
          // TODO: revise
          // cpuUtilization_ = calcCPUutilization();
      };

//  Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuUtilization_; }

// Return this process's memory utilization
const string& Process::Ram() const {
  return ram_;
  // std::ostringstream stream;
  // stream << std::right << std::setw(ramWidth) << std::setfill(' ')
  //        << LinuxParser::Ram(pid_);
  // return stream.str();
  // return LinuxParser::Ram(pid_); // TODO: clean }
}

// Return the command that generated this process
const string& Process::Command() const { return command_; }

// Return the user (name) that generated this process
const string& Process::User() const {
  return user_;  // TODO: clean
}

// Return the age of this process (in seconds)
long Process::UpTime() const { return upTime_; }

bool Process::operator<(const Process& other) const {  // DEBUG:
  // return CpuUtilization() < other.CpuUtilization();
  return cpuUtilization_ > other.cpuUtilization_;
}

float Process::CalcCPUutilization() const {
  // TODO:  revise
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

string Process::ReadRam(int pid) const {
  std::ostringstream stream;
  stream << std::right << std::setw(ramWidth) << std::setfill(' ')
         << std::setprecision(1) << std::fixed << LinuxParser::Ram(pid);
  return stream.str();
}

string Process::ReadUser(int pid) const {
  std::ostringstream stream;
  stream << std::left << std::setw(userWidth) << std::setfill(' ')
         << LinuxParser::User(pid);
  return stream.str();
}