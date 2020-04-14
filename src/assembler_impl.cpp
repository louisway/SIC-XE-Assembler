#include "assembler.h"

int main(int argc, char** argv){
  assembler asmler;
  asmler.one_pass_process(std::string(argv[1]));
  asmler.two_pass_process();
  return 1;
}
