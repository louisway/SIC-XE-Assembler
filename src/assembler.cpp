#include "assembler.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::unordered_map<std::string, int> assembler::optable = {
           {"START", -1},
           {"END",   -1},
           {"BYTE",  -1},
           {"WORD",  -1},
           {"ADD", 24},
           {"LDA", 0},
           {"AND", 64},   
           {"JSUB",72},
           {"RSUB",76},                   
};

assembler::assembler():parts(),symtable(),verb_pos(),address(),comments(){
  op_stack["LDA"] = new LDA();
  op_stack["RESERVE"] =  new RESERVE();
  op_stack["ADD"] =  new ADD();
  op_stack["JSUB"] =  new JSUB();
  op_stack["RSUB"] =  new RSUB();
}

assembler::~assembler(){
 for(auto &c: op_stack){
   delete c.second;
 }

}

void assembler::print_size(){
  std::cout << "print size:" << std::endl;
  std::cout << "symtable: " << symtable.size() << std::endl;
  std::cout << "parts: " << parts.size() << std::endl;
  /*for(auto& a:parts){
    for(auto& a1:a){
     std::cout << a1 << " ";
    }
    std::cout << std::endl; 
  }*/
  std::cout << "verb_pos: " << verb_pos.size() << std::endl;
  std::cout << "address: " << address.size() << std::endl;
  std::cout << "comments: " << comments.size() << std::endl;
  std::cout << "error flag: " << error_flag.size() << std::endl;
  
}

void assembler::print(){
  if(verb_pos.size() > 0 && verb_pos.back() == -1){
    //std::cout << "comments" << std::endl;
    return;
  }
  if(parts.size() > 0){
    std::cout<< "parts: " ;
    for(auto p:parts.back()){
       std::cout<< p << " " ;
    }
    std::cout << std::endl;
  }
  if(verb_pos.size() > 0){
    std::cout << "verb_pos: " << verb_pos.back() << std::endl; 
  }
  if(address.size() > 0){
    std::cout << "address: " << address.back() << std::endl;
  }
  if(comments.size() > 0){
    std::cout << "comments: " << comments.back() << std::endl; 
  }
}


void assembler::interprete_line(std::string line){
  //analysis for each line
  int pos = 0;
  int prev_pos = 0;
  int length = 0;
  std::vector<std::string> sub_parts;
  std::string verb;
  int p = -1;
  while(pos <= line.length()){
      
     if(pos < line.length() && line[pos] != ' ' && line[pos] != '\t'){
       ++length; 
     }else{
       if(length > 0){
         sub_parts.push_back(line.substr(prev_pos,length)); 
         if(optable.find(sub_parts.back()) != optable.end()){
           verb = sub_parts.back(); 
           p = sub_parts.size()-1;
         }
       } 
       length = 0;
       prev_pos = pos+1;
     }
     ++pos; 
  }
  if(sub_parts.size() == 0 || sub_parts[0] == "."){
    p = -1; 
  }
  /*if(length > 0){
    sub_parts.push_back(line.substr(prev_pos,length));
  }*/ 
  //check address
  if(p == -1 || sub_parts[0] == "."){
    address.push_back(-1);
    comments.push_back("comments");
  }else{
    //symbol
    comments.push_back(" ");
    if(p > 0 && LOCCTR != -1){
      std::string label = sub_parts[p-1];
      if(symtable.find(label) != symtable.end()){
         comments.back() += "fault: multiple define symbol";
         error_flag.back() = false;
      }else{
         symtable[label] = LOCCTR;
      } 
    }
    //process the parameter
    if(sub_parts[p] == "START"){
      LOCCTR = std::stoi(sub_parts[p+1]); 
      comments.back() += "start the program";
      address.push_back(LOCCTR);
      start_address = LOCCTR;
    }else if(sub_parts[p] == "END"){
      //add reserved_memory to the end
      std::vector<std::string> sp;
      sp.push_back("RESERVE");
      for(int i = 0; i < reserved_m_size.size(); ++i){
        std::string label = "*";
        label += std::to_string(i);
        symtable[label] = LOCCTR;
        address.push_back(LOCCTR);
        LOCCTR += reserved_m_size[i];
        sp.push_back(reserved_m_value[i]);
        parts.push_back(sp);
        verb_pos.push_back(0);
        sp.pop_back();
        comments.push_back("RESERVE"); 
      } 
      address.push_back(LOCCTR);
      comments.back() += "END";
    }else{
      address.push_back(LOCCTR);
      comments.back() += sub_parts[p];
      op_stack[sub_parts[p]]->preprocess(sub_parts, p, reserved_m_size, reserved_m_value, LOCCTR);
 
    }
  }
  verb_pos.push_back(p);
  parts.push_back(sub_parts);

}


void assembler::one_pass_process(std::string asmFile){
  std::ifstream sourcefile(asmFile);
  std::string line;
  while(std::getline(sourcefile,line)){
    interprete_line(line);
  }
}

std::string assembler::generate_header(std::vector<std::string> &parameter,
                                       int pos){
  std::string header = "H";
  if(parameter[pos-1].size() > 6){
    header += parameter[pos-1].substr(0,6); 
  }else{
    header += parameter[pos-1];
  }
  for(int i = parameter[pos-1].size() + 1; i <= 6; ++i){
    header += " ";
  }
  header += generate_hex(std::stoi(parameter[pos+1]),6);
  header += generate_hex(LOCCTR-start_address,6);
  return header;
}


void assembler::two_pass_process(){
    std::ofstream outputfile("out.obj");
    //header recorder
    int pos = 0;
    while(pos < verb_pos.size()){
      if(verb_pos[pos] == -1){
         ++pos; 
         continue;
      } 
      if(parts[pos][verb_pos[pos]] == "START"){
        break;
      }
      ++pos;
    }
    std::string header = generate_header(parts[pos], verb_pos[pos]);
    std::cout << header << std::endl;
    record.push_back(header);
    ++pos;
    //Text Record
    std::string text; 
    while(pos < parts.size()){
      if(verb_pos[pos] == -1){
        ++pos;
        continue;
      }
      if(parts[pos][verb_pos[pos]] == "END"){
        break;
      }
      std::string code = op_stack[parts[pos][verb_pos[pos]]]->generate_opcode(symtable,address[pos],parts[pos][verb_pos[pos]+1]);
      if(text.size() + code.size() > 69){
        std::cout << text << std::endl;
        std::string ssize = generate_hex(int((text.length()-9)/2),2);
        for(int ppp = 0; int(ssize.length())-1-ppp >= 0; ++ppp){
          text[8-ppp] = ssize[ssize.length()-1-ppp];  
        }
        std::cout << text << std::endl;
        record.push_back(text);
        text = ""; 
      } 
      if(text.length() == 0){
        text = "T"; 
        text += generate_hex(address[pos], 6);   
        text += "00";
      }
      text += code;
      ++pos; 
    }
    if(text.length() > 0){
      std::string ssize = generate_hex(int((text.length()-9)/2),2);
      for(int ppp = 0; int(ssize.length())-1-ppp >= 0; ++ppp){
        text[8-ppp] = ssize[ssize.length()-1-ppp];  
      } 
      std::cout << text << std::endl;
      record.push_back(text);
    }
    //End Record
    std::string end_record = "E";
    end_record += generate_hex(start_address,6);
    std::cout << end_record << std::endl;
    record.push_back(end_record);   
    for(auto& r:record){
      outputfile.write(r.c_str(),r.size());
      outputfile.write("\n",1); 
    }
}

