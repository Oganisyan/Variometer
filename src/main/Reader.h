#ifndef _READER_H_
#define _READER_H_

#include <Stream.h>
#include <WString.h>

class Reader {
  Stream& stream_;
  char buffer_[256];
  int  currPos_;
  bool hasLine_;

  public:
    Reader(Stream& stream);
    void loop();
    String getLine();
    bool hasLine();
  
};



#endif //_READER_H_
