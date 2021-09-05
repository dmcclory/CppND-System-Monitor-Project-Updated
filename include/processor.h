#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <string>
#include <vector>

using std::stol;
using std::to_string;

class CpuUtilizationSnapshot {
 public:
  long int user{0};
  long int nice{0};
  long int system{0};
  long int idle{0};
  long int iowait{0};
  long int irq{0};
  long int softirq{0};
  long int steal{0};
  long int guest{0};
  long int guest_nice{0};

  CpuUtilizationSnapshot(){};

  CpuUtilizationSnapshot(std::vector<std::string> entries) {
    user = stol(entries[1]);
    nice = stol(entries[2]);
    system = stol(entries[3]);
    idle = stol(entries[4]);
    iowait = stol(entries[5]);
    irq = stol(entries[6]);
    softirq = stol(entries[7]);
    steal = stol(entries[8]);
    guest = stol(entries[9]);
    guest_nice = stol(entries[10]);
  }

  long int Idle() { return idle + iowait; }
  long int NonIdle() { return user + nice + system + irq + softirq + steal; }
};

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  CpuUtilizationSnapshot prev;
};

#endif