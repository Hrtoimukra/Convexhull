#include<cassert>
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<iostream>
#include<fstream>

using namespace std;

typedef long double ftype;

struct vec3{
       ftype x,y,z;

       vec3(ftype x=0,ftype y=0,ftype z=0):x(x),y(y),z(z){
       }

       vec3 operator-(const vec3 &o) const{
       	    return vec3(x-o.x,y-o.y,z-o.z);
       } 	

       vec3 cross(const vec3 &o) const{
       	    return vec3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
       }

       ftype dot(const vec3 &o) const{
            return x*o.x +y*o.y +z*o.z; 	
       }
};

struct face{
	  int a,b,c;
	  vec3 n;  
};

const ftype EPS=1e-9;

void add_face(vector<face>& f,vector<vec3>& p,vector<vector<bool>>& dead,int a,int b,int c){
     f.push_back({a,b,c,(p[b]-p[a]).cross(p[c]-p[a])});
     dead[a][b]=dead[b][c]=dead[c][a]=false; 
}

vector<face> hull3(vector<vec3> &p){
    int n=p.size();
    assert(n>=3);
    vector<face> f;
    vector<vector<bool>> dead(n,vector<bool>(n,true));

    add_face(f,p,dead,0,1,2);
    add_face(f,p,dead,0,2,1);

    for(int i=3;i<n;++i){

        vector<face> f2;

        for(vector<face>::iterator it=f.begin();it!=f.end();++it){
            struct face F=*it;
            if((p[i] - p[F.a]).dot(F.n) > EPS)
               dead[F.a][F.b]=dead[F.b][F.c]=dead[F.c][F.a]=true;
            else
               f2.push_back(F);     
        }	

        f.clear();

        for(vector<face>::iterator it=f2.begin();it!=f2.end();++it){

           struct face F=*it;

           vector<int> ind;
           ind.push_back(F.a);
           ind.push_back(F.b);
           ind.push_back(F.c);

           for(int j=0;j<3;++j){

               int a=ind[j];
               int b=ind[(j+1)%3];

               if(dead[b][a])
                  add_face(f,p,dead,b,a,i);	
           }  	
        }

        f.insert(f.end(),f2.begin(),f2.end());
    } 

    return f;
}


ftype frand(ftype low,ftype high){
    ftype  f = (ftype)rand() / RAND_MAX;
    return low + f * (high - low);
}

int main(int argc,char **argv){
   
   vector<vec3> p;

   int lower=0;
   int upper=1000;

   ftype low=(ftype)lower;
   ftype high=(ftype)upper;
   
   for(int i=0;i<1000;++i){

       ftype x=frand(low,high);
       ftype y=frand(low,high);
       ftype z=frand(low,high);
        
       vec3 v;
       v.x=x;
       v.y=y;
       v.z=z;

       p.push_back(v);  
   }

/*
   ifstream in("input2.txt");
   ftype x,y,z;

   for(int i=0;i<20702;++i){
       in>>x>>y>>z;
       vec3 v;
       v.x=x;
       v.y=y;
       v.z=z;

       p.push_back(v);
   }

 */   

   vector<face> f=hull3(p);

   FILE *fpi=fopen("Input.txt","w");
   FILE *fpt=fopen("myoutput.txt","w");

   for(int i=0;i<100;++i)
       fprintf(fpi,"%Lf\t%Lf\t%Lf\n",p[i].x,p[i].y,p[i].z);
 
   for(int i=0;i<f.size();++i){
       fprintf(fpt,"%Lf\t%Lf\t%Lf\n",p[f[i].a].x,p[f[i].a].y,p[f[i].a].z);
       fprintf(fpt,"%Lf\t%Lf\t%Lf\n",p[f[i].b].x,p[f[i].b].y,p[f[i].b].z);
       fprintf(fpt,"%Lf\t%Lf\t%Lf\n",p[f[i].c].x,p[f[i].c].y,p[f[i].c].z);
   }

   return 0;   
}



