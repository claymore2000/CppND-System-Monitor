#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

static const long SecondsInADay = 86400;
static const long SecondsInAnHour = 3600;
static const long SecondsInAMinute = 60;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)
{

  long days = seconds / SecondsInADay;
  long secondsUsedInDays = days * SecondsInADay;
  long hours = (seconds - secondsUsedInDays) / SecondsInAnHour;
  long minutesInCurrentHour = (seconds - secondsUsedInDays - (hours * SecondsInAnHour)) / SecondsInAMinute;

  std::stringstream ss;

  ss << std::setfill('0') << std::setw(2) << days << ":" << hours << ":" << minutesInCurrentHour; 

  return ss.str();
}
