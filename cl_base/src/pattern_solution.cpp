#include <pattern_solution.hpp>

PatternSolution::PatternSolution(Scheme scheme){
    for(auto [partnum,group,num]:scheme){
        groupNums.emplace_back(group,num);
    }

}

PatternSolution::PatternSolution(const PatternSolution& other):groupNums(other.groupNums){
    for(const Blueprint* blueprint:other.blueprints){
        blueprints.push_back(new Blueprint(*blueprint));
    }
}

PatternSolution::~PatternSolution(){
    // std::cout<<"delete solution"<<std::endl;
    for(auto blueprint:blueprints){
        delete blueprint;
    }
}

bool PatternSolution::operator<(const PatternSolution& other) const{
    int a=remain_groups();
    int b=other.remain_groups();
    if(a!=b) return a>b;
    return get_volume()>other.get_volume();
}

int PatternSolution::remain_groups() const{
    int result=0;
    for(const auto& groupNum:groupNums){
        result=result+groupNum.second;
    }
    return result;
}

double PatternSolution::get_volume() const{
    double result=0;
    for(const auto& blueprint:blueprints){
        result+=blueprint->get_volume();
    }
    return result;
}

PatternSolution& PatternSolution::operator=(const PatternSolution& other){
    if (this != &other) { // 防止自我赋值
        for(auto blueprint:blueprints){
            delete blueprint;
        }
        blueprints.clear();
        for(auto blueprint:other.blueprints){
            blueprints.push_back(new Blueprint(*blueprint));
        }
        groupNums=other.groupNums;
    }
    return *this;
}

int PatternSolution::placed_pattern(){
    int result=0;
    for(auto blueprint:blueprints){
        auto nodes=blueprint->top->traverse();
        for(auto node:nodes){
            if(node->is_pattern()) result++;
        }
    }
    return result;
}