#pragma once
#include <random>
#include "hyperparameters.hpp"
#include <vector>
#include <algorithm>

class cl_base_EXPORT Random{
public:
    Random();
    int rand_int(int a,int b);
    std::vector<int> rand_range(int a,int b);
    void set_seed(int seed);
    template<typename T>
    T rand_element(const std::vector<T>& vec){
        int index=rand_int(0,vec.size()-1);
        return vec[index];
    }
    template<typename T>
    std::vector<T> rand_batch_elements(const std::vector<T>& vec, int n){
        std::vector<int> randRange=rand_range(0,vec.size()-1);
        std::vector<T> result;
        int N=std::min(n,(int)(vec.size()));
        for(int i=0;i<N;++i){
            result.push_back(vec[i]);
        }
        return result;
    }


private:
    std::mt19937 gen;
};