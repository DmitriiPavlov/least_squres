#include "mymatrices.h"
#include "myvectors.h"
#include "raylib.h"
#include <math.h>
#ifndef MY_SETTINGS
#define MY_SETTINGS
#define WIDTH 1000
#define HEIGHT 600
#define CIRCLE_RADIUS 5
#define DRAW_COLOR BLACK
#define DIVISIONS 100
#endif
#ifndef myregressions_h
#define myregressions_h

//custom exponent function so we don't overflow, slow growth on purpose
long double myexp(long double n){
    return pow(1.008L, n);
}

//All of the Reg functions return a matrix with regression result (result of least squares computation)
//All of the Eval functions return a y value for a given x value and regression result'
//All of the Rcalc functions return r squared (adjusted) for a given data set of points and a regression
//All of the RcalcNotAdj functions return r squared not adjusted
Mat * CircleReg(PointArray * point_array){
    Mat * A_matrix = Mcreate(3,point_array->current_pointer,0);
    for (int i = 0; i<point_array->current_pointer;i++){
        set(A_matrix,0,i, point_array->array[i].x);
        set(A_matrix,1,i, point_array->array[i].y);
        set(A_matrix,2,i,1);
    }
    
    Mat * b_matrix = Mcreate(1,point_array -> current_pointer,0);
    for (int i = 0; i<point_array->current_pointer; i++){
        set(b_matrix,0,i, -pow(point_array->array[i].x,2) - pow(point_array->array[i].y,2));
    }
    
    Mat * A_transposed = Mcopy(A_matrix); transpose(A_transposed);
    
    Mat * left_product = matmul(A_transposed, A_matrix);
    Mat * right_product = matmul(A_transposed, b_matrix);
    
    Mat * total = Mcombine(left_product,right_product);
    rref(total);
    
    
    Mfree(left_product); Mfree(right_product); Mfree(A_transposed); Mfree(b_matrix); Mfree(A_matrix);
    
    return total;
}

Mat * LinearReg(PointArray * point_array){
    Mat * A_matrix = Mcreate(2,point_array->current_pointer,0);
    for (int i = 0; i<point_array->current_pointer;i++){
        set(A_matrix,0,i, point_array->array[i].x);
        set(A_matrix,1,i,1);
    }
    
    Mat * b_matrix = Mcreate(1,point_array -> current_pointer,0);
    for (int i = 0; i<point_array->current_pointer; i++){
        set(b_matrix,0,i,point_array->array[i].y);
    }
    
    Mat * A_transposed = Mcopy(A_matrix); transpose(A_transposed);
    
    Mat * left_product = matmul(A_transposed, A_matrix);
    Mat * right_product = matmul(A_transposed, b_matrix);
    
    Mat * total = Mcombine(left_product,right_product);

    rref(total);
    
    Mfree(left_product); Mfree(right_product); Mfree(A_transposed); Mfree(b_matrix); Mfree(A_matrix);
    
    return total;
}

Mat * QuadraticReg(PointArray * point_array){
    Mat * A_matrix = Mcreate(3,point_array->current_pointer,0);
    for (int i = 0; i<point_array->current_pointer;i++){
        set(A_matrix,0,i, pow(point_array->array[i].x,2));
        set(A_matrix,1,i,point_array->array[i].x);
        set(A_matrix,2,i,1);
    }
    
    Mat * b_matrix = Mcreate(1,point_array -> current_pointer,0);
    for (int i = 0; i<point_array->current_pointer; i++){
        set(b_matrix,0,i,point_array->array[i].y);
    }
    
    Mat * A_transposed = Mcopy(A_matrix); transpose(A_transposed);
    
    Mat * left_product = matmul(A_transposed, A_matrix);
    Mat * right_product = matmul(A_transposed, b_matrix);
    
    Mat * total = Mcombine(left_product,right_product);
    rref(total);
    
    
    Mfree(left_product); Mfree(right_product); Mfree(A_transposed); Mfree(b_matrix); Mfree(A_matrix);
    
    return total;
    
}

