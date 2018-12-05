//wl.cpp
//implementation of Window List class interface and internal functions

#include "wl.h"

#include <new>
#include <algorithm>

#include <iostream>


#include "util.h"

using std::sort;

#define POS_INF 200000000

//util function
bool CompareYPair(Pair A, Pair B){return A.point.y > B.point.y;}

//Constructor
WL::WL(const vector<Segment>& segment_set) { 
  segment_list_ = segment_set;

  //Construct WL_
  //0. Create sorted list of end points
  vector<Pair> end_point_pairs;
  for(int i = 0; i < segment_list_.size(); i++){
    Pair start_pair;
    Pair end_pair;
    start_pair.point = segment_list_[i].start_point;
    start_pair.is_start = true;
    start_pair.segment = &(segment_list_[i]);
    end_pair.point = segment_list_[i].end_point;
    end_pair.is_start = false;
    end_pair.segment = &(segment_list_[i]);
    end_point_pairs.push_back(start_pair);
    end_point_pairs.push_back(end_pair);
  }

  sort(end_point_pairs.begin(), end_point_pairs.end(), CompareYPair);
  
  //1. Window make iteration
  Window w;
  w.segment_list = vector<Segment*>(0);
  w.start = end_point_pairs[0].point.y;
  w.end = end_point_pairs[0].point.y;
  
  //create WL with one window
  WL_.push_back(w);

  int win_num = 0;
  int cur_count = 0;//the number of segments overlap at current point
  int seg_count = 0;//the number of total segments in window
  int low = 1;
  for(int i = 0; i < end_point_pairs.size(); i++) {
    if(end_point_pairs[i].is_start == true) {
      cur_count++;
      seg_count++;
      if(DELTA*low < seg_count) {
        //create new window
        WL_[win_num].end = end_point_pairs[i].point.y;
        Window w2;
        w2.segment_list = vector<Segment*>(0);
        w2.start = end_point_pairs[i].point.y;
        w2.end = end_point_pairs[i].point.y;
        for(int j = 0; j < WL_[win_num].segment_list.size(); j++)
          if((WL_[win_num].segment_list[j])->end_point.y < w2.start)
            w2.segment_list.push_back(WL_[win_num].segment_list[j]);
        
        //add current segment to new window
        w2.segment_list.push_back(end_point_pairs[i].segment);

        WL_.push_back(w2);
        win_num++;
       
        //update low
        seg_count = cur_count;
        low = seg_count;
      }

      else WL_[win_num].segment_list.push_back(end_point_pairs[i].segment);//add segment to window 

    }

    else { // end_point
      cur_count--;
      low = low < cur_count ? low : cur_count;
      if(DELTA*low < seg_count) {
        //create new window
        WL_[win_num].end = end_point_pairs[i].point.y;
        Window w2;
        w2.segment_list = vector<Segment*>(0);
        w2.start = end_point_pairs[i].point.y;
        w2.end = end_point_pairs[i].point.y;
        for(int j = 0; j < WL_[win_num].segment_list.size(); j++)
          if((WL_[win_num].segment_list[j])->end_point.y < w2.start)
            w2.segment_list.push_back(WL_[win_num].segment_list[j]);
        
        WL_.push_back(w2);
        win_num++;


        seg_count = cur_count;
        low = 1 < seg_count ? 1 : seg_count;
      }

    }
  }
  WL_.pop_back();
  //WL_[WL_.size()-1].end = end_point_pairs[end_point_pairs.size()-1].point.y;
}

//API functions
//ProcessQuery
vector<Segment> WL::ProcessQuery(const Y y) {
  vector<Segment> answer;
  int low = 0;
  int high = WL_.size() - 1;
  int mid;

  //edge case
  if(y < WL_[high].end) return answer;
  
  //if(y >= WL_[low].start) mid = 0;
  
  while(low <= high) {
    mid = (low+high)/2;
    if(WL_[mid].start > y && WL_[mid].end <= y) break;//mid is where y resides in
    if(WL_[mid].start <= y) high = mid-1;
    else low = mid+1;
  }
  
  Window answer_window = WL_[mid];
  for(int i = 0; i < answer_window.segment_list.size(); i++) {
    if((answer_window.segment_list[i]->start_point).y > y && (answer_window.segment_list[i]->end_point).y <= y)
      answer.push_back(*(answer_window.segment_list[i]));
  }
  return answer;
}

















