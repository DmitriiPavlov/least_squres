#pragma once

#include <stdio.h>

#include <stdlib.h>

#include <string.h>
#ifndef mymatrices

#define mymatrices


//our custom struct that holds matrices
// all the functions beginning with M are designed to operate on matrices
// with R are designed to operate on rows

struct Mat{
    
    long double
    * array;
    
    int width;
    
    int height;
};

typedef struct Mat Mat;

//just useful custom function
int min(int x , int y){
    if (x<y){
        return x;
        
    }
    return y;
}

//gets a number from a matrix at the given x,y
long double get(Mat * matrix,int x, int y){
//first we must check whether it is in bounds of the matrix
    if (x >= matrix->width || x<0){
        //for debugging
//        printf("Error, x coordinate is out of bounds\n");
        return 0;
    }
    
    if (y >= matrix->height || y<0){
        //for debugging
//        printf("Error, y coordinate is out bounds\n");
//        printf("%i",y);
//        printf("%i",matrix->height);
        return 0;
    }

//now we just have to find the position in the array of our item
    
    int position = x + y*matrix->width;
        return matrix->array[position];
}

//sets a matrix coordinate at the given x , y
void set(Mat * matrix,int x, int y, long double
         num){
    if ((x >= matrix->width )|| (x<0)){
        printf("Error, x coordinate is out of bounds on set function call");
        return;
    }
    
    if (y >= matrix->height || y<0){
        printf("Error, y coordinate is out of bounds on set function call");
        return;
    }
    
    int position = x + y * matrix->width;
    
    matrix->array[position] = num;
}


void transpose(Mat * matrix);

