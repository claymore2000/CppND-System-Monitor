#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

static const long SecondsInAnHour = 3600;
static const long SecondsInAMinute = 60;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds)
{

  long hours = seconds / SecondsInAnHour;
  
  long remainingSeconds = seconds % SecondsInAnHour;
  
  long minutes = remainingSeconds / SecondsInAMinute;
  long secondsInCurrentMinute = remainingSeconds % SecondsInAMinute;

  std::stringstream ss;
  
  ss << std::setfill('0') << std::setw(2) << hours << ":"
     << std::setfill('0') << std::setw(2) << minutes << ":"
     << std::setfill('0') << std::setw(2) << secondsInCurrentMinute; 

  return ss.str();
}
