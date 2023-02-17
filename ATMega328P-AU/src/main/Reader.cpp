#include "Reader.h"


Reader::Reader(Stream& stream) : stream_(stream), currPos_(0), hasLine_(false)
{}

void Reader::loop() {
  while(stream_.available() && !hasLine_) {
    char c = stream_.read();
    if(c == '\n') {
      hasLine_ = true;
      buffer_[currPos_] = '\0';  
    } else if(isprint(c)) {
      buffer_[currPos_++] = c;  
      if(currPos_ >= sizeof(buffer_)) {
        currPos_ = 0;
      }
      buffer_[currPos_] = '\0';  
    }
  }
}


String Reader::getLine() {
    String rv(buffer_);
    currPos_ = 0;
    hasLine_ = false;
    buffer_[currPos_] = '\0';  
    return rv;
}  

bool Reader::hasLine() {
  return hasLine_;
}
