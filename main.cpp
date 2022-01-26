#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

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

int main(int argc, char *argv[])
{   
    //cout << t[0] <<endl;
    try{
        if(argc==6){
            if(strcmp(argv[1],"fullyconnected")==0){
                vvf f = fullyconnected(read_matrix(argv[2]),read_matrix(argv[3]),read_matrix(argv[4]));
                write(argv[5],f);
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
        }
        else{
            cout << "Invalid use of commands\n";
            error = 1;
            exit(0);
        }
    }
    catch (const char* msg){
        cout << "Error catched" << endl;
        error = 1;
        exit(0);
    }
    return 0;
}

