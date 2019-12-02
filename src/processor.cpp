#include <vector>
#include <string>
#include <numeric>
using std::stof;


#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{

  std::vector<std::string> utilization = LinuxParser::CpuUtilization();
  if (utilization.empty() == false)
    {
      /*
      The advantage of this approach is that the vector is sized exactly once, rather than continuously growing. 
      The disadvantage is that vector elements have to be default-constructed first, and then be reassigned with 
      the correct value afterwards. This tradeoff is worth it for element types that satisfy all of the following:

      can be default-constructed
      are cheap to default-construct
      can be assigned with a value of the same type
      are cheap to assign
      */
      std::vector<float> floatUtilVector;
      floatUtilVector.reserve(utilization.size());
      transform(utilization.begin(), utilization.end(), back_inserter(floatUtilVector),
		[](std::string const& val) {return stof(val);});

      float denominator = std::accumulate(floatUtilVector.begin(), floatUtilVector.end(),0);
      float numerator = denominator - floatUtilVector.at(3);
      return numerator/denominator;
      }
  return 0.0;
}
