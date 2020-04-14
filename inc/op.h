#ifndef OP
#define OP

#include<string>
#include<vector>
#include<unordered_map>

/*
* Basic operation class 
*
*/
class basic_op{
 public:
  bool check_operand(std::string operand);
  void process(std::vector<std::string> &parameter,
                          int pos,
                          std::vector<int> &size,
                          std::vector<std::string>& value);
  virtual std::string generate_opcode(std::unordered_map<std::string, int> &symtable,
                                       int cur_address, std::string parameter){ return ""; };

  virtual void preprocess(std::vector<std::string> &parameter,
                          int pos,
                          std::vector<int> &size,
                          std::vector<std::string>& value,
                          int &LOCCTR){}; 
  virtual ~basic_op() =default;
};

/*
*JSUB
*
*/
class JSUB:public basic_op{
  const static int opcode = 72;
public:
  JSUB():basic_op(){}
  ~JSUB() = default;  
  void preprocess(std::vector<std::string> &parameter,
                  int pos,
                  std::vector<int> &size,
                  std::vector<std::string>& value,
                  int &LOCCTR);
  std::string generate_opcode(std::unordered_map<std::string,int> &symtable,
                              int cur_address,std::string parameter);
  friend std::string generate_hex(int value, int length);
};

/*
*RSUB
*
*/
class RSUB:public basic_op{
  const static int opcode = 76;
public:
  RSUB():basic_op(){}
  ~RSUB() = default;
  void preprocess(std::vector<std::string> &parameter,
                  int pos,
                  std::vector<int> &size,
                  std::vector<std::string>& value,
                  int &LOCCTR);
  std::string generate_opcode(std::unordered_map<std::string,int> &symtable,
                              int cur_address,std::string parameter);
  friend std::string generate_hex(int value, int length);
};
/*
* LDA 
*
*/
class LDA:public basic_op{
  const static int opcode = 0;
public:
  LDA():basic_op(){}
  ~LDA() = default;
  void preprocess(std::vector<std::string> &parameter,
                  int pos,
                  std::vector<int> &size,
                  std::vector<std::string>& value,
                  int &LOCCTR);
  std::string generate_opcode(std::unordered_map<std::string,int> &symtable,
                              int cur_address,std::string parameter);
  friend std::string generate_hex(int value, int length);
};

/*
* RESERVE
*
*/
class RESERVE:public basic_op{
public:
  RESERVE():basic_op(){}
  ~RESERVE() = default; 
  void preprocess(std::vector<std::string> &parameter,
                  int pos,
                  std::vector<int> &size,
                  std::vector<std::string>& value,
                  int &LOCCTR);
  std::string generate_opcode(std::unordered_map<std::string,int> &symtable,
                              int cur_address,std::string parameter);
  friend std::string generate_hex(int value, int length);
};

/*
* ADD
*
*/
class ADD:public basic_op{
  const static int opcode = 24;
public:
  ADD():basic_op(){}
  ~ADD() =default;
  void preprocess(std::vector<std::string> &parameter,
                  int pos,
                  std::vector<int> &size,
                  std::vector<std::string>& value,
                  int &LOCCTR);
 std::string generate_opcode(std::unordered_map<std::string,int> &symtable,
                             int cur_address,std::string parameter);
 friend std::string generate_hex(int value, int length);
};

/*
* auxiliary function generate hex
*/
std::string generate_hex(int value, int length);


#endif
