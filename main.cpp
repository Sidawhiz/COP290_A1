#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <pthread.h>
#include "umkl.h"
#include "ublas.h"
#include <chrono>
#include <numeric>
using namespace std;

//========================TypeDefs===================================

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

typedef vector<float> vf;
typedef pair<float,float> pf;
typedef vector<pf> vpf;
typedef vector<vf> vvf;

//=========================MACROS====================================

#define pb push_back
#define fast ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define mod 1000000007

//==============================FUNCTIONS============================

int error = 0;

vf read_vector(string s){
    try{
        //t.clear();
        vf temp;
        ifstream file (s);
        if(!file.is_open()){
            throw (s);
        }
        string line;
        float num;
        file >> line;
        int len = stoi(line);
        for(int i=0 ; i<len ; i++){
            file >> line;
            num = stof(line);
            temp.pb(num);
        }
        file.close();
        return temp;
    }
    catch (std::invalid_argument)
    {
        cout << "Data in your file is not of type float" << "\n";
        error = 1;
        exit(0);
    }
    catch(string j)
    {
        cout << "Error in opening file : " << j << "\n";
        error = 1;
        exit(0);
    }
}

vvf multiply(vvf m1, vvf m2){
    if(m1[0].size()==m2.size()){
        vvf ans(m1.size(),vf(m2[0].size(),0));
        for(int i = 0 ; i<m1.size(); i++){
            for(int j = 0; j< m2[0].size() ; j++){
                for(int k = 0; k<m2.size(); k++){
                    ans[i][j] += m1[i][k]*m2[k][j];
                }
            }
        }
        return ans;
    }
    else{
        cout << "Dimensions don't match\n";
        error = 1;
        exit(0);
    }
}

vvf add(vvf m1, vvf m2){
    if(m1.size()==m2.size() && m1[0].size()==m2[0].size()){
        vvf ans(m1.size(),vf(m1[0].size(),0));
        for(int i = 0 ; i<m1.size(); i++){
            for(int j = 0; j< m1[0].size() ; j++){
                ans[i][j] = m1[i][j] + m2[i][j];
            }
        }
        return ans;
    }
    else{
        cout << "Dimensions don't match\n";
        error = 1;
        exit(0);
    }
}

vvf fullyconnected(vvf m1, vvf m2, vvf m3){
    vvf ans;
    ans = multiply(m1,m2);
    ans = add(ans,m3);
    return ans;
}

vvf read_matrix(string s){
    try{
        //tt.clear();
        ifstream file (s);
        if(!file.is_open()){
            throw (s);
        }
        //cout << "hi" ;
        string line;
        int cols,rows;
        file >> line;
        cols = stoi(line);
        file >> line;
        rows = stoi(line);
        //cout << cols << " " << rows ;
        vvf temp(rows,vf(cols,0.0));
        int count = 0;
        float num;
        int i,j;
        while(file >> line){  // 0,0   1,0   0,1   1,1    0,2    1,2
            num = stof(line);
            //cout << num << " ";
            i = count/rows; // 3 columns
            j = count%rows; // 2 rows
            temp[j][i] = num;
           count++;
        }
        //cout << " " << count;
        if(count!=cols*rows){
            throw(s);
        }
        file.close();
        return temp;
    }
    catch (std::invalid_argument)
    {
        cout << "Data in your file is not of type float" << "\n";
        error = 1;
        exit(0);
    }
    catch(string j)
    {
        cout << "Error in opening file : " << j << "\n";
        error = 1;
        exit(0);
    }
}
// Hello there
void write(string s, vvf tem){
    int rows = tem.size(), cols = tem[0].size();
    ofstream file (s);
    file << cols << endl;
    file << rows << endl;
    for(int i = 0 ; i<cols; i++){
        for(int j = 0; j<rows ; j++){
            file << tem[j][i] << endl;
        }
    }
}

vvf relu(vvf tem){
    int rows = tem.size(), cols = tem[0].size();
    for(int i = 0 ; i<cols; i++){
        for(int j = 0; j<rows ; j++){
            if(tem[j][i]<0.0){
                tem[j][i]=0;
            }
        }
    }
    return tem;
}

vvf tanh(vvf tem){
    int rows = tem.size(), cols = tem[0].size();
    for(int i = 0 ; i<cols; i++){
        for(int j = 0; j<rows ; j++){
            float z = tem[j][i];
            tem[j][i] = (float) ((exp(z)-exp(z*(-1)))/(exp(z)+exp(z*(-1))));
        }
    }
    return tem;
}

