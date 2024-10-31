#include "raylib.h"
#include "mymatrices.h"
#include "myvectors.h"
#include "myregressions.h"
#include <math.h>
// some macros to define the size of each circle and the color of the equation
#define CIRCLE_RADIUS 4
#define DRAW_COLOR BLACK

Vector2 convertToVector(Point2D point){
    Vector2 vec = {point.x,point.y};
    return vec;
}

Point2D convertToPoint(Vector2 vector){
    Point2D point = {(long double)vector.x, (long double)vector.y};
    return point;
}

/* given a mouse click as input, either adds the location clicked as a point to the array indicating a circle, or
 deletes an existing point in the array*/
//CIRCLE RADIUS is predefined to be 4
void handleMouseClick(PointArray * point_array, Point2D mouse_location){
    bool broken = false;
    //iterates through and determines whether the click was clicking a point
    for (int i = 0; i<point_array->current_pointer; i++){
        if (distance(mouse_location,point_array->array[i]) < CIRCLE_RADIUS){
            pop(point_array,i);
            broken = true;
            return;
        }
        
    }
    //if a point wasn't deleted, adds a point
    if (!broken){
        push(point_array,initPoint(mouse_location.x,mouse_location.y));
    }
    
    
}

int main(void) {
    //initializing raylib
    int listOfNumbers[] = {1,23,3,5,4,3,5,5,5,5,5,5,5,5,5};
    //creating the point_array, and adding some default points to it that can later be deleted
    PointArray *  point_array = initPointArray(1);
    push(point_array,initPoint(WIDTH/2,HEIGHT/2));
    push(point_array,initPoint(WIDTH/2+50,HEIGHT/2+50));
    push(point_array,initPoint(WIDTH/2-50,HEIGHT/2-50));
    push(point_array,initPoint(WIDTH/2-100,HEIGHT/2-100));
    push(point_array,initPoint(WIDTH/2-150,HEIGHT/2-150));
    Mat * regressions[5];
    InitWindow(WIDTH, HEIGHT, "Real time, regression demonstration");
    ClearBackground(RAYWHITE);
    int index = 0;
    SetTargetFPS(20);
    
    // indicates whether the button has been pressed to activate the regression
    bool linearEnabled = true;
    bool expEnabled = true;
    bool logEnabled = true;
    bool quadraticEnabled = true;
    
    //main loop
    while (!WindowShouldClose()){
        //clear screen
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        //get mouse position, create structs on the stack for the 4 circles
        Vector2 mouse_location = GetMousePosition();
        Point2D mouse_point = convertToPoint(mouse_location);
        Point2D linearActiveCircle = initPoint(WIDTH-20,50);
        Point2D expActiveCircle = initPoint(WIDTH-20,80);
        Point2D logActiveCircle = initPoint(WIDTH-20,110);
        Point2D quadraticActiveCircle = initPoint(WIDTH-20,140);
        
        
        //creates a struct with the 5 matrices that are produced by the regressions
        Mat * regressions[5] = {
            LinearReg(point_array),
            QuadraticReg(point_array),
            CircleReg(point_array),
            LogReg(point_array),
            ExpReg(point_array)
        };
        
        //computes the r squared value (adjusted)
        long double rvalues[5] = {
            RcalcLinearReg(regressions[0],point_array) * linearEnabled,
            RcalcQuadraticReg(regressions[1],point_array) * quadraticEnabled,
            //                RcalcCircularReg(regressions[2],point_array),
            //sets the circular r squared value to 0 by default
            0,
            RcalcLogReg(regressions[3],point_array)*logEnabled,
            RcalcExpReg(regressions[4],point_array)*expEnabled
        };
        
        //computed the r squared value not adjusted
        long double rvaluesnotadj[5] = {
            RcalcLinearRegNotAdj(regressions[0],point_array),
            RcalcQuadraticRegNotAdj(regressions[1],point_array),
            //            RcalcCircularReg(regressions[2],point_array),
            0,
            RcalcLogRegNotAdj(regressions[3],point_array),
            RcalcExpRegNotAdj(regressions[4],point_array)
        };
        
        //on left mouse click, does everything
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            bool clickedElsewhere = false;
            //we are going to check if it was within any of our circles to activate/ deactive the regressions
            if (distance(mouse_point,linearActiveCircle)<10){
                linearEnabled = !linearEnabled;
                clickedElsewhere = true;
            }
            if (distance(mouse_point,quadraticActiveCircle)<10){
                quadraticEnabled = !quadraticEnabled;
                clickedElsewhere = true;
            }
            if (distance(mouse_point,expActiveCircle)<10){
                expEnabled = !expEnabled;
                clickedElsewhere = true;
            }
            if (distance(mouse_point,logActiveCircle)<10){
                logEnabled = !logEnabled;
                clickedElsewhere = true;
            }
            
            
            /* our procedure is being called here */
            if (!clickedElsewhere){
                handleMouseClick(point_array, mouse_point);
            }

            
            //makes sure the allocated memory is freed on the eap
            for (int i = 0; i<5; i++){
                Mfree(regressions[i]);
            }
            
            //allocated more memory/ creates more matrices with the results of the regressions
            Mat * regressions[5] = {
                LinearReg(point_array),
                QuadraticReg(point_array),
                CircleReg(point_array),
                LogReg(point_array),
                ExpReg(point_array)
            };
            
            //computes r squared (adjusted)
            long double rvalues[5] = {
                RcalcLinearReg(regressions[0],point_array) * linearEnabled,
                RcalcQuadraticReg(regressions[1],point_array) * quadraticEnabled,
                //                RcalcCircularReg(regressions[2],point_array),
                0,
                RcalcLogReg(regressions[3],point_array)*logEnabled,
                RcalcExpReg(regressions[4],point_array)*expEnabled
            };
            
            //computes r squared
            long double rvaluesnotadj[5] = {
                RcalcLinearRegNotAdj(regressions[0],point_array),
                RcalcQuadraticRegNotAdj(regressions[1],point_array),
                //            RcalcCircularReg(regressions[2],point_array),
                0,
                RcalcLogRegNotAdj(regressions[3],point_array),
                RcalcExpRegNotAdj(regressions[4],point_array)
            };
            
            //creates a list of boolean values
            bool enabled[5] = {
                linearEnabled,
                quadraticEnabled,
                0,
                logEnabled,
                expEnabled
            };
            
            //iterates through and finds the maximum r squared (adjusted) value
            index = 0;
            long double r_value = -100000000;
            bool found = false;
            for (int i = 0; i<5; i++){
                if (rvalues[i] > r_value && enabled[i]){
                    r_value = rvalues[i];
                    index = i;
                    found = true;
                }
            }
            
            
            //chooses the regression with the best r squared (adjusted) value
            //plots the regression along with the type of regression and the r squared value
            if (point_array->current_pointer >3 && found){
                switch(index){
                    case 0:
                        DrawLinearReg(regressions[0]);
                        DrawText("Linear",10,0,40,BLACK);
                        DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                        break;
                    case 1:
                        DrawQuadraticReg(regressions[1]);
                        DrawText("Quadratic",10,0,40,BLACK);
                        DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                        break;
                    case 2:
                        DrawCircularReg(regressions[2]);
                        DrawText("Circular",10,0,40,BLACK);
                        DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,10,BLACK);
                        break;
                    case 3:
                        DrawLogReg(regressions[3]);
                        DrawText("Logarithmic",10,0,40,BLACK);
                        DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                        break;
                    case 4:
                        DrawExpReg(regressions[4]);
                        DrawText("Exponential",10,0,40,BLACK);
                        DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                        break;
                    default:
                        DrawLinearReg(regressions[0]);
                        DrawText("Linear",10,0,40,BLACK);
                        DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                        break;
                }
            }
        }
        //clears the point_array by simply just setting the current pointer to 0
        if (IsKeyDown(KEY_BACKSPACE)){
            point_array->current_pointer = 0;
        }
        
        //plots all the point from the point array
        for (int i = 0; i<point_array->current_pointer; i++){
            DrawCircle(point_array->array[i].x,point_array->array[i].y,CIRCLE_RADIUS,BLUE);
        }
        
        //plots the proper regression
        if (point_array->current_pointer >3 && rvalues[index] !=0){
            switch(index){
                case 0:
                    DrawLinearReg(regressions[0]);
                    DrawText("Linear",10,0,40,BLACK);
                    
                    DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                    break;
                case 1:
                    DrawQuadraticReg(regressions[1]);
                    DrawText("Quadratic",10,0,40,BLACK);
                    DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                    break;
                case 2:
                    DrawCircularReg(regressions[2]);
                    DrawText("Circular",10,0,40,BLACK);
                    DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                    break;
                case 3:
                    DrawLogReg(regressions[3]);
                    DrawText("Logarithmic",10,0,40,BLACK);
                    DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                    break;
                case 4:
                    DrawExpReg(regressions[4]);
                    DrawText("Exponential",10,0,40,BLACK);
                    DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                    break;
                default:
                    DrawLinearReg(regressions[0]);
                    DrawText("Linear",10,0,40,BLACK);
                    DrawText(TextFormat("R-squared : %Lf",rvaluesnotadj[index]),10,40,20,BLACK);
                    break;
                    
            }
        }
        
        // now we are going to write some of the code for enabling and disabling the regressions
        
        //draws a green circle if its active, a red circle if its not active
        DrawText(TextFormat("Linear"),WIDTH-100,40,20,BLACK);
        if (linearEnabled){
            DrawCircle(WIDTH-20,50,10,GREEN);
        }
        else {
            DrawCircle(WIDTH-20,50,10,RED);
        }
        
        DrawText(TextFormat("Exponential"),WIDTH-160,70,20,BLACK);
        if (expEnabled){
            DrawCircle(WIDTH-20,80,10,GREEN);
        }
        else {
            DrawCircle(WIDTH-20,80,10,RED);
        }
        
        DrawText(TextFormat("Logarithmic"),WIDTH-150,100,20,BLACK);
        if (logEnabled){
            DrawCircle(WIDTH-20,110,10,GREEN);
        }
        else {
            DrawCircle(WIDTH-20,110,10,RED);
        }
        
        DrawText(TextFormat("Quadratic"),WIDTH-140,130,20,BLACK);
        if (quadraticEnabled){
            DrawCircle(WIDTH-20,140,10,GREEN);
        }
        else {
            DrawCircle(WIDTH-20,140,10,RED);
        }
        DrawText("Click to enable and disable regression types",WIDTH-460,10,20,BLACK);
        DrawText("Backspace to clear board, click to create and delete points",10,HEIGHT-20,20,BLACK);
        EndDrawing();
    }
    
    
    CloseWindow();
}
