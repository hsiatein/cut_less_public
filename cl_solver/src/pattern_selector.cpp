#include <pattern_selector.hpp>

PatternSelector::PatternSelector(const Problem& problem,Timer timer):problem(problem),patterns_size(0),timer(timer){
    model.lp_.sense_ = ObjSense::kMinimize;
    model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;
    highs.setOptionValue("time_limit",timer.get_remain_time()/2000);
    highs.setOptionValue("output_flag",INFO_HIGHS_INFO);
}
void PatternSelector::partsnum_register(const StagePatterns& patterns){
    partsnum_register(patterns,patterns.patterns.size()+1);
}

void PatternSelector::partsnum_register(const StagePatterns& patterns,size_t max_stage){
    for(const auto& [stage,stagePatterns]:patterns.patterns){
        if(stage>max_stage) break;
        size_t N=stagePatterns.size();
        for(size_t i=0;i<N;++i){
            if(partsnum_of_patterns.find(stagePatterns[i].partsNum)==partsnum_of_patterns.end()){
                partsnum_of_patterns[stagePatterns[i].partsNum]={};
                partsnums.push_back(stagePatterns[i].partsNum);
            }
            partsnum_of_patterns[stagePatterns[i].partsNum].emplace_back(stage,i);
            ++patterns_size;
        }
    }
    size_t partsnums_size=partsnums.size();
    size_t parts_size=problem.STRUCT;
    model.lp_.num_col_ = partsnums_size;
    model.lp_.num_row_ = parts_size;
    std::vector<double> col_cost_(partsnums_size);
    std::vector<double> col_lower_(partsnums_size,0);
    std::vector<double> col_upper_(partsnums_size,(double)(*std::max_element(problem.partsNum.begin(),problem.partsNum.end()))+0.5);
    std::vector<double> row_lower_(parts_size);
    std::vector<double> row_upper_(parts_size);
    model.lp_.integrality_.resize(partsnums_size);
    for(size_t col=0;col<partsnums_size;++col){
        col_cost_[col]=(double)(partsnums[col].get(problem.CUTLOSS))+AVERAGE_CUT_PUNISH;
        model.lp_.integrality_[col] = HighsVarType::kInteger;

    }
    for(size_t row=0;row<parts_size;++row){
        row_lower_[row]=(double)(problem.partsNum[row])-0.5;
        row_upper_[row]=(double)(problem.partsNum[row])+0.5;
    }
    model.lp_.col_cost_ = col_cost_;
    model.lp_.col_lower_ = col_lower_;
    model.lp_.col_upper_ = col_upper_;
    model.lp_.row_lower_ = row_lower_;
    model.lp_.row_upper_ = row_upper_;

    auto [start_,index_,value_]=generate_highs_mat();
    model.lp_.a_matrix_.start_ = start_;
    model.lp_.a_matrix_.index_ = index_;
    model.lp_.a_matrix_.value_ = value_;

    highs.passModel(model);
}

std::tuple<std::vector<int>,std::vector<int>,std::vector<double>> PatternSelector::generate_highs_mat(){
    std::vector<int> start_={0};
    std::vector<int> index_;
    std::vector<double> value_;
    size_t partsnums_size=partsnums.size();
    size_t parts_size=problem.STRUCT;
    int start=0;
    for(int i=0;i<partsnums_size;++i){
        for(int j=0;j<parts_size;++j){
            if(partsnums[i].get(j)==0) continue;
            index_.push_back(j);
            ++start;
            value_.push_back((double)(partsnums[i].get(j)));
        }
        start_.push_back(start);
    }
    
    return std::tuple<std::vector<int>,std::vector<int>,std::vector<double>>(start_,index_,value_);
}

size_t PatternSelector::get_patterns_size() const{
    return patterns_size;
}

json PatternSelector::to_json() const{
    json result=json::array();
    for(auto& [partsnum,location]:partsnum_of_patterns){
        json j=partsnum.to_json();
        j["Location"]=location;
        result.push_back(j);
    }
    return result;
}

Scheme PatternSelector::select(){
    HighsStatus return_status;
    Scheme scheme;
    const HighsLp& lp = highs.getLp();
    return_status = highs.run();
    // assert(return_status == HighsStatus::kOk);
    const HighsSolution& solution = highs.getSolution();
    std::vector<int> result;

    timer.print_time(Color::GREEN);
    timer.print(Color::BLUE,"\n模式选择完成\n");
    for (int col = 0; col < lp.num_col_; col++) {
        result.push_back((int)(solution.col_value[col]+0.01));
        if(result[col]==0) continue;
        if(INFO_SELECT_RESULT){
            timer.print(Color::BLUE,"Pattern Group ",col,": ",partsnums[col].to_json()["PartsNum"].dump(),"; num = ",result[col],"\n");
            // std::cout << "Pattern Group" <<col<<": "<< partsnums[col].to_json()["PartsNum"].dump();
            // std::cout << "; value = " << result[col];
            // std::cout << std::endl;
        }
        scheme.emplace_back(partsnums[col],partsnum_of_patterns[partsnums[col]],result[col]);
    }
    return scheme;

}

json PatternSelector::scheme_to_json(const Scheme& scheme) const{
    json result=json::array();
    for(const auto& [partsnum,locations,num]:scheme){
        json j;
        j.merge_patch(partsnum.to_json());
        j["Locations"]=locations;
        j["Num"]=num;
        result.push_back(j);
    }
    return result;
}