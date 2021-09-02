#include "format.h"

#include <chrono>
#include <string>

using std::string;
using std::to_string;

using namespace std::chrono;

// [X] TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long s) {
  seconds sec = seconds(s);
  auto h = duration_cast<hours>(sec);
  auto m = duration_cast<minutes>(sec - h);
  auto rs = duration_cast<seconds>(sec - (h + m));
  char buffer[9];
  sprintf(buffer, "%02li:%02li:%02li", h.count(), m.count(), rs.count());
  return std::string(buffer);
}