#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define XSIZE 4000
#define YSIZE 4000
#define r 0.1

int main(){
  float ***u;
  float **u1;
  float *u2;
  u = (float***)malloc(2*sizeof(float**));
  u1 = (float**)malloc(2*XSIZE*sizeof(float*));
  u2 = (float*)malloc(2*XSIZE*YSIZE*sizeof(float));
  for(int i = 0; i < 2; i++){
    u[i] = u1+i*XSIZE;
  }
  for(int i = 0; i < 2* XSIZE; i++){
    u1[i] = u2+i*YSIZE;
    }

  //initialization                                                                  
  for(int i = 0; i < XSIZE; i++){
    for(int j = 0; j < YSIZE; j++){
    if(i == 0||i == XSIZE-1|| j == 0|| j == YSIZE-1){
    u[0][i][j] = 0;
    }
    else{
    u[0][i][j] = 1;
    }
   }
   }
int n = 0;
  struct timeval t0, t1;
  gettimeofday(&t0,NULL);
  for(n = 1; n < 100; n++){
  int m = n%2;
  int mm = 1-m;
  for(int i = 0; i < XSIZE; i++){
    for(int j = 0; j < YSIZE; j++){
    if(i == 0|| i == XSIZE-1 || j == 0 || j == YSIZE-1){
    u[m][i][j] = 0;
    }
    else{
      u[m][i][j] = (1-4*r)*u[mm][i][j] + r*(u[mm][i+1][j] + u[mm][i-1][j] + u[m\
m][i][j+1] + u[mm][i][j-1]);
    }
    }
  }
  }
  gettimeofday(&t1,NULL);
  printf("Elapsed time = %lf\n",
         (double)(t1.tv_sec-t0.tv_sec)+(double)(t1.tv_usec-t0.tv_usec)*1.0e-6);
  /*for(int i = 0; i < XSIZE; i++){                                             
    for(int j = 0; j < YSIZE; j++){                                             
      printf("%f ",u[1][i][j]);                                                 
      if(j == YSIZE-1){                                                         
        printf("\n");                                                           
      }                                                                         
    }                                                                           
    }*/
    free(u);
    free(u1);
    free(u2);
   return 0;
 }