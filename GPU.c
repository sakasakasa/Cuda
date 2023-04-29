#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <sys/time.h>

#define XSIZE 4000
#define YSIZE 4000
#define r 0.1

__global__
void scatter(float* u, int n){
  int b = blockIdx.x;
  int t = threadIdx.x;
  int idx = b * blockDim.x+t+n*XSIZE*YSIZE;
  int to_idx = (idx + XSIZE * YSIZE)%(2*XSIZE*YSIZE);


while(idx < n* XSIZE*YSIZE+XSIZE*YSIZE){
/*if(b!=0 && b!= XSIZE-1 && t != 0 && t!=YSIZE-1)*/
  if((idx-n*XSIZE*YSIZE) % XSIZE != 0
  && (idx-n*XSIZE*YSIZE) > XSIZE
  && (idx-n*XSIZE*YSIZE+1) % XSIZE !=0
  && XSIZE*YSIZE-1-(idx-n*XSIZE*YSIZE) > XSIZE){
  float u_temp = u[idx];
  float u1 = u[idx+1];
  float u2 = u[idx-1];
  float u3 = u[idx+XSIZE];
  float u4 = u[idx-XSIZE];

  __syncthreads();

  u[to_idx] = (1.0-4.0*r)*u_temp+r*(u1+u2+u3+u4);
  idx += blockDim.x*gridDim.x;
  to_idx += blockDim.x * gridDim.x;
   }
   else {
   idx += blockDim.x*gridDim.x;
   to_idx += blockDim.x * gridDim.x;}
  }
}

struct timeval t0,t1;
int main(){
  //float u[XSIZE*YSIZE];
  //float v[XSIZE*YSIZE];

  float* u = (float*)malloc(sizeof(float)*2*XSIZE*YSIZE);

  //initialization
   for(int i = 0; i < XSIZE; i++){
    for(int j = 0; j < YSIZE; j++){
    if(i == 0||i == XSIZE-1|| j == 0|| j == YSIZE-1){
    u[i*XSIZE+j] = 0;
    u[i*XSIZE+j+XSIZE*YSIZE] = 0;
    }
    else{
    u[i*XSIZE+j] = 1;
    u[i*XSIZE+j+XSIZE*YSIZE] = 1;
    }
   }
  }

 int n = 0;
 float* ud;
 cudaMalloc((void**)&ud,2*XSIZE*YSIZE*sizeof(float));
 cudaMemcpy(ud, u, 2*XSIZE*YSIZE*sizeof(float),cudaMemcpyHostToDevice);
 gettimeofday(&t0,NULL);
 for(n = 1; n < 100; n++){
 int m = n % 2;
  if(m == 1){
   scatter<<<128,1024>>>(ud,0);}
  else{
   scatter<<<128,1024>>> (ud,1);}

  cudaThreadSynchronize();
 }
 gettimeofday(&t1,NULL);
 cudaMemcpy(u,ud,2*XSIZE*YSIZE*sizeof(float),cudaMemcpyDeviceToHost);
 printf("Elapsed time = %lf\n",
         (double)(t1.tv_sec-t0.tv_sec)+(double)(t1.tv_usec-t0.tv_usec)*1.0e-6);

 cudaFree(ud);

  /*for(int i = 0; i < XSIZE; i++){
  for(int j = 0; j < YSIZE; j++){
  printf("%f ", u[i*XSIZE+j+XSIZE*YSIZE]);
 if(j == YSIZE-1){
  printf("\n");
  }
  }
  }*/
return 0;
}