Mat * LogReg(PointArray* point_array){
    Mat * A_matrix = Mcreate(2,point_array->current_pointer,0);
    for (int i = 0; i<point_array->current_pointer;i++){
        set(A_matrix,0,i, log(point_array->array[i].x));
        set(A_matrix,1,i,1);
    }
    
    Mat * b_matrix = Mcreate(1,point_array -> current_pointer,0);
    for (int i = 0; i<point_array->current_pointer; i++){
        set(b_matrix,0,i,point_array->array[i].y);
    }
    
    Mat * A_transposed = Mcopy(A_matrix); transpose(A_transposed);
    
    Mat * left_product = matmul(A_transposed, A_matrix);
    Mat * right_product = matmul(A_transposed, b_matrix);
    
    Mat * total = Mcombine(left_product,right_product);
    rref(total);
    
    
    Mfree(left_product); Mfree(right_product); Mfree(A_transposed); Mfree(b_matrix); Mfree(A_matrix);
    
    return total;
    
}

Mat * ExpReg(PointArray * point_array){
    Mat * A_matrix = Mcreate(2,point_array->current_pointer,0);
    for (int i = 0; i<point_array->current_pointer;i++){
        set(A_matrix,0,i, myexp(point_array->array[i].x));
        set(A_matrix,1,i,1);
    }
    
    Mat * b_matrix = Mcreate(1,point_array -> current_pointer,0);
    for (int i = 0; i<point_array->current_pointer; i++){
        set(b_matrix,0,i,point_array->array[i].y);
    }
    
    Mat * A_transposed = Mcopy(A_matrix); transpose(A_transposed);
    
    Mat * left_product = matmul(A_transposed, A_matrix);
    Mat * right_product = matmul(A_transposed, b_matrix);
    
    Mat * total = Mcombine(left_product,right_product);
    rref(total);
    
    
    Mfree(left_product); Mfree(right_product); Mfree(A_transposed); Mfree(b_matrix); Mfree(A_matrix);

    return total;
    
}
long double EvalLogReg(Mat * result, long double x_val){
    long double a = get(result,2,0);
    long double b = get(result,2,1);
    return a * log(x_val) + b;
}

long double EvalQuadraticReg(Mat * result, long double x_val){
    long double A = get(result,3,0);
    long double B = get(result,3,1);
    long double C = get(result,3,2);
    
    return A * pow(x_val,2) + B * x_val + C;
    
}

long double EvalExpReg(Mat * result, long double x_val){
    long double A = get(result,2,0);
    long double B = get(result,2,1);
    return A * myexp(x_val) + B;
    
}
void DrawCircularReg(Mat * result){
    
    long double A = get(result,3,0);
    long double B = get(result,3,1);
    long double C = get(result,3,2);
    
    long double center_x = -A/2;
    long double center_y = -B/2;
    long double radius = sqrt(pow(A/2,2) + pow(B/2,2) - C);
    
    DrawCircleLines(center_x,center_y,radius,DRAW_COLOR);

}

void DrawLinearReg(Mat * result){
    long double slope = get(result,2,0);
    long double y_intercept = get(result,2,1);
    DrawLine(0,y_intercept,WIDTH,WIDTH*slope+y_intercept,DRAW_COLOR);
}

void DrawQuadraticReg(Mat * result){
    for (int x = 0; x < WIDTH; x++){
        DrawLine(x -1 ,EvalQuadraticReg(result, x-1),
                 x,EvalQuadraticReg(result,x),DRAW_COLOR);
    }
}

void DrawLogReg(Mat * result){
    for (int x = 0; x < WIDTH; x++){
        DrawLine(x -1 ,EvalLogReg(result, x-1),
                 x,EvalLogReg(result,x),DRAW_COLOR);
    }
}

