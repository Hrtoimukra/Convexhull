#include "geodeb.h"

#include<iostream>
#include<vector>
#include<algorithm>  
#include<chrono>
#include<ctime>

#define LEFT -1
#define RIGHT 1
#define COLL  0

typedef struct point{
        int x;
        int y;
        bool operator <(point b){
             if(y!=b.y)
               return y < b.y;
             return  x < b.x;  	
        }	
}point;

point pivot;

void display_top_two(std::vector<point>& convex_hull){
     std::cout<<"Stack top two points"<<std::endl; 
     std::cout<<convex_hull[convex_hull.size()-1].x<<" "<<convex_hull[convex_hull.size()-1].y<<std::endl;
     std::cout<<convex_hull[convex_hull.size()-2].x<<" "<<convex_hull[convex_hull.size()-2].y<<std::endl; 
}

void display_sorted_points(std::vector<point>& points){
     std::cout<<"Sorted Points"<<std::endl;
     for(int i=0;i<points.size();++i)
        std::cout<<points[i].x<<" "<<points[i].y<<std::endl; 
}

void display_point(point p){
     std::cout<<p.x<<" "<<p.y<<std::endl;
}

void display_convex_hull(std::vector<point>& convex_hull){
     std::cout<<" Convex Hull in counterclockwise direction"<<std::endl;
     int n=convex_hull.size();
     for(int i=0;i<n;++i)
         std::cout<<convex_hull[i].x<<" "<<convex_hull[i].y<<std::endl;      
}

int make_turn(point a,point b,point c){
	int cp=(b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y);

    std::cout<<"Top two of the stack"<<std::endl;
    display_point(b);
    display_point(a);

    std::cout<<"Third point"<<std::endl;
    display_point(c);

    std::cout<<"Cross Product of the points="<<cp<<std::endl; 

	if(cp>0)
	  return LEFT;
	else if(cp<0)
	  return RIGHT;
	else
	  return COLL;          
}

int sqre_dist(point a,point b){
    int dx=b.x-a.x;
    int dy=b.y-a.y;
    return dx*dx+dy*dy;
}

bool rotational_sweep(point a,point b){
     int turn=make_turn(pivot,a,b);
     if(turn==0)
     	return sqre_dist(pivot,a) < sqre_dist(pivot,b);
     if(turn==LEFT)
        return true;
     else
        return false;      	  
}

int get_lowest_point(std::vector<point>& points){
    int n=points.size();
    int low=0;
    for(int i=1;i<n;++i){
        if(points[i]<points[low])
           low=i;  	
    }
    return low;
}

std::vector<point> graham_scan(std::vector<point>& points){
    std::vector<point> convex_hull;
    int n=points.size();

    if(n<3)
       return convex_hull;
    
    int low=get_lowest_point(points);
    
    point temp=points[0];
    points[0]=points[low];
    points[low]=temp;

    pivot=points[0];

    std::cout<<"The Pivot point "<<pivot.x<<" "<<pivot.y<<std::endl;
   
    sort(points.begin()+1,points.end(),rotational_sweep);

    convex_hull.push_back(points[0]);
    convex_hull.push_back(points[1]);
    convex_hull.push_back(points[2]);

    display_sorted_points(points); 

    for(int i=3;i<n;++i){
        while(convex_hull.size()>1 && make_turn(convex_hull[convex_hull.size()-2],convex_hull[convex_hull.size()-1],points[i])!=LEFT){
              //dispplay_top_two(convex_hull);
              convex_hull.pop_back(); 
        }
        convex_hull.push_back(points[i]);   
        //display_top_two(convex_hull);   
    } 

    display_convex_hull(convex_hull);
    return convex_hull;   
}


void runTests(int numTests, int numPoints) {
    std::vector<point> points;
    std::srand(std::time(0));

    for (int i = 0; i < numPoints; i++) 
        points.push_back({static_cast<int>(std::rand() % 1000), static_cast<int>(std::rand() % 1000)});
    
    auto start = std::chrono::high_resolution_clock::now();

    GD_INIT("debug.html"); 

    for(int i=0;i<points.size();++i)
       GD_POINT(points[i].x,points[i].y);
  
    GD_PAUSE();
    std::vector<point> ch;    
     
    ch=graham_scan(points);

    for(int i=0;i<ch.size()-1;++i)
       GD_SEGMENT(ch[i].x,ch[i].y,ch[i+1].x,ch[i+1].y);
       
    GD_SEGMENT(ch[ch.size()-1].x,ch[ch.size()-1].y,ch[0].x,ch[0].y);     


   // for (int i = 0; i < numTests; i++)
        //graham_scan(points);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "CPU Time: " << duration << "ms" << std::endl;
}

int main(int argc,char **argv){

    int numTests=100;
    int numPoints=1000000; 

    runTests(1,100);   
  
    std::vector<point> points={{1,0},{2,0},{3,0},{4,0},{5,0},{5,5},{4,4},{3,3},{2,2},{1,1}};
     

    return 0;             
}

















 


