#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// [X] TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> values = LinuxParser::CpuUtilization();
  CpuUtilizationSnapshot current = CpuUtilizationSnapshot(values);

  // based on the stackover flow comment linked in the example
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  // but with some of functionality moved into CpuUtilizationSnapshot
  long int PrevTotal = prev.Idle() + prev.NonIdle();
  long int CurrentTotal = current.Idle() + current.NonIdle();

  long int total = CurrentTotal - PrevTotal;
  long int idled = current.Idle() - prev.Idle();

  float res = float(total - idled) / (float)total;

  prev = current;
  return res;
}