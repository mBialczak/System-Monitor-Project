#include "process.h"

#include <unistd.h>

#include <cctype>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "linux_parser.h"

using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using namespace std::literals::chrono_literals;

Process::Process(int pid) : pid_(pid){};

//  Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  vector<string> stats{LinuxParser::ReadProcStats(pid_)};
  long SystemUpTime = LinuxParser::UpTime();
  long procActiveJiffies = LinuxParser::ProcessActiveJiffies(stats);
  // convert total proc Active Jiffies to seconds
  float procTotalTime =
      static_cast<float>(procActiveJiffies) / sysconf(_SC_CLK_TCK);
  // Read process start time
  float procStartTime = std::stof(stats[LinuxParser::pStartTime_]);
  // Convert process start time to seconds
  procStartTime /= sysconf(_SC_CLK_TCK);
  // elapsed time in second since the process started;
  float seconds = SystemUpTime - procStartTime;
  return procTotalTime / seconds;
}

// Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

bool Process::operator<(const Process& other) const {  // DEBUG:
  // return CpuUtilization() < other.CpuUtilization();
  return pid_ > other.pid_;
}