vvf maxpooling(vvf tem, int stride){
    int row = tem.size();
    int r = row;
    row = row/stride;
    vvf temp(row,vf(row,1.0));
    for(int i = 0 ; i<r; i+=stride){
        for(int j = 0; j<r ; j+=stride){
            float mx = tem[i][j];
            for(int ii = 0 ; ii< stride ; ii++){
                for(int jj = 0; jj<stride ; jj++){
                    //mx = max(tem[i+ii][j+jj],mx);
                    if(tem[i+ii][j+jj]>mx){
                        mx = tem[i+ii][j+jj];
                    }
                }
            }
            //cout << mx << endl;
            temp[i/stride][j/stride] = mx;
        }
    }
    //cout << endl;
    return temp;
}

vvf avgpooling(vvf tem, int stride){
    int row = tem.size();
    int r = row;
    row = row/stride;
    vvf temp(row,vf(row,1.0));
    for(int i = 0 ; i<r; i+=stride){
        for(int j = 0; j<r; j+=stride){
            float mx = 0.0;
            for(int ii = 0 ; ii< stride ; ii++){
                for(int jj = 0; jj<stride ; jj++){
                    mx += tem[i+ii][j+jj];
                }
            }
            mx/=stride;
            mx/=stride;
            temp[i/stride][j/stride] = mx;
        }
    }
    return temp;
}

vf softmax(vf tem){
    int rows = tem.size();
    vf exps(rows);
    float sum = 0.0;
    for(int j = 0; j<rows ; j++){
        float z = tem[j];
        float ans = (float) exp(z);
        exps[j] = ans;
        sum += ans;
    }
    for(int j = 0; j<rows ; j++){
        tem[j] = (float) (exps[j]/sum);
    }
    return tem;
}

vf sigmoid(vf tem){
    int rows = tem.size();
    for(int j = 0; j<rows ; j++){
        float z = tem[j];
        tem[j] = (float) (1/(1+exp(z*(-1))));
    }
    return tem;
}

void write_vector(string s, vf tem){
    int rows = tem.size();
    ofstream file (s);
    file << rows << endl;
    for(int j = 0; j<rows ; j++){
        file << tem[j]<< endl;
    }
}

vvf fully_mkl(vvf m1, vvf m2, vvf m3){
    vvf ans;
    ans = mult_mkl(m1,m2);
    ans = add(ans,m3);
    return ans;
}

vvf fully_blas(vvf m1, vvf m2, vvf m3){
    vvf ans;
    ans = mult_blas(m1,m2);
    ans = add(ans,m3);
    return ans;
}

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
            split_thread[count++] = {i*thread_row, max(row,(i+1)*thread_row), j*thread_col, max(col,(j+1)*thread_col), input, weight, &product};
            
        }
    }
    
    for(int i=0; i<max_thread; i++)
        pthread_create(&tids[i], NULL, matrix_mult_2, &split_thread[i]);

    for(int i=0; i<max_thread; i++)
        pthread_join(tids[i], NULL);

    return product;
}

vvf fully_pthread(vvf m1, vvf m2, vvf m3){
    vvf ans;
    ans = mult_pthread(m1,m2);
    ans = add(ans,m3);
    return ans;
}


