//
//  Point2.h
//
#ifndef CIRCLE_H
#define CIRCLE_H



class Point2 {
public:
    // constructor
    Point2();
    Point2(float x, float y);


    void Print(); // setter
    int Quadrant(); // getter
    float DistanceBetween(Point2 p2); // diameter

private:
    float x;
    float y;
}; // always remember the semicolon



#endif
