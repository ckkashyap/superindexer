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


Block readBlockFromFile(std::fstream& f) {
  Block b {};
  f.read(reinterpret_cast<char *>(&b.data), sizeof(b.data));
  for(auto& o : b.offsets) {
      f.read(reinterpret_cast<char *>(&o), sizeof(o));
  }
  return b;  
}

Block writeBlockToFile(Block b, std::fstream& f) {
  f.write(reinterpret_cast<char *>(&b.data), sizeof(b.data));
  for(auto o : b.offsets) {
      f.write(reinterpret_cast<char *>(&o), sizeof(o));
  }
  return b;  
}


std::ostream& operator<<(std::ostream& s, Block& b) {
  s << "Block dump" << std::endl;
  s << "\tdata = " << b.data << std::endl;
  int counter = 0;
  for (auto o : b.offsets) {
    s << "\toffset " << counter << " = " << o << std::endl;
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


std::fstream initializeFile(const std::string& fn) {
  std::fstream f {};
  f.open( fn ,std::ios_base::in | std::ios_base::out | std::ios_base::binary );
  if (!f.is_open()) {
    std::cout << "Creating the index file" << std::endl;
    f.open( fn , std::ios_base::out | std::ios_base::binary );
    f.close();
    f.open( fn ,std::ios_base::in | std::ios_base::out | std::ios_base::binary );
  }
  return f;
}



int main(int argc, char *argv[]) {  
  Block b{};

  b.data=123;
  int i = 0;
  for(auto& o : b.offsets) {
    o = i++;
  }
  std::cout << b << std::endl;

  std::string fn {"index.dat"};
  std::fstream f = initializeFile(fn);
  writeBlockToFile(b, f);
  f.close();
  f = initializeFile(fn);
  b = readBlockFromFile(f);
  std::cout << b << std::endl;
  
  return 0;
}

