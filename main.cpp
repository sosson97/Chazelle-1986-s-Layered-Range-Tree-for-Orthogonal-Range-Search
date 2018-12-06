//main.cpp
//read points from text file and query it to LRT

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "point.h"
#include "lrt.h"


using namespace std;

int main(int argc, char* argv[]){
  
  //const string file_name = argv[1];
  const string file_name = "input500.txt";
  ifstream input_stream(file_name.c_str());
  if(!input_stream.good()) {
    cerr << file_name << " does not exist." << endl;
    exit(1);
  }

  vector<Point> input;
   
  string line, x, y;
  while(getline(input_stream, line)){
    istringstream line_stream(line);
    line_stream >> x;
    line_stream >> y; 
    input.emplace_back(Point{stof(x), stof(y)});
    
  }

  cout << "Type query x1, x2, y1, y2" << endl;
  X x1, x2;
  Y y1, y2;
  
  cin >> x1 >> x2 >> y1 >> y2;

  LRT test_lrt(input);
  vector<Point> output = test_lrt.ProcessQuery(x1, x2, y1, y2);
  
  ofstream output_stream("output.txt");
  for(int i = 0; i < output.size(); i++)
    output_stream << output[i].x << " " << output[i].y << "\n";

  input_stream.close();
  output_stream.close();
  return 0;
}
