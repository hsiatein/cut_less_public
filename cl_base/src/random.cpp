#include <random.hpp>
#include <config.hpp>

Random::Random():gen(RANDOM_SEED){

}

int Random::rand_int(int a,int b){
    std::uniform_int_distribution<> dist(a, b);
    return dist(gen);
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