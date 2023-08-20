#include "geodeb.h"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;

std::pair<int,int> mid;

int quadrant(pair<int,int> pt){  
    if(pt.first>=0 && pt.second>=0)
      return 1;
    else if(pt.first<0 && pt.second>=0)
      return 2;
    else if(pt.first<0 && pt.second <0)
      return 3;
    else
      return 4;         	  
}

int turn(pair<int,int> a,pair<int,int> b,pair<int,int> c){  
    int area=(b.second-a.second)*(c.first-b.first)-(c.second-b.second)*(b.first-a.first);  	
    if(area==0)
       return 0;
    else if(area>0)
       return 1;
    else
       return -1;        
}

bool compare(pair<int,int> p1,pair<int,int> q1){
   pair<int,int> p={p1.first-mid.first,p1.second-mid.second};
   pair<int,int> q={q1.first-mid.first,q1.second-mid.second};

   int x=quadrant(p);
   int y=quadrant(q);

   if(x!=y)
     return x<y;
   return p.second*q.first < q.second*p.first;      
}

vector<pair<int,int>> merge_convex_hull(vector<pair<int,int>> left_hull,vector<pair<int,int>> right_hull){
   
   int left_size=left_hull.size();
   int right_size=right_hull.size();

   int idx=0;
   int right_most_idx;
   int left_most_idx;

   for(int i=1;i<left_size;++i){
       if(left_hull[i].first>left_hull[idx].first)
          idx=i; 	
   } 

   right_most_idx=idx;
   idx=0;

   for(int i=1;i<right_size;++i){
       if(right_hull[i].first<right_hull[idx].first)
          idx=i; 
   }

   left_most_idx=idx;

   int idx_l=right_most_idx;
   int idx_r=left_most_idx;

   bool done=false; 
  
   while(!done){
        done=true;
        while(turn(right_hull[idx_r],left_hull[idx_l],left_hull[(idx_l+1)%left_size])>=0)  
        	  idx_l=(idx_l+1)%left_size;
        while(turn(left_hull[idx_l],right_hull[idx_r],right_hull[(right_size+idx_r-1)%right_size])<=0){
        	  idx_r=(right_size+idx_r-1)%right_size;
        	  done=false;
        }  	
   } 

   int top_left=idx_l;
   int top_right=idx_r;

   idx_l=right_most_idx;
   idx_r=left_most_idx;

   done=false;

   while(!done){    
        done=true;
   	    while(turn(left_hull[idx_l],right_hull[idx_r],right_hull[(idx_r+1)%right_size])>=0)
   	    	 idx_r=(idx_r+1)%right_size;
        while(turn(right_hull[idx_r],left_hull[idx_l],left_hull[(left_size+idx_l-1)%left_size])<=0){
             idx_l=(idx_l-1+left_size)%left_size;
             done=false;    
        } 	    	
   } 

   int bottom_left=idx_l;
   int bottom_right=idx_r;

   vector<pair<int,int>> final_hull;

   int left_hull_points=top_left;
   final_hull.push_back(left_hull[left_hull_points]);

   while(left_hull_points!=bottom_left){
        left_hull_points=(left_hull_points+1)%left_size;
        final_hull.push_back(left_hull[left_hull_points]); 
   } 

   int right_hull_points=bottom_right;
   final_hull.push_back(right_hull[right_hull_points]);

   while(right_hull_points!=top_right){
        right_hull_points=(right_hull_points+1)%right_size;
        final_hull.push_back(right_hull[right_hull_points]);  
   }   

   return final_hull;
}

vector<pair<int,int>> brute_convex_hull(vector<pair<int,int>> points){
   set<pair<int,int>> st;
    
   for(int i=0;i<points.size();++i){
       for(int j=i+1;j<points.size();++j){

           int left_points=0;
           int right_points=0;

           for(int k=0;k<points.size();++k){
               
               int a_x=points[i].first;
               int a_y=points[i].second;

               int b_x=points[j].first;
               int b_y=points[j].second;

               int c_x=points[k].first;
               int c_y=points[k].second;


               int area=(b_x-a_x)*(c_y-a_y)-(c_x-a_x)*(b_y-a_y);

               if(area<=0)
                  left_points++;
               if(area>=0)
                  right_points++;            
           }   

           if(left_points==points.size() || right_points==points.size()){
              st.insert(points[i]);
              st.insert(points[j]);  
           }   
       }  
   }

  vector<pair<int,int>> sorted_points;

  for(set<pair<int,int>>::iterator it=st.begin();it!=st.end();++it)
     sorted_points.push_back(*it);

  mid={0,0};
  int n=sorted_points.size();

  for(int i=0;i<n;++i){

     mid.first+=sorted_points[i].first;
     mid.second+=sorted_points[i].second;

     sorted_points[i].first*=n;
     sorted_points[i].second*=n;   
  }   

  sort(sorted_points.begin(),sorted_points.end(),compare);

  for(int i=0;i<n;++i)
      sorted_points[i]={sorted_points[i].first/n,sorted_points[i].second/n};

  return sorted_points;
}

void display(vector<pair<int,int>>& convex_hull){
     GD_PAUSE();
     for(int i=0;i<convex_hull.size()-1;++i)
        GD_SEGMENT(convex_hull[i].first,convex_hull[i].second,convex_hull[i+1].first,convex_hull[i+1].second);
     GD_SEGMENT(convex_hull[convex_hull.size()-1].first,convex_hull[convex_hull.size()-1].second,convex_hull[0].first,convex_hull[0].second); 
}

vector<pair<int,int>> divide(vector<pair<int,int>> points){
     if(points.size()<=5) 
        return brute_convex_hull(points);     

     vector<pair<int,int>> left;
     vector<pair<int,int>> right;

     for(int i=0;i<points.size()/2;++i)
         left.push_back(points[i]);
     for(int i=points.size()/2;i<points.size();++i)
         right.push_back(points[i]);


     vector<pair<int,int>> left_convex_hull=divide(left);
     display(left_convex_hull);                    

     vector<pair<int,int>> right_convex_hull=divide(right);
     display(right_convex_hull);

     vector<pair<int,int>> convex_hull=merge_convex_hull(left_convex_hull,right_convex_hull);

     return convex_hull;                
} 

void runTests(int numTests,int numPoints){

     std::vector<pair<int,int>> points;
     std::srand(std::time(0));

     for(int i = 0; i < numPoints; i++) {
         points.push_back({static_cast<int>(std::rand() % 1000), static_cast<int>(std::rand() % 1000)});
     }

    for(int i=0;i<points.size();++i)
       GD_POINT(points[i].first,points[i].second);


    sort(points.begin(),points.end());
    vector<pair<int,int>> final_convex_hull;
    final_convex_hull=divide(points);

    display(final_convex_hull);    
}

int main(){
   GD_INIT("CONQUER.html");
   runTests(1,100);
   return 0; 
}




































































