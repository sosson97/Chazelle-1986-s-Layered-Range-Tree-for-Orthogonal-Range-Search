//pst.h
//define Prioirty Search Tree class


#ifndef PST_H_
#define PST_H_

#include <vector>

#include "point.h"

using std::vector;


typedef char Type;
#define LEFT_OPEN 0
#define RIGHT_OPEN 1


//left < right
//first coordiante of right = mid
typedef struct PSTNode{
  Point point;
  Y mid;
  PSTNode* left;
  PSTNode* right;
} PSTNode;


class PST {
 public:
  //constructor 
  PST(const vector<Point>& point_set, const Type query_type);
  ~PST() {}
  //member functions
  vector<Point> ProcessQuery(X a, Y b, Y c);//(a,b,c) aligned range query, (a, y1, y2)

 private: 
  //internal functions
  vector<Point> ProcessQueryRecursive_(X a, Y y1, Y y2, PSTNode* root);
  PSTNode* ConstructPST_(const vector<Point>& point_set);
  //member 
  Type query_type_;
  PSTNode* root_;
};

#endif // PST_H_
