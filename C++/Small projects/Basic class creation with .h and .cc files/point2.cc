#include "point2.h"
#include <iostream>
#include <cmath>

Point2::Point2(){
  x=0;
  y=0;
}

Point2::Point2(float userx, float usery){
  x = userx;
  y = usery;
}
void Point2::Print(){
  std::cout << x << "," << y<< std::endl;
}
float Point2::DistanceBetween(Point2 p2){
  return sqrt((x-p2.x)*(x-p2.x)+(y-p2.y)*(y-p2.y));
}
int Point2::Quadrant(){
  if (x>0 && y>=0){
    return 1;
  }
  else if (x<=0 && y>0){
    return 2;
  }
  else if (x<0 && y<=0){
    return 3;
  }
  else if (x>=0 && y<0){
    return 4;
  }
  else{
    return 0;
  }
}
