#include <process.hpp>

Process::Process(){

}

Process::Process(const Process& other):operations(other.operations),all_options(other.all_options){
    for(auto solution:other.history){
        history.push_back(new PatternSolution(*solution));
    }
}


Process::~Process(){
    for(auto solution:history){
        delete solution;
    }
}

void Process::log_solution(PatternSolution* solution){
    history.push_back(new PatternSolution(*solution));
}

void Process::log_operation(Option option){
    operations.emplace_back(std::get<0>(option)->sheetID,std::get<1>(option)->size,std::get<2>(option),std::get<4>(option),std::get<6>(option),std::get<7>(option));
}

void Process::log_options(std::vector<Option> options){
    all_options.push_back({});
    for(auto& option:options){
        all_options.back().emplace_back(std::get<0>(option)->sheetID,std::get<1>(option)->size,std::get<2>(option),std::get<4>(option),std::get<6>(option),std::get<7>(option));
    }
}


void Process::print_operation(size_t i,const Timer& timer) const{
    timer.print(Color::PURPLE,"Operation: ",i,"\n");
    timer.print(Color::BLUE,"Selected:\n");
    timer.print(Color::CYAN,to_string(operations[i]),"\n");
    timer.print(Color::BLUE,"Candidates:\n");
    for(const auto& cand:all_options[i]){
        timer.print(Color::CYAN,to_string(cand),"\n");
    }
}