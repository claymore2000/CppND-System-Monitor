#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  void SetPid(int pid) { pid_ = pid; }
  std::string User();                      
  void SetUser(std::string user) { user_ = user; }
  std::string Command();                   
  void SetCmdline(std::string command) { cmdline_ = command; }
  float CpuUtilization();                  // TODO: See src/process.cpp
  void SetCpuUtilization(float cpu) { cpu_ = cpu; }
  std::string Ram() const;                       
  void SetRam(std::string ram) { ram_ = ram; } 
  long int UpTime();
  void SetUpTime(long uptime) { uptime_ = uptime; } 
  bool operator<(Process const& a) const;  

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string ram_;
  long uptime_;
  std::string cmdline_;
  float cpu_;
};

#endif