//prints out a matrix with the rows on separate lines (for debugging purposes)
void Mprint(Mat * matrix){
    for (int y = 0; y<matrix->height; y++){
        for (int x = 0; x<matrix->width;x++){
            printf("%Lf",get(matrix,x,y));
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

//multiplies a row of a matrix by a scalar
void Rscale(Mat * matrix, int row, long double
            number){
    
    for (int column = 0; column < matrix->width; column++){
        
        long double
        current_num = get(matrix,column,row);
        set(matrix,column,row,current_num*number);
    }
    
}

//swaps two rows of a matrix
void Rswap(Mat * matrix, int row1, int row2){
    for (int column = 0; column<matrix->width; column++){
        long double
        row1num = get(matrix,column,row1);
        long double
        row2num = get(matrix,column,row2);
        set(matrix,column,row1,row2num);
        set(matrix,column,row2,row1num);
    }
}

//subtracts row 2 from row 1
void Rsubtract(Mat * matrix, int row1 , int row2){
    for (int column = 0; column<matrix->width; column++){
        long double
        row1num = get(matrix,column,row1);
        long double
        row2num = get(matrix,column,row2);
        set(matrix,column,row1,row1num - row2num);
    }
}

//divides the row by the first non zero integer in it, making the first non zero integer one
// the x and y coordinate of this integer needs to be passed in
void Ronescale(Mat * matrix, int row, int column){
    
    long double
    current_num = get(matrix,column,row);
    if (current_num!=0){
        long double
        scale_factor = 1/current_num;
        Rscale(matrix,row,scale_factor);
    }
    
}
//"cancels" out a row with another row, using elementary matrix operation
void Rcancel(Mat * matrix, int row1, int row2,int column){
    long double
    row1_num = get(matrix,column,row1);
    long double
    row2_num = get(matrix,column,row2);
    Rscale(matrix,row2,row1_num/row2_num);
    Rsubtract(matrix,row1,row2);
    Rscale(matrix,row2,row2_num/row1_num);
    
}

//takes a matrix and performs an rref operation on it using custom algorithm
//we use this instead of inverting matrices, because inverting is memory expensive
void rref(Mat * matrix){
//we loop through all the rows and columns
//we loop until we are outh of bounds on either the row or the column
    bool outofBounds = false;
    int row = 0;
    int column = 0;
    while(!outofBounds){
        //break conditiions
        if (row >= matrix->height || column >= matrix->width){
            outofBounds = true;
            break;
        }
        
        //we check if there is a a number with a leading non zero number
        //we bring that row to the very top
        for (int current_row = row; current_row < matrix->height; current_row++){
            if (get(matrix,column,current_row) != 0){
                Rswap(matrix, current_row, row);
                Ronescale(matrix, row, column);
                break;
            }
        }
        //we now loop through the row until we find a non zero number
        if (get(matrix,column,row) == 0){
            column ++;
        }
        
        //we now uses our chosen row to "cancel" out the rest of the rows so that its a leading one
        else{
            for (int current_row = 0; current_row < matrix->height; current_row++){
                if (current_row!= row && get(matrix,column,current_row) != 0){
                    Rcancel(matrix,current_row,row,column);
                }
            }
            row ++;
            column ++;
        }
    }
    
    
}

//matrix struct constructor
Mat * Mcreate(int width, int height, long double default_value){
    Mat * matrix = (Mat *) malloc(sizeof(Mat));
    matrix->width = width;
    matrix->height = height;
    long double* array = (long double*)malloc(sizeof(long double)*width*height);
    matrix->array = array;
    for (int x = 0; x < width; x++){
        for (int y = 0; y<height; y++){
            set(matrix,x,y,default_value);
        }
    }
    
    return matrix;
}

//creates an identity matrix thats widthxwidth
Mat * identity(int width){
    Mat * matrix = Mcreate(width,width,0);
    
    for (int i = 0 ; i<width;i++){
        set(matrix,i,i,1);
    }
    return matrix;
}

//creates a new matrix with the result of the matrix multiplication of two other matrices
Mat * matmul(Mat * mat1, Mat * mat2){
    if (mat1->width!=mat2->height){
        puts("Dimension mismatch error when multiplying two matrices:\n");
        Mprint(mat1);
        Mprint(mat2);
    }
    Mat * product = Mcreate(mat2->width, mat1->height,0);
    
    for (int row = 0; row <product->height; row++){
        
        for (int column = 0; column < product->width; column++){
            
            float sum = 0;
            
            for (int index = 0; index < mat1->width; index ++){
                
                sum += get(mat1,index,row) * get(mat2,column,index);
            }
            
            set(product,column,row,sum);
        }
    }
    return product;
    
}
//frees the memory associated with a matrix
//NO MEMORY LEAKS

void Mfree(Mat * matrix){
    free(matrix->array);
    free(matrix);
}

//copies all the contents of a matrix into a new one

Mat * Mcopy(Mat * old_matrix){
    Mat * new_matrix = Mcreate(old_matrix->width, old_matrix->height, 0);
    
    new_matrix->array = (long double *) memcpy(new_matrix->array, old_matrix->array,sizeof(long double) * old_matrix->width* old_matrix->height);
    
    return new_matrix;
}

//adds a new column to a matrix with all 0 's (useful for our least squares method)
Mat * addColumn(Mat * matrix){
    Mat * new_matrix = Mcreate(matrix->width+1, matrix->height, 0);
    
    for (int x = 0; x<matrix->width; x++){
        for (int y = 0; y<matrix->height; y++){
            set(new_matrix,x,y,get(matrix,x,y));
        }
    }
    Mfree(matrix);
    return new_matrix;
}

//matrix constructor from given array
Mat * Minit(long double *array, int width, int height){
    Mat * matrix = (Mat *) malloc(sizeof(Mat));
    matrix->array = array;
    matrix->width = width;
    matrix->height = height;
    return matrix;
}

//takes a pointer to a matrix, transposes it
void transpose(Mat * matrix){
    Mat * original_matrix = Mcopy(matrix);
    matrix->width = original_matrix ->height;
    matrix->height = original_matrix->width;
    
    for (int x = 0; x<original_matrix->width; x++){
        for (int y = 0; y<original_matrix->height; y++){
            set(matrix,y,x,get(original_matrix,x,y));
        }
    }
    Mfree(original_matrix);
}

//joins two matrices together along their rows
Mat * Mcombine(Mat * matrix1, Mat * matrix2){
    Mat * new_matrix = Mcreate(matrix1->width+matrix2->width, matrix1->height,0);
    for (int x= 0; x<matrix1->width; x++){
        for (int y = 0; y< matrix1->height; y++){
            set(new_matrix,x,y,get(matrix1,x,y));
        }
    }
    
    for(int x = matrix1->width; x< new_matrix->width; x++){
        for (int y = 0; y< matrix1->height; y++){
            set(new_matrix,x,y,get(matrix2,x - matrix1->width,y));
        }
    }
    return new_matrix;
}

#endif