float timedwork_mkl(vvf m1, vvf m2, vvf m3, string r)
{
   // starttimer();
   auto begin = std::chrono::high_resolution_clock::now();
   vvf f = fully_mkl(m1, m2, m3);
   write(r,f);
   auto end = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
   // return(1e-6 * elapsed.count());
   return(1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
   // return mssince();
}

float timedwork_pthread(vvf m1, vvf m2, vvf m3, string r)
{
   // starttimer();
   auto begin = std::chrono::high_resolution_clock::now();
   vvf f = fully_pthread(m1, m2, m3);
   write(r,f);
   auto end = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
   // return(1e-6 * elapsed.count());
   return(1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
   // return mssince();
}


float timedwork_blas(vvf m1, vvf m2, vvf m3, string r)
{
   // starttimer();
   auto begin = std::chrono::high_resolution_clock::now();
   vvf f = fully_blas(m1, m2, m3);
   write(r,f);
   auto end = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
   // return(1e-6 * elapsed.count());
   return(1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
   // return mssince();
}


float timedwork_ori(vvf m1, vvf m2, vvf m3, string r)
{
   // starttimer();
   auto begin = std::chrono::high_resolution_clock::now();
   vvf f = fullyconnected(m1, m2, m3);
   write(r,f);
   auto end = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
   // return(1e-6 * elapsed.count());
   return(1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
   // return mssince();
}

float mean_calc(vf t){
    float mean = (float)(accumulate(t.begin(),t.end(),0.0)/t.size());
    return mean;
}

float stdev_calc(vf t){
    float stdev ;
    int len = t.size();
    float mean = mean_calc(t);
    double temp = 0.0;
    for(int i=0; i<len; i++){
        temp += pow(t[i]-mean,2);
    }
    temp=temp/len;
    stdev = (float)sqrt(temp);
    return stdev;
    
}


int main(int argc, char *argv[])
{   
    //cout << t[0] <<endl;
    try{
        if(argc==7){
            //cout << "Hi " << endl;
            if(strcmp(argv[1],"fullyconnected")==0){
                if(strcmp(argv[2],"mkl")==0){
                    vf gh;
                    for(int i = 0; i<30; i++){
                        float time = timedwork_mkl(read_matrix(argv[3]),read_matrix(argv[4]),read_matrix(argv[5]),argv[6]);
                        gh.pb(time);
                    }    
                    float me = mean_calc(gh);
                    float dev = stdev_calc(gh);
                    std::cout<<"Mean is " << me << " ms"<<std::endl;
                    std::cout<<"Standard deviation is " << dev << " ms"<<std::endl;
                }
                else if(strcmp(argv[2],"openblas")==0){
                    //float time = timedwork_blas(read_matrix(argv[3]),read_matrix(argv[4]),read_matrix(argv[5]),argv[6]);
                    vf gh;
                    for(int i = 0; i<30; i++){
                        float time = timedwork_blas(read_matrix(argv[3]),read_matrix(argv[4]),read_matrix(argv[5]),argv[6]);
                        gh.pb(time);
                    }    
                    float me = mean_calc(gh);
                    float dev = stdev_calc(gh);
                    std::cout<<"Mean is " << me << " ms"<<std::endl;
                    std::cout<<"Standard deviation is " << dev << " ms"<<std::endl;
                }
                else if(strcmp(argv[2],"pthread")==0){
                    //float time = timedwork_pthread(read_matrix(argv[3]),read_matrix(argv[4]),read_matrix(argv[5]),argv[6]);
                    vf gh;
                    for(int i = 0; i<30; i++){
                        float time = timedwork_pthread(read_matrix(argv[3]),read_matrix(argv[4]),read_matrix(argv[5]),argv[6]);
                        gh.pb(time);
                    }    
                    float me = mean_calc(gh);
                    float dev = stdev_calc(gh);
                    std::cout<<"Mean is " << me << " ms"<<std::endl;
                    std::cout<<"Standard deviation is " << dev << " ms"<<std::endl;
                }
                else{
                    cout << "Invalid use of commands\n";
                    error = 1;
                    exit(0);
                }
            }
            else{
                cout << "Invalid use of commands\n";
                error = 1;
                exit(0);
            }
        } 
        else if(argc==6){
            //cout << "Hi " << endl;
            if(strcmp(argv[1],"fullyconnected")==0){
                //float time = timedwork_mkl(read_matrix(argv[2]),read_matrix(argv[3]),read_matrix(argv[4]),argv[5]);
                vf gh;
                for(int i = 0; i<30; i++){
                    float time = timedwork_ori(read_matrix(argv[2]),read_matrix(argv[3]),read_matrix(argv[4]),argv[5]);
                    gh.pb(time);
                }    
                float me = mean_calc(gh);
                float dev = stdev_calc(gh);
                std::cout<<"Mean is " << me << " ms"<<std::endl;
                std::cout<<"Standard deviation is " << dev << " ms"<<std::endl;
            }
            else if(strcmp(argv[1],"pooling")==0){
                int stride = stoi(argv[4]);
                if(strcmp(argv[2],"max")==0){
                    vvf f = maxpooling(read_matrix(argv[3]),stride);
                    write(argv[5],f);
                }
                else if(strcmp(argv[2],"average")==0){
                    vvf f = avgpooling(read_matrix(argv[3]),stride);
                    write(argv[5],f);
                }
            }
            else{
                cout << "Invalid use of commands\n";
                error = 1;
                exit(0);
            }
        } 
        else if(argc==5){
            if(strcmp(argv[1],"activation")==0){
                if(strcmp(argv[2],"relu")==0){
                    vvf f = relu(read_matrix(argv[3]));
                    write(argv[4],f);
                }
                else if(strcmp(argv[2],"tanh")==0){
                    vvf f = tanh(read_matrix(argv[3]));
                    write(argv[4],f);
                }
            }
            else if(strcmp(argv[1],"probability")==0){
                if(strcmp(argv[2],"softmax")==0){
                    vf f = softmax(read_vector(argv[3]));
                    write_vector(argv[4],f);
                }
                else if(strcmp(argv[2],"sigmoid")==0){
                    vf f = sigmoid(read_vector(argv[3]));
                    write_vector(argv[4],f);
                }
            }  
            else{
                cout << "Invalid use of commands\n";
                error = 1;
                exit(0);
                //cout << "Hi"<<endl;
            }
        }
        else{
            cout << "Invalid use of commands\n";
            error = 1;
            exit(0);
        }
    }
    catch (...){
        cout << "Error catched" << endl;
        error = 1;
        exit(0);
    }
    return 0;
}
