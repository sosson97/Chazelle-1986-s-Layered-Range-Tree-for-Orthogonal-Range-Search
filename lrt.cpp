//lrt.cpp
//implementation of LRT class interface and internal functions

#include "lrt.h"

#include <algorithm>
#include <new>

#include <iostream>
#include "util.h"

using std::vector;
using std::sort;



//Constructor
LRT::LRT(const vector<Point>& input_point_set){
  //0. Copy the input
  vector<Point> point_set = input_point_set;

  //1. Sort by x-coordinate 
  sort(point_set.begin(), point_set.end(), CompareX);

  //2. Call recursive tree constructor
  root_ = ConstructLRT_(point_set, NEG_INF, POS_INF);
}

//API function
//ProcessQuery
vector<Point> LRT::ProcessQuery(X x1, X x2, Y y1, Y y2){
   
  vector<Point> answer;
  
  if(x1 > x2) return answer;
  if(y1 > y2) return answer;


  //1. Find split node to decompose R into R1, R2, R*
  LRTNode* split_node = FindSplitNode_(x1, x2, root_);
  
  if(split_node->children[0] == NULL){ //handling leaf node case
    for(int i = 0; i < split_node->point_list->size(); i++){
      Point p = split_node->point_list->at(i);
      if(p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2) answer.push_back(p);
    }
    return answer;
  }
  int b1;//block index where x1 is in
  int b2;//block index where x2 is in
  for(int i = 0; i < ALPHA; i++){
    if(x1 >= (split_node->children)[i]->left_slab && x1 < (split_node->children)[i]->right_slab) b1 = i;
    if(x2 >= (split_node->children)[i]->left_slab && x2 < (split_node->children)[i]->right_slab) b2 = i;
  }

  LRTNode* R1_node = split_node->children[b1];
  LRTNode* R2_node = split_node->children[b2];
  //2. First solve R*
  if(b2 - b1 != 1) { //check R* exists 
    X left_slab = R1_node->right_slab;
    X right_slab = R2_node->left_slab;
   
    //2-1. Find segments intersecting y = y2, [left_slab of R*:right_slab of R*]
    vector<Segment> temp = split_node->wl->ProcessQuery(y2);//Use window list to find intersecting segments
    vector<Segment> segment_set;
    for(int i = 0; i < temp.size(); i++)
      if(left_slab <= temp[i].start_point.x && temp[i].start_point.x < right_slab) segment_set.push_back(temp[i]);
    
    //2-2. Scan each segment following the polygonal line
    for(int i = 0; i < segment_set.size(); i++) {
      Segment s = segment_set[i];
      while(s.end_point.y >= y1) {
        answer.push_back(s.end_point);
        if(s.next == NULL) break;
        else s = *(s.next);
      }
    }
  }

  //3. Solve R1, R2
  
  
  //3.1 Solve R1
  vector<Point> R1_answer;
  //handling leaf node case
  if(R1_node->children[0] == NULL) {
    for(int i = 0; i < R1_node->point_list->size(); i++){
      Point p = R1_node->point_list->at(i);
      if(p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2) R1_answer.push_back(p);
    }
  }

  else R1_answer = R1_node->pst_right_open->ProcessQuery(x1, y1, y2);

  //3.2 Solve R2
  vector<Point> R2_answer;
  //handling leaf node case
  if(R2_node->children[0] == NULL) {
    for(int i = 0; i < R2_node->point_list->size(); i++){
      Point p = R2_node->point_list->at(i);
      if(p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2) R2_answer.push_back(p);
    }
  }
  else R2_answer = R2_node->pst_left_open->ProcessQuery(x2, y1, y2);

  //4. Merge partial solutions
  answer.insert(answer.end(), R1_answer.begin(), R1_answer.end());
  answer.insert(answer.end(), R2_answer.begin(), R2_answer.end());

  return answer;
}





