#ifndef PROCESS_H
#define PROCESS_H

#include <chrono>
#include <string>
using namespace std::literals::chrono_literals;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(const Process& other) const;  // TODO: See src/process.cpp

 private:
  int pid_;
};

#endif