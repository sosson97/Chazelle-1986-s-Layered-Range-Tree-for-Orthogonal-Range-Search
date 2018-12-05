//point.h
//define Point class and Segment class


#ifndef POINT_H_
#define POINT_H_

typedef float X;
typedef float Y;


typedef struct{
  X x;
  Y y;
} Point;

typedef struct Segment{
  Point start_point;
  Point end_point;
  struct Segment* next;//downward segment 
} Segment;

#endif // POINT_H_
