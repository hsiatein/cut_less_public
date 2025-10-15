#include <pattern_solution.hpp>
#include <deque>

PatternSolution::PatternSolution(Scheme scheme){
    for(auto [partnum,group,num]:scheme){
        groupNums.emplace_back(group,num);
    }

}

PatternSolution::PatternSolution(const PatternSolution& other):groupNums(other.groupNums),parts_num(other.parts_num){
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

// bool PatternSolution::operator<(const PatternSolution& other) const{
//     int a=remain_groups();
//     int b=other.remain_groups();
//     if(a!=b) return a>b;
//     return get_volume()>other.get_volume();
// }

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
        parts_num=other.parts_num;
    }
    return *this;
}

int PatternSolution::placed_pattern() const{
    int result=0;
    for(auto blueprint:blueprints){
        auto nodes=blueprint->top->traverse();
        for(auto node:nodes){
            if(node->is_pattern()) result++;
        }
    }
    return result;
}

double PatternSolution::get_util_volume() const{
    double util=0;
    for(const auto blueprint:blueprints){
        util+=blueprint->get_util_volume();
    }
    return util;
}

double PatternSolution::cal_util_rate() const{
    return get_util_volume()/get_volume();
}

void PatternSolution::check_self() const{
    for(auto* blueprint:blueprints){
        if(blueprint->top==nullptr) throw cleanAndError("PatternSolution::check_self(): blueprint top is nullptr");
        std::deque<PatternNode*> Q;
        Q.push_back(blueprint->top);
        while (!Q.empty())
        {
            PatternNode* u=Q[0];
            Q.pop_front();
            for(auto child:u->childs){
                Q.push_back(child);
            }
            if(!u->size.valid()){
                throw cleanAndError("PatternSolution::check_self(): node size is invalid");
            }
        }
    }
}