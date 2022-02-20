#include <vector>
#include <cmath>
#include <string>
#include <pthread.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

typedef vector<float> vf;
typedef pair<float,float> pf;
typedef vector<pf> vpf;
typedef vector<vf> vvf;



//pthread implementation
struct matrix_mult_struct
{
    int start_row;
    int end_row;
    int start_col;
    int end_col;
    vvf input;
    vvf weight;
    vvf* product;
    
};

//normal matrix multiplication operation which we will split into parallel threads
void* matrix_mult_2(void* arg)
{
    try{
        struct matrix_mult_struct *obj = (struct matrix_mult_struct *)arg;
        int b = (obj->weight).size();
        int c = (obj->weight)[0].size();
        
        if(b!=(obj->input)[0].size()) throw "Number of columns of input matrix not equal to number of rows of weight matrix";
        
        
        
        for(int i=(obj->start_row); i<(obj->end_row); i++)
        {
            for(int j=(obj->start_col); j<(obj->end_col); j++)
            {
                float sum=0;
                for(int k=0; k<b; k++)
                    sum += (obj->input)[i][k] * (obj->weight)[k][j];
                (*(obj->product))[i][j]=sum;
            }
        }
        pthread_exit(0);
        
    }
    catch(string msg){
        throw move(msg);
    }
}


//threading
vvf mult_pthread(vvf input, vvf weight)
{
    
    int row = input.size();
    int col = weight[0].size();
    vvf product(row,vf(col,0));

    int max_thread=4;
    int thread_row = row/(max_thread/2);
    int thread_col = col/(max_thread - max_thread/2);
    struct matrix_mult_struct split_thread[max_thread];
    

    pthread_t tids[max_thread];
    
    int count = 0;
    for(int i=0; i<max_thread/2; i++){
        for(int j=0; j<max_thread - max_thread/2; j++){
            split_thread[count++] = {i*thread_row, min(row,(i+1)*thread_row), j*thread_col, min(col,(j+1)*thread_col), input, weight, &product};
            
        }
    }
    
    for(int i=0; i<max_thread; i++)
        pthread_create(&tids[i], NULL, matrix_mult_2, &split_thread[i]);

    for(int i=0; i<max_thread; i++)
        pthread_join(tids[i], NULL);

    return product;
}
