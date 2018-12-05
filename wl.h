//point.h
//define Window List class

#ifndef WL_H_
#define WL_H_



#include <vector>

#include "point.h"

using std::vector;




#define DELTA 2

typedef struct{
  Point point;
  bool is_start;
  Segment* segment;
} Pair;



class WL {//Window List
 public:
  //constructor 
  WL(const vector<Segment>& segment_set);
  ~WL() {}
  
  //member functions
  vector<Segment> ProcessQuery(const Y y); 

 private: 
  //internal functions

  //members
  vector<Segment> segment_list_;
  typedef struct{
    vector<Segment*> segment_list;
    Y start;
    Y end;
  } Window;
  vector<Window> WL_;
};

#endif // WL_H_
