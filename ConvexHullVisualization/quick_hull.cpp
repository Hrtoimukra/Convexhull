#include "geodeb.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>

typedef struct point{  
      int x;
      int y;   
}point;

int cross_product(point &a,point &b,point &c){
    return(b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);    
}

void find_hull(std::vector<point>& points,point a,point b,std::vector<point>& hull){
     int highest_point=-1;
     int maxm_distance=-1;

     for(int i=0;i<points.size();++i){
         int area=cross_product(a,b,points[i]);
         if(area>maxm_distance){
            highest_point=i;
            maxm_distance=area;   
         }    
     }

     if(maxm_distance>0){

        GD_SEGMENT(a.x,a.y,points[highest_point].x,points[highest_point].y);
        GD_SEGMENT(points[highest_point].x,points[highest_point].y,b.x,b.y); 
        find_hull(points,a,points[highest_point],hull);       
        hull.push_back(points[highest_point]);
        find_hull(points,points[highest_point],b,hull);  
     }     
}

std::vector<point> quick_hull(std::vector<point> &points){
    int n=points.size();
    if(n<3)
       return points;
    
    int minm=0;
    int maxm=0;

    for(int i=1;i<points.size();++i){
        if(points[minm].x>points[i].x)
           minm=i;
        if(points[maxm].x<points[i].x)
           maxm=i;      
    }
   
    GD_SEGMENT(points[minm].x,points[minm].y,points[maxm].x,points[maxm].y); 
    std::vector<point> hull;
    hull.push_back(points[minm]);
    find_hull(points,points[minm],points[maxm],hull);
    hull.push_back(points[maxm]);
    find_hull(points,points[maxm],points[minm],hull); 

    return hull; 
}

void runTests(int numTests, int numPoints) {
    std::vector<point> points;
    std::srand(std::time(0));

    GD_INIT("QUICKHULL.html"); 

    for (int i = 0; i < numPoints; i++)
        points.push_back({static_cast<float>(std::rand() % 1000), static_cast<float>(std::rand() % 1000)});
   
    for(int i=0;i<points.size();++i)
        GD_POINT(points[i].x,points[i].y);

    GD_PAUSE();     

    quick_hull(points);

    auto start = std::chrono::high_resolution_clock::now();

    //for (int i = 0; i < numTests; i++)
        //quickHullCPU(points);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "CPU Time: " << duration << "ms" << std::endl;
}

int main(int argc,char **argv){
    runTests(1,100);
    return 0;   
}





