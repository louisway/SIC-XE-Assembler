#include "op.h"

/*
*Basic operation
*
*/
bool basic_op::check_operand(std::string operand){
  //true for immediate value
  for(auto &c: operand){
    if(c < '0' || c > '9'){
      return false; 
    } 
  }
  return true;
}

/*
* LDA 
*/
void LDA::preprocess(std::vector<std::string> &parameter,
                     int pos,
                     std::vector<int> &size,
                     std::vector<std::string> &value,
                     int &LOCCTR){
  LOCCTR += 3; 
  if(parameter[pos+1][0] == '=' || parameter[pos+1][0] == '#'){
    if(check_operand(parameter[pos+1].substr(1,parameter[pos+1].length()-1))){
      size.push_back(3);
      value.push_back(parameter[pos+1].substr(1,parameter[pos+1].length()-1)); 
      parameter[pos+1] = "*" + std::to_string(value.size()-1);
    }else{
      parameter[pos+1] = parameter[pos+1].substr(1,parameter[pos+1].length()-1);
    } 
  }else if(parameter[pos+1][0] == '@'){
      parameter[pos+1] = parameter[pos+1].substr(1,parameter[pos+1].length()-1);
  }
}

std::string LDA::generate_opcode(std::unordered_map<std::string,int> &symtable,
                                 int cur_address,std::string parameter){
  std::string code;
  code += generate_hex(opcode+3,2);
  code += "2";
  code += generate_hex(symtable[parameter] - cur_address -3,3);
  return code;
}

/*
*RESERVE
*
*/
void RESERVE::preprocess(std::vector<std::string> &parameter,
                     int pos,
                     std::vector<int> &size,
                     std::vector<std::string> &value,
                     int &LOCCTR){
}

std::string RESERVE::generate_opcode(std::unordered_map<std::string,int> &symtable,
                                 int cur_address,std::string parameter){
  return generate_hex(std::stoi(parameter),6);
};


/*
*auxiliary function generate_hex
*
*/
std::string generate_hex(int value, int length){
  char hex_str[20];
  sprintf(hex_str, "%X", value);
  std::string hex_(hex_str);
  std::string com_zero(length-hex_.length(),'0');
  return com_zero+hex_;
}
