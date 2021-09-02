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
  return to_string(h.count()) + ":" + to_string(m.count()) + ":" +
         to_string(rs.count());
}