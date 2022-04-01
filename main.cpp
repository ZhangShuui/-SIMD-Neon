#include <iostream>
#include <random>
#include <arm_neon.h>
#include <sys/time.h>
using namespace std;
const int maxN = 6000;
float Data[maxN][maxN];
struct timeval start,ending;
int n = 0;
void init(){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            Data[i][j]=float (i+1) + float (j+1)*1.3;
        }
    }
}
void GaussElimi(){
    float32x4_t vt,va,vaji,vaik,vajk,vx;
    for (int i = 0; i < n - 1; ++i) {
        vt = vld1q_dup_f32(Data[i]+i);
        for (int j = i+1; j+4 <= n-1; j+=4) {
            va = vld1q_f32(Data[i]+j);
            va = vdivq_f32(va,vt);
            vst1q_f32(Data[i]+j,va);
        }
        for (int j = n-n%4; j < n; ++j) {
            Data[i][j]/=Data[i][i];
        }
        Data[i][i] = 1.0;
        for (int j = i+1; j <= n-1; ++j) {
            vaji = vld1q_dup_f32(Data[j]+i);
            for (int k = i+1; k+4 <= n-1 ; k+=4) {
                vaik = vld1q_f32(Data[i]+k);
                vajk = vld1q_f32(Data[j]+k);
                vx = vmulq_f32(vaik,vaji);
                vajk = vsubq_f32(vajk,vx);
                vst1q_f32(Data[j]+k,vajk);
            }
            for (int k = n-n%4; k < n; ++k) {
                Data[j][k] = Data[j][k] - Data[i][k]*Data[j][i];
            }
            Data[j][i] = 0.0;
        }
    }
}//不对齐
void display(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout<<Data[i][j]<<" ";
        }
        cout<<endl;
    }
}
int main(){
    init();
    int step=10;
    int count=0;
    for (int i = 10; i <= 1000; i+=step) {
        n=i;
        gettimeofday(&start,NULL);
        gettimeofday(&ending,NULL);
        count=0;
        while(float (ending.tv_sec-start.tv_sec)<5){
            GaussElimi();
            gettimeofday(&ending,NULL);
            count++;
        }
        float x=float(ending.tv_sec-start.tv_sec);
        cout<<i<<","<<count<<','<<float(ending.tv_sec-start.tv_sec)<<","<<x/float(count)<<endl;
        if (n==100)
            step=100;
    }
    return 0;

}