//Internal function
//ConstructLRT_
LRTNode* LRT::ConstructLRT_(const vector<Point>& point_set, const X left_slab, const X right_slab){
  //point_set is sorted in x-order
  
  //0. Create empty root node, fill it as possible as we can
  LRTNode* temp = new LRTNode();
  temp->left_slab = left_slab;
  temp->right_slab = right_slab;
 
  //1. split point_set into alpha set
  int pnum = point_set.size();
  //1-1. If pnum < ALPHA, create leaf node and return
  if(pnum <= ALPHA) {//leaf node
    temp->wl = NULL;
    temp->pst_left_open = NULL;
    temp->pst_right_open = NULL;
    temp->point_list = new vector<Point>(point_set);
    for(int i = 0; i < ALPHA; i++)
      temp->children[i] = NULL;
    return temp;
  }

  //1-2. if not, split!
  int block_size = pnum / ALPHA;
  vector<Point> blocks[ALPHA];
  for(int i = 0; i < ALPHA-1; i++)
    blocks[i] = vector<Point>(point_set.begin() + i*block_size, point_set.begin() + (i+1)*block_size); 
  blocks[ALPHA-1] = vector<Point>(point_set.begin() + (ALPHA-1)*block_size, point_set.end());

  //2. Create segments for each block
  vector<Segment> segment_set;
  for(int i = 0; i < ALPHA; i++){
    vector<Segment> temp_segments = ConstructSegments_(blocks[i]);
    segment_set.insert(segment_set.end(), temp_segments.begin(), temp_segments.end());
  }

  //3. Create internal node and its children. Then, return
  temp->wl = new WL(segment_set); 
  temp->pst_left_open = new PST(point_set, LEFT_OPEN);
  temp->pst_right_open = new PST(point_set, RIGHT_OPEN);
  temp->point_list = NULL;

  temp->children[0] = ConstructLRT_(blocks[0], left_slab, blocks[1][0].x); 
  temp->children[ALPHA-1] = ConstructLRT_(blocks[ALPHA-1], blocks[ALPHA-1][0].x, right_slab);
  for(int i = 1; i < ALPHA-1; i++)
    temp->children[i] = ConstructLRT_(blocks[i], blocks[i][0].x, blocks[i+1][0].x);

  return temp;
}


//Internal function
//ConstructSegments_
vector<Segment> LRT::ConstructSegments_(vector<Point> block){
  //when block has a single point, return empty vector 
  vector<Segment> segment_set;
  //if(block.size() <= 1) return segment_set;
  
  //1. Sort by y-coordinate
  sort(block.begin(), block.end(), CompareY);
  
  //2. Create segments in reverse order. Then, return it
  //create first infinity segment
  

  Segment* old_s;
  for(int i = block.size(); i > 0; i--) {
    Segment* s = new Segment();
    
    if(i == block.size()) {
      Point p;
      p.x = block[block.size()-1].x;
      p.y = POS_INF; 
      s->start_point = p; 
    }
    else s->start_point = block[i];
    s->end_point = block[i-1];
    
    if(i < block.size()){
      old_s->next = s;
      segment_set.push_back(*old_s);
    }
    old_s = s;
  }
  old_s->next = NULL;
  segment_set.push_back(*old_s);

  return segment_set;
}

//Internal function
//FindSplitNode_
LRTNode* LRT::FindSplitNode_(X x1, X x2, LRTNode* root){
  if(root->children[0] == NULL) return root;
  
  int b1;//block index where x1 is in
  int b2;//block index where x2 is in
  for(int i = 0; i < ALPHA; i++){
    if(x1 >= (root->children)[i]->left_slab && x1 < (root->children)[i]->right_slab) b1 = i;
    if(x2 >= (root->children)[i]->left_slab && x2 < (root->children)[i]->right_slab) b2 = i;
  }
  if(b1 == b2) FindSplitNode_(x1, x2, root->children[b1]);
  else return root;
}




