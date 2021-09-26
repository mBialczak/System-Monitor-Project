#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <chrono>

using namespace std::literals::chrono_literals;

class Processor {
 public:
  float Utilization() const;

 private:
  const std::chrono::microseconds interval = 200ms;
};

#endif