#include"cnpy.h"
#include<complex>
#include<cstdlib>
#include<iostream>
#include<map>
#include<string>

const int N = 1<<18;
//const int N = 1<<20;

int main()
{
    //set random seed so that result is reproducible (for testing)
    srand(0);
    //create random data
    std::vector<std::complex<double>> data(N);
    for(int i = 0;i < N;i++) data[i] = std::complex<double>(rand(),rand());

    //save it to file
    //now write to an npz file
    cnpy::npz_save("out.npz","arr1",&data[0],{N},"w");
}