void DrawExpReg(Mat * result){
    for (int x = 0; x < WIDTH; x++){
        DrawLine(x -1 ,EvalExpReg(result, x-1),
                 x,EvalExpReg(result,x),DRAW_COLOR);
    }
}



long double EvalCircleRegTop(Mat * result, long double x_val){
    long double A = get(result,3,0);
    long double B = get(result,3,1);
    long double C = get(result,3,2);
    
    long double center_x = -A/2;
    long double center_y = -B/2;
    long double radius = sqrt(pow(A/2,2) + pow(B/2,2) - C);
    return sqrt(radius*radius - pow(x_val-center_x,2)) + center_y;
    // y = sqrt(radius^2 - (x-center_x)^2) + center_y
}

long double EvalCircleRegBottom(Mat * result, long double x_val){
    long double A = get(result,3,0);
    long double B = get(result,3,1);
    long double C = get(result,3,2);
    
    long double center_x = -A/2;
    long double center_y = -B/2;
    long double radius = sqrt(pow(A/2,2) + pow(B/2,2) - C);
    return -1 * sqrt(radius*radius - pow(x_val-center_x,2)) + center_y;
    // y = sqrt(radius^2 - (x-center_x)^2) + center_y
}

long double RcalcLinearReg(Mat * result, PointArray * point_array){
    
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - point_array->array[point].x*get(result,2,0)-get(result,2,1),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return 1 - (1-pow(r,2)) * (point_array->current_pointer-1) / (point_array->current_pointer - 2 -1);
    
}

long double RcalcCircularReg(Mat * result, PointArray * point_array){
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += min(pow(point_array->array[point].y - EvalCircleRegBottom(result,point_array->array[point].x),2),
                               pow(point_array->array[point].y - EvalCircleRegTop(result,point_array->array[point].x),2)
                               );
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return 1 - (1-pow(r,2)) * (point_array->current_pointer-1) / (point_array->current_pointer - result->width -1);
}

long double RcalcQuadraticReg(Mat * result, PointArray * point_array){
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - EvalQuadraticReg(result,point_array->array[point].x),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return 1 - (1-pow(r,2)) * (point_array->current_pointer-1) / (point_array->current_pointer - 3 -1);
    
}

long double RcalcLogReg(Mat * result, PointArray * point_array){
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - EvalLogReg(result,point_array->array[point].x),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return 1 - (1-pow(r,2)) * (point_array->current_pointer-1) / (point_array->current_pointer - 2 -1);
    
}

long double RcalcExpReg(Mat * result, PointArray * point_array){
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - EvalExpReg(result,point_array->array[point].x),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return 1 - (1-pow(r,2)) * (point_array->current_pointer-1) / (point_array->current_pointer - 2 -1);
    
}

long double RcalcExpRegNotAdj(Mat * result, PointArray * point_array){
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - EvalExpReg(result,point_array->array[point].x),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return r;
}

long double RcalcLogRegNotAdj(Mat * result, PointArray * point_array){
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - EvalLogReg(result,point_array->array[point].x),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return r;
    
}

long double RcalcQuadraticRegNotAdj(Mat * result, PointArray * point_array){
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - EvalQuadraticReg(result,point_array->array[point].x),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return r;
    
}

long double RcalcLinearRegNotAdj(Mat * result, PointArray * point_array){
    
    long double explained_error = 0;
    long double unexplained_error = 0;
    
    long double mean = 0;
    for (int i = 0; i<point_array->current_pointer;i++){
        mean+= point_array->array[i].y;
    }
    
    mean = mean/point_array->current_pointer;
    
    for (int point = 0; point<point_array->current_pointer; point++){
        explained_error += pow(point_array->array[point].y - point_array->array[point].x*get(result,2,0)-get(result,2,1),2);
        unexplained_error += pow(point_array->array[point].y - mean,2);
    }
    long double r = 1 - explained_error/unexplained_error;
    return r;
    
}
#endif
