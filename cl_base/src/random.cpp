#include <random.hpp>
#include <config.hpp>
#include <random>

Random::Random(int seed):gen(seed),real_dist(0,1){

}

std::vector<int> Random::rand_range(int a,int b){
    std::vector<int> shuffledVec(b-a+1);
    std::iota(shuffledVec.begin(),shuffledVec.end(),a);
    std::shuffle(shuffledVec.begin(),shuffledVec.end(),gen);
    return shuffledVec;
}

void Random::set_seed(int seed){
    gen=std::mt19937(seed);
}