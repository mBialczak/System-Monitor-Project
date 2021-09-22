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
  int Pid() const;                             // TODO: See src/process.cpp
  std::string User() const;                    // TODO: See src/process.cpp
  std::string Command() const;                 // TODO: See src/process.cpp
  float CpuUtilization() const;                // TODO: See src/process.cpp
  std::string Ram() const;                     // TODO: See src/process.cpp
  long int UpTime() const;                     // TODO: See src/process.cpp
  bool operator<(const Process& other) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  const std::chrono::microseconds interval = 200ms;  // DEBUG: maybe not needed
  int pid_;
};

#endif