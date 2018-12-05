//lrt.h
//define Layered Range Tree class


#ifndef LRT_H_
#define LRT_H_

#include <vector>

#include "point.h"
#include "pst.h"
#include "wl.h"


using std::vector;

#define ALPHA 3
#define NEG_INF -200000000
#define POS_INF 200000000



//Each LRTNode contains the point at left_slab, not contain the point at right_slab
typedef struct LRTNode{
  X left_slab;
  X right_slab;
  WL* wl;//for internal node
  PST* pst_left_open;//for internal node
  PST* pst_right_open;//for internal node
  vector<Point>*  point_list;//for leaf node
  LRTNode* children[ALPHA];
} LRTNode;


class LRT {
 public:
  //constructor 
  LRT(const vector<Point>& point_set);
  ~LRT() {}
  //member functions
  vector<Point> ProcessQuery(X x1, X x2, Y y1, Y y2);//[x1:x2]X[y1:y2]range query

 private: 
  //internal functions
  LRTNode* ConstructLRT_(const vector<Point>& point_set, const X left_slab, const X right_slab);
  vector<Segment> ConstructSegments_(vector<Point> block);
  LRTNode* FindSplitNode_(X x1, X x2, LRTNode* root);
  
  
  
  //member 
  LRTNode* root_;
};

#endif // LRT_H_
