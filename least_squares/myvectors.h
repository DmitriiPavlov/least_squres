#include <string.h>
#include <stdio.h>
#include <math.h>
#ifndef myvectors_h
#define myvectors_h

#define DEFAULT_LENGTH 10

// all of this is just defining some custom data structures to hold our information
struct Point2D{
    long double x;
    long double y;
};
typedef struct Point2D Point2D;

struct PointArray{
    Point2D * array;
    int length;
    int current_pointer;
};
typedef struct PointArray PointArray;

//initialized the array with either a user given length, or just the default length
PointArray * initPointArray(int length){
    if (length == 0){
        length = DEFAULT_LENGTH;
    }
    PointArray * new_array = (PointArray *)malloc(sizeof(PointArray));
    new_array->array = (Point2D *) malloc(sizeof(Point2D)* length);
    new_array->length = length;
    new_array->current_pointer = 0;
    
    return new_array;
}

Point2D initPoint(long double x , long double y){
    Point2D mypoint = {x,y};
    return mypoint;
}

//adds a point to the array, also needs to check if the array has exceeded maximum size
//just implementation of a dynamically sized array / vector in C++


/* adds a point to the list - if there is no */
void push(PointArray * point_array, Point2D point){
    
    if (point_array->length == point_array->current_pointer){
        
        Point2D * new_address = (Point2D *) malloc(sizeof(Point2D) * point_array->length*2);
        
        memcpy(new_address, point_array->array, sizeof(Point2D) * point_array->length);
        
        free(point_array->array);
        
        point_array->array = new_address;
        
        point_array->length = point_array->length*2;
        
    }
    
    point_array->array[point_array->current_pointer] = point;
    
    point_array->current_pointer++;
    
}

//removes the element of the array at the given index
void pop(PointArray * point_array, int index){
    point_array->current_pointer--;
    
    for (int i = index; i<point_array->current_pointer; i++){
        point_array->array[i] = point_array->array[i+1];
    }
}


double distance(Point2D point1, Point2D point2){
    return sqrt((point2.x-point1.x) * (point2.x - point1.x) + (point2.y - point1.y) * (point2.y-point1.y));
}
#endif
