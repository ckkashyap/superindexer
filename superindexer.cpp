#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>


using BlockOffset = uint64_t;
using FileID = uint64_t;
const int BlockSize = 38;
struct Block{
  FileID data;
  std::vector<BlockOffset> offsets;
  Block() : data {0}, offsets(BlockSize-1) {}
};


unsigned int char2int(const unsigned char ch) {
  if(ch>='0' && ch <='9'){
    return ch - '0';
  }else if(ch>='A' && ch <='Z'){
    return 10 + ch - 'A';
  }else if(ch>='a' && ch <='z'){
    return 10 + ch - 'a';
  }else if(ch=='_'){
    return 36;
  }
  return 0;
}


BlockOffset seekRW(std::fstream& f, BlockOffset off, std::ios_base::seekdir dir = std::ios_base::beg) {
  f.seekp(off, dir);
  f.seekg(off, dir);
  return off;
}


Block readBlockFromFile(std::fstream& f) {
  Block b {};
  BlockOffset off = f.tellg();
  f.read(reinterpret_cast<char *>(&b.data), sizeof(b.data));
  for(auto& o : b.offsets) {
      f.read(reinterpret_cast<char *>(&o), sizeof(o));
  }
  f.seekg(off, f.beg);
  return b;  
}

BlockOffset writeBlockToFile(Block b, std::fstream& f) {
  BlockOffset off = f.tellp();
  std::cout << "--> " << b.data << std::endl;
  f.write(reinterpret_cast<char *>(&b.data), sizeof(b.data));
  for(auto& o : b.offsets) {
      f.write(reinterpret_cast<char *>(&o), sizeof(o));
  }
  f.seekp(off, f.beg);
  return off;  
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


FileID getNextID() {
  const std::string fn {"next.dat"};
  std::fstream f {};
  f.open(fn, f.in | f.out );
  if(!f.is_open()) {
    f.open( fn , f.out | f.binary );
    f << "1" << std::endl;
    f.close();
    f.open("next.dat", f.in | f.out );
  }
  FileID i {};
  f >> i ;
  seekRW(f, 0);
  f << i+1 << std::endl;
  f.close();
  return i;  
}


Block getWordBlock(std::string word, std::fstream& f) {
  seekRW(f, 0);
  BlockOffset currentOffset = 0;
  Block b {readBlockFromFile(f)};

  std::cout << b << std::endl;
  

  for(auto c : word) {
    unsigned int i = char2int(c);
    std::cout << "processing " << c << " => " << i  << " => " << b.offsets[i] << std::endl;

    if (b.offsets[i] == 0 ) {
      BlockOffset off = f.tellp();
      Block newBlock {};
      seekRW(f, 0, f.end); 
      BlockOffset newCurrent = b.offsets[i] = writeBlockToFile(newBlock, f);
      seekRW(f, off);
      std::cout << b << std::endl;
      writeBlockToFile(b, f);
      seekRW(f, newCurrent);
      currentOffset = newCurrent;
      b = newBlock;
    } else {
      currentOffset = b.offsets[i];
      seekRW(f, currentOffset);
      b = readBlockFromFile(f);
    }
  }

  return b;
}




FileID addWord(std::string word, std::fstream& f) {
  Block b = getWordBlock(word, f);
  FileID fid {b.data};

  if (fid == 0)
    fid = getNextID();

  b.data = fid;
  writeBlockToFile(b, f);

  return fid;
}




std::fstream& initializeFile(std::fstream& f, std::string fn) {
  f.close();
  f.open( fn ,f.in | f.out | f.binary );
  if (!f.is_open()) {
    std::cout << "Creating the index file" << std::endl;
    f.open( fn , f.out | f.binary );
    Block b {};
    b.data=123;
    writeBlockToFile(b, f);
    f.close();
    f.open( fn ,f.in | f.out | f.binary );
  }
  return f;
}



int main(int argc, char *argv[]) {
  std::fstream f {};
  initializeFile(f, "index.dat");
  FileID fid = addWord(argv[1], f);
  seekRW(f, 0, f.end);
  f.close();

  std::cout << fid << std::endl;
  return 0;
}

