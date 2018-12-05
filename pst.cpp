//pst.cpp
//implementation of Priority Search Tree class interface and internal functions

#include "pst.h"

#include <new>
#include <algorithm>

#include "util.h"

using std::sort;

//Constructor
PST::PST(const vector<Point>& input_point_set, const Type query_type){
  query_type_ = query_type;
  root_ =  ConstructPST_(input_point_set);
}


//API function
//ProcessQuery
vector<Point> PST::ProcessQuery(X a, Y y1, Y y2) {
  return  ProcessQueryRecursive_(a, y1, y2, root_);
}

//Internal function
//ProcessQueryRecursive_
vector<Point> PST::ProcessQueryRecursive_(X a, Y y1, Y y2, PSTNode* root) {
  vector<Point> answer;
  if(query_type_ == LEFT_OPEN) {//[-inf:a]X[y1:y2]
    //check root first
    if(root->point.x > a) return answer;
    else if(root->point.y >= y1 && root->point.y <= y2) answer.push_back(root->point);  
    
    //check left, right condition
    //check left
    vector<Point> left_answer;
    if(root->mid >= y1 && root->left != NULL) left_answer = ProcessQueryRecursive_(a, y1, y2, root->left);//we have to check left subtree

    //check right
    vector<Point> right_answer;
    if(root->mid <= y2 && root->right != NULL) right_answer = ProcessQueryRecursive_(a, y1, y2, root->right);//we have to check right subtree

    //merge answers
    answer.insert(answer.end(), left_answer.begin(), left_answer.end()); 
    answer.insert(answer.end(), right_answer.begin(), right_answer.end());
  }
  else if(query_type_ == RIGHT_OPEN){//[a:inf]X[y1:y2]
    //check root first
    if(root->point.x < a) return answer;
    else if(root->point.y >= y1 && root->point.y <= y2) answer.push_back(root->point); 
 
    //check left, right condition
    //check left
    vector<Point> left_answer;
    if(root->mid >= y1 && root->left != NULL) left_answer = ProcessQueryRecursive_(a, y1, y2, root->left);//we have to check left subtree

    //check right
    vector<Point> right_answer;
    if(root->mid <= y2 && root->right != NULL) right_answer = ProcessQueryRecursive_(a, y1, y2, root->right);//we have to check right subtree

    //merge answers
    answer.insert(answer.end(), left_answer.begin(), left_answer.end()); 
    answer.insert(answer.end(), right_answer.begin(), right_answer.end());

  }
  return answer;
}



//Internal function
//ConstructPSTLeft_
PSTNode* PST::ConstructPST_(const vector<Point>& input_point_set) {  
  
  //1. Find root point according to the query_type
  //if query_type == LEFT_OPEN, 0-th point
  //if query_type == RIGHT_OPEN, last point 
  //if size == 1, create leaf node
  PSTNode* root = new PSTNode();
  root->point = query_type_ ?  input_point_set[input_point_set.size()-1] : input_point_set[0];
  if(input_point_set.size() == 1){//leaf node
    root->mid = root->point.y;
    root->left = NULL;
    root->right = NULL;
    return root;
  }

  //2. Partition remaining points into two by y-coordinate. Then, set the mid
  vector<Point> point_set = query_type_? vector<Point>(input_point_set.begin(), input_point_set.end()-1) : vector<Point>(input_point_set.begin() + 1, input_point_set.end());
    
  sort(point_set.begin(), point_set.end(), CompareY);

  vector<Point> left_point_set = vector<Point>(point_set.begin(), point_set.begin() + point_set.size()/2); //left <- low y-coordinate 
  vector<Point> right_point_set = vector<Point>(point_set.begin() + point_set.size()/2, point_set.end());  //right <- high y-coordinate

  root->mid = right_point_set[0].y;

  //3. Reorder by x-coordinate before creating children
  sort(left_point_set.begin(), left_point_set.end(), CompareX);
  sort(right_point_set.begin(), right_point_set.end(), CompareX);

  //4. Create children. Then return root
  if(left_point_set.size() > 0) 
    root->left = ConstructPST_(left_point_set);
  else
    root->left = NULL;
  if(right_point_set.size() > 0)
    root->right = ConstructPST_(right_point_set);
  else
    root->right = NULL;

  return root;
}


