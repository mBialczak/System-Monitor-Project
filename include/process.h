#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int pid);
  int Pid() const;
  const std::string& User() const;
  const std::string& Command() const;
  float CpuUtilization() const;
  const std::string& Ram() const;
  long UpTime() const;
  bool operator<(const Process& other) const;  // sorts by CPU utilization

 private:
  float CalcCPUutilization() const;
  std::string ReadRam(int pid) const;
  std::string ReadUser(int pid) const;

  int pid_;
  float cpuUtilization_;
  std::string ram_;
  std::string command_;
  std::string user_;
  long upTime_;

  short userWidth{12};  // for output formatting
  short ramWidth{6};    // for output formatting
};

#endif