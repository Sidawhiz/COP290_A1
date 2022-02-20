#include "umkl.h"
namespace mkl{
    #include <mkl.h>
}  

//==============================FUNCTIONS============================

vvf mult_mkl(vvf p, vvf q)
{
    int a = p.size() , b = p[0].size() , c = q[0].size();
    // Declare array in the form of pointers and not in the form of usual way
    // because usual way uses stack but sixes here of matrix can be high
    // so we have to use heap
    float *in1 = new float[a*b];
    int count=0;
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            in1[count++]=p[i][j];
        }
    }
    count=0;
    float *in2 = new float[b*c];
    for (int i = 0; i < b; i++)
    {
        for (int j = 0; j < c; j++)
        {
            in2[count++]=q[i][j];
        }
    }
    float *out = new float[a*c];
    mkl::cblas_sgemm(mkl::CblasRowMajor, mkl::CblasNoTrans, mkl::CblasNoTrans, a, c, b, 1.0, in1, b, in2, c, 0.0, out, c);

    vvf ans(a, vf(c));
    count=0;
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < c; j++)
        {
            ans[i][j]=out[count++];
        }
    }
    return ans;
}

