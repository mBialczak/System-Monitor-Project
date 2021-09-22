#include "processor.h"

#include <chrono>
#include <thread>

#include "linux_parser.h"

using namespace std::literals::chrono_literals;
// TODO: REMOVE conscept if works
/* PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
std::this_thread::sleep_for(200ms);
*/

float Processor::Utilization() {
  // Read CPU stats in a time interval
  prevCpuStats = LinuxParser::CpuUtilization();
  std::this_thread::sleep_for(interval);
  cpuStats = LinuxParser::CpuUtilization();
  // extract stats for first read
  long prevTotal = LinuxParser::Jiffies(prevCpuStats);
  long prevIdle = LinuxParser::IdleJiffies(prevCpuStats);
  // extract stats for second read
  long total = LinuxParser::Jiffies(cpuStats);
  long idle = LinuxParser::IdleJiffies(cpuStats);
  // calc  the differences in reads
  float totalDelta = static_cast<float>(total - prevTotal);
  float idleDelta = static_cast<float>(idle - prevIdle);
  // calc the final CPU percentage
  return (totalDelta - idleDelta) / (totalDelta);
}