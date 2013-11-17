#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>


using BlockOffset = uint64_t;
const int BlockSize = 38;
struct Block{
  BlockOffset data;
  std::vector<BlockOffset> offsets;
  Block() : data {0}, offsets(BlockSize-1) {}
};




int char2int(const char ch) {
  if(ch>='0' && ch <='9'){
    return ch-'0';
  }else if(ch>='A' && ch <='Z'){
    return 10 + ch - 'A';
  }else if(ch=='_'){
    return 36;
  }  
}


Block readBlock(std::fstream f) {
  Block b {};
  f.read(reinterpret_cast<char *>(&b.data), sizeof(b.data));
  for(auto o : b.offsets) {
      f.read(reinterpret_cast<char *>(&o), sizeof(o));
  }
  return b;  
}

std::ostream& operator<<(std::ostream& s, Block& b) {
  s << "Block dump" << std::endl;
  s << "\tdata = " << b.data << std::endl;
  int counter = 0;
  for (auto o : b.offsets) {
    s << "\toffset " << counter << " = " << b.data << std::endl;
    counter++;
  }
  return s;
}


BlockOffset findWord(std::string word, std::fstream f) {
  BlockOffset n {};
  

  for(auto c : word) {
    int i = char2int(c);    
  }

  return n;
}





int main(int argc, char *argv[]) {
  
  Block b{};
  std::cout << b << std::endl;
  std::cout << b.offsets.size() << std::endl;

  return 0;
}

