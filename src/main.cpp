// TODO: remove before sending project
#include <chrono>
#include <iostream>
#include <thread>

#include "format.h"
#include "linux_parser.h"
#include "process.h"
using namespace std::literals::chrono_literals;
#include <exception>
//--------------------------------------------
#include "ncurses_display.h"
#include "system.h"

int main() {
  try {
    System system;
    NCursesDisplay::Display(system);
    // DEBUG:
    // std::cout << "System:\n"
    //           << " ----------------------" << std::endl;
    // std::cout << "Memory:\t" << system.MemoryUtilization() << std::endl;
    // std::cout << "upTime:\t" << Format::ElapsedTime(system.UpTime())
    //           << std::endl;
    // std::cout << "Total processes:\t" << system.TotalProcesses() <<
    // std::endl; std::cout << "Running processes:\t" <<
    // system.RunningProcesses()
    //           << std::endl;
    // std::cout << "Kernel:\t" << system.Kernel() << std::endl;
    // std::cout << "OS name:\t" << system.OperatingSystem() << std::endl;
    // std::cout << "CPU usage:\t" << system.Cpu().Utilization() << std::endl;

    // std::cout << "Processes:\n"
    //           << " ----------------------" << std::endl;
    // std::vector<Process> procs = system.Processes();
    // for (size_t i = 0; i < procs.size(); i++) {
    //   // for (const auto& proc : procs) {
    //   std::cout << "PID:\t" << procs[i].Pid() << std::endl;
    //   std::cout << "User:\t" << procs[i].User() << std::endl;
    //   std::cout << "Command:\t" << procs[i].Command() << std::endl;
    //   std::cout << "Process CPU:\t" << procs[i].CpuUtilization() <<
    //   std::endl; std::cout << "Process RAM:\t" << procs[i].Ram() <<
    //   std::endl; std::cout << "Process UpTime:\t" << procs[i].Ram() <<
    //   std::endl; if (i < procs.size() - 1) {
    //     std::cout << "proc[i] < proc[i+1]:\t" << std::boolalpha
    //               << (procs[i] < procs[i + 1]) << std::endl;
    //   }
    //   std::cout << "--------------------------------------" << std::endl;
    // }

  } catch (std::exception& exept) {
    std::cout << exept.what();
  }
  // Process process{101103};
  // std::cout << "Pid:\t" << process.Pid() << std::endl;
  // std::cout << "User:\t" << process.User() << std::endl;
  // std::cout << "Command:\t" << process.Command() << std::endl;
  // std::cout << "CPU:\t" << process.CpuUtilization() << std::endl;
  // std::cout << "RAM:\t" << process.Ram() << std::endl;
  // std::cout << "UpTime:\t" << process.UpTime() << std::endl;

  // for (int i = 0; i < 30; i++) {
  //   std::cout << "Process CPU ussage:\t" << 100 * process.CpuUtilization()
  //             << std::endl;
  //   std::this_thread::sleep_for(1000ms);
  // }

  // LinuxParser::UpTime(36951);
  // std::cout << "PID: " << 58469
  //           << "\tuptime: " //<<
  //           Format::ElapsedTime(LinuxParser::UpTime(58469))
  //           << std::endl;

  // std::cout << LinuxParser::Uid(1555) << std::endl;
  // std::cout << LinuxParser::User(1555) << std::endl;
  // auto stats = LinuxParser::ProccStats(1555);
  // for (size_t i = 0; i < stats.size(); i++) {
  //   std::cout << i << "\t" << stats[i] << std::endl;
  // }
}