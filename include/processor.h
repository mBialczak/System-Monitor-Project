#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <chrono>  //TODO:: verify if needed
#include <string>  //TODO:: verify if needed
#include <vector>  //TODO:: verify if needed
using namespace std::literals::chrono_literals;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  std::vector<std::string>
      prevCpuStats;  // TODO: consider moving to the function
  std::vector<std::string> cpuStats;
  const std::chrono::microseconds interval = 200ms;
};

#endif

// TODO: REMOVE conscept if works
/* PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
prevsteal NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
*/