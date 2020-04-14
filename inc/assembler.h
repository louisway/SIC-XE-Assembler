#ifndef ASSEMBLER
#define ASSEMBLER

#include <unordered_map>
#include <string>
#include <vector>
#include "op.h"


class assembler{
  std::unordered_map<std::string,basic_op*> op_stack;
  std::unordered_map<std::string, int> symtable;
  std::vector<int> reserved_m_size;
  std::vector<std::string> reserved_m_value;
  std::vector<std::vector<std::string> > parts;
  std::vector<bool> error_flag;
  std::vector<int> verb_pos;
  std::vector<int> address;
  std::vector<std::string> comments;
  std::vector<std::string> record;
  int LOCCTR;
  int start_address;
public:
  assembler();
  ~assembler();
  void interprete_line(std::string line);
  void one_pass_process(std::string asmFile);
  void two_pass_process();
  std::string generate_header(std::vector<std::string> &parameter, int pos);
  friend std::string generate_hex(int value, int length);
  void print();
  void print_size();
  static std::unordered_map<std::string, int> optable;
};

#endif
