#include <lns.hpp>
#include <deque>

const std::array<RotateOrient,6> LNS::rotates={RotateOrient::I,RotateOrient::X,RotateOrient::Y,RotateOrient::Z,RotateOrient::XY,RotateOrient::XZ};


LNS::LNS(const Problem* problem,Scheme scheme,const StagePatterns& patterns,Timer timer,SolverConfig config)
:lastProcess(nullptr),scheme(scheme),problem(problem),sheetsNum(problem->sheetsNum),solution(new PatternSolution(scheme)),patterns(patterns),timer(timer),config(config),randomEngine(config.LNS_RANDOM_SEED){
    timer.print_time(Color::GREEN);
    timer.print(Color::BLUE,"\nLNS开始\n");
    timer.print(Color::PURPLE,config.lns_to_string(),"\n");
}

LNS::~LNS(){
    delete solution;
    for(auto history_solution:history){
        delete history_solution;
    }
    if(lastProcess!=nullptr) delete lastProcess;
}

bool LNS::greater(PatternSolution* a,PatternSolution* b){
    int a_remain=a->remain_groups();
    int b_remain=b->remain_groups();
    if(a_remain!=b_remain) return a_remain<b_remain;
    double a_volume=a->get_volume();
    double b_volume=b->get_volume();
    if(a_volume!=b_volume) return a_volume<b_volume;
    int a_cut=cal_cutnum(a);
    int b_cut=cal_cutnum(b);
    if(a_cut!=b_cut) return a_cut<b_cut;
    return false;
}

int LNS::cal_cutnum(Blueprint* blueprint){
    auto patternNodes=blueprint->top->traverse();
    int result=0;
    for(auto patternNode:patternNodes){
        if(patternNode->is_cutloss()) result++;
        else if(patternNode->is_pattern()){
            result=result+get_pattern(patternNode->stageLocation).partsNum.get(problem->CUTLOSS);
        }
    }
    return result;
}

int LNS::cal_cutnum(PatternSolution* solution){
    int result=0;
    for(auto blueprint:solution->blueprints){
        result+=cal_cutnum(blueprint);
    }
    return result;
}

void LNS::replace_best(){
    PatternSolution* best=new PatternSolution(*solution);
    history.push_back(best);
}

void LNS::replace_solution(){
    delete solution;
    solution=new PatternSolution(*history.back());
}

void LNS::run(){
    int n=0;
    while(!timer.is_overtime(config.TIME_LIMIT)){
        Process process;

        recreate(process);

        if(history.empty() || greater(solution,history.back())){
            replace_best();
            timer.print_time(Color::GREEN);
            timer.print(Color::BLUE,"\ntotal volume: ",history.back()->get_volume()*1e-12,"\ntotal cuts: ",cal_cutnum(history.back()),"\n");

            delete lastProcess;
            lastProcess=new Process(process);
            if(config.INFO_OPERATION){
                timer.print(Color::RED,"Process:\n");
                for(size_t i=0;i<lastProcess->operations.size();i++){
                    lastProcess->print_operation(i,timer);
                }
            }
            // for(auto solution:lastProcess->history){
            //     std::cout<<"remain num: "<<solution->remain_groups()<<std::endl;
            //     std::cout<<"placed num: "<<solution->placed_pattern()<<std::endl;
            // }
        }
        // else if(randomEngine.rand_int(0,99)<SOLUTION_GET_BEST_PROB){
        //     replace_solution();
        // }
        // delete solution;
        // solution=new Solution(scheme);
        ruin_all();
        ++n;
        
        
        // std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    }

}

void LNS::ruin(){
    int init_option_num=0;
    int n=0;
    while(true){
        std::vector<DeleteOption> options=get_deletables();
        if(init_option_num==0) init_option_num=options.size();
        if(init_option_num*(1-config.DESTROY_RATE)>=options.size()) break;
        DeleteOption option=select_delete_option(options);
        delete_node(option);
        n++;
        // if(n==4) break;
    }

    if(randomEngine.rand_double()<config.CLOSE_SHEET_PROB) close_sheets();
}

void LNS::ruin_all(){
    delete solution;
    solution=new PatternSolution(scheme);
}

void LNS::recreate(Process& process){
    // std::cout<<"sheet num: "<<solution->blueprints.size()<<std::endl;
    while(!timer.is_overtime(config.TIME_LIMIT)){
        // 选择一批模式
        auto [groupID,group]=get_next_group();
        if(group.empty()) break;
        std::vector<std::pair<StageLocation,Size>> batch_patterns=get_batch_patterns(group);

        // 为这批模式生成插入选项
        std::vector<Option> options;
        for(auto [stageLocation,size]:batch_patterns){
            std::vector<Option> tempOptions=generate_options(groupID,stageLocation,size);
            // std::cout<<tempOptions.size()<<std::endl;
            options.insert(options.end(),tempOptions.begin(),tempOptions.end());
        }

        // 当前使用的母板找不到选项时使用新母板
        std::vector<Blueprint*> blueprints;
        if(options.empty()){
            blueprints=open_sheets(batch_patterns);
            for(auto [stageLocation,size]:batch_patterns){
                std::vector<Option> tempOptions=generate_options(groupID,stageLocation,size,blueprints);
                // std::cout<<tempOptions.size()<<std::endl;
                options.insert(options.end(),tempOptions.begin(),tempOptions.end());
            }
        }
        // std::cout<<"remain num: "<<process.history.back()->remain_groups()<<std::endl;
        // std::cout<<"placed num: "<<process.history.back()->placed_pattern()<<std::endl;

        // 获得一个选项，插入
        if(options.empty()) break;
        Option bestOption=select_option(options);
        insert(bestOption);

        // 关闭没有使用的原料，把使用了的原料加入solution
        if(blueprints.size()>0){
            auto keeped=keep_nonempty_sheets(blueprints);
            solution->blueprints.insert(solution->blueprints.end(),keeped.begin(),keeped.end());
        }

        // 记录过程
        if(config.INFO_OPERATION){
            process.log_operation(bestOption);
            std::vector<Option> record;
            for(auto op:options){
                if(std::get<0>(op)!=std::get<0>(bestOption) || std::get<1>(op)!=std::get<1>(bestOption)) continue;
                record.push_back(op);
            }
            process.log_options(record);
        }
        process.log_solution(solution);
    }
    // for(auto solution:process.history){
    //     std::cout<<"placed num: "<<solution->placed_pattern()<<std::endl;
    // }

}

std::vector<std::pair<StageLocation,Size>> LNS::get_batch_patterns(const std::vector<StageLocation>& group){
    std::vector<StageLocation> stageLocations=randomEngine.rand_batch_elements<StageLocation>(group,config.PATTERN_BATCH_SIZE);
    std::vector<std::pair<StageLocation,Size>> result;
    for(auto stageLocation:stageLocations){
        result.emplace_back(stageLocation,get_pattern(stageLocation).top->size);
    }
    return result;
}

std::pair<size_t,std::vector<StageLocation>> LNS::get_next_group(){
    std::vector<std::pair<size_t,GroupNum*>> cand;
    for(size_t i=0;i<solution->groupNums.size();++i){
        GroupNum& group=solution->groupNums[i];
        if(std::get<1>(group)==0) continue;
        cand.push_back({i,&group});
    }
    if(cand.empty()) return {};
    std::pair<size_t,GroupNum*> group=randomEngine.rand_element<std::pair<size_t,GroupNum*>>(cand);
    group.second->second=group.second->second-1;
    return {group.first,group.second->first};
}


std::vector<Option> LNS::generate_options(size_t groupID,StageLocation stageLocation,Size size){
    std::vector<Option> result;
    std::map<Blueprint*,std::vector<PatternNode*>> emptyStructs;
    for(auto blueprint:solution->blueprints){
        emptyStructs[blueprint]=blueprint->get_empty_structs();
    }
    for(auto [blueprint,structs]:emptyStructs){
        for(auto patternNode:structs){
            for(auto rotate:rotates){
                Size new_size=size.rotate(rotate);
                std::vector<CutOrients> cutOrients=patternNode->match(new_size);
                for(auto cutOrient:cutOrients){
                    OptionCost cost=patternNode->evaluate(new_size,cutOrient,config);
                    // Option option(blueprint,patternNode,stageLocation,new_size,rotate,cutOrient,cost);
                    result.emplace_back(blueprint,patternNode,groupID,stageLocation,new_size,rotate,cutOrient,cost);
                }
            }
        }
    }
    return result;
}

std::vector<Option> LNS::generate_options(size_t groupID,StageLocation stageLocation,Size size,std::vector<Blueprint*>& blueprints){
    std::vector<Option> result;
    for(auto blueprint:blueprints){
        PatternNode* patternNode=blueprint->top;
        for(auto rotate:rotates){
            Size new_size=size.rotate(rotate);
            std::vector<CutOrients> cutOrients=patternNode->match(new_size);
            for(auto cutOrient:cutOrients){
                OptionCost cost=patternNode->evaluate(new_size,cutOrient,config);
                result.emplace_back(blueprint,patternNode,groupID,stageLocation,new_size,rotate,cutOrient,cost);
            }
        }
    }
    return result;
}

Option LNS::select_option(const std::vector<Option>& options){
    if(randomEngine.rand_double()<config.BLINK_PROB){
        return randomEngine.rand_element<Option>(options);
    }
    Option best=options[0];
    OptionCost best_cost=std::get<7>(best);
    for(const Option& option:options){
        OptionCost cost=std::get<7>(option);
        if(cost.first>best_cost.first || (cost.first==best_cost.first && cost.second<=best_cost.second)) continue;
        best=option;
        best_cost=cost;
    }
    return best;
}

std::vector<Blueprint*> LNS::open_sheets(const std::vector<std::pair<StageLocation,Size>>& batchPatterns){
    std::vector<Vec3i> batchSizes;
    for(auto pair:batchPatterns){
        Vec3i p_size=pair.second.size;
        std::sort(p_size.begin(),p_size.end());
        batchSizes.push_back(p_size);
    }
    std::vector<SheetType> new_sheets;
    std::vector<Blueprint*> new_blueprints;
    for(const auto& sheet:problem->sheets){
        if(sheetsNum[sheet.id]<=0) continue;
        for(auto p_size:batchSizes){
            Vec3i s_size=sheet.size.size;
            if(s_size[0]>p_size[0] && s_size[1]>p_size[1] && s_size[2]>p_size[2]){
                new_sheets.push_back(sheet);
                break;
            }
        }
        if(new_sheets.size()>=((double)config.SHEET_BATCH_SIZE)/(1-config.SHEET_DISCARD_PROB)) break;
    }
    auto sheets=randomEngine.rand_batch_elements<SheetType>(new_sheets,config.SHEET_BATCH_SIZE);
    for(auto sheet:sheets){
        new_blueprints.push_back(new Blueprint(sheet));
    }
    return new_blueprints;
}

std::vector<Blueprint*> LNS::keep_nonempty_sheets(std::vector<Blueprint*>& sheets){
    std::vector<Blueprint*> result;
    for(Blueprint* blueprint:sheets){
        if(blueprint->top->childs.empty()){
            sheetsNum[blueprint->sheetID]+=1;
            delete blueprint;
        }
        else{
            result.push_back(blueprint);
        }
    }
    return result;
}

void LNS::close_sheets(){
    std::vector<Blueprint*> result;
    for(Blueprint* blueprint:solution->blueprints){
        if(blueprint->top->childs.empty()){
            sheetsNum[blueprint->sheetID]+=1;
            delete blueprint;
        }
        else{
            result.push_back(blueprint);
        }
    }
    solution->blueprints=result;
}

void LNS::insert(Option option){
    auto [blueprint,patternNode,groupID,stageLocation,size,rotate,cutOrients,optionCost]=option;
    blueprint->emptyStructs.clear();
    patternNode->insert(groupID,stageLocation,size,rotate,cutOrients,config);
}


void LNS::delete_node(DeleteOption delete_option){
    auto [blueprint,patternNode,cost]=delete_option;
    // for(int i=0;i<solution->blueprints.size();i++){
    //     if(blueprint==solution->blueprints[i]) std::cout<<"Blueprint:"<<i<<"\n";
    // }
    blueprint->emptyStructs.clear();
    auto childs=patternNode->traverse();
    for(const auto child:childs){
        if(child->is_pattern()) solution->groupNums[child->groupID].second++;
    }
    auto parent=patternNode->parent;
    if(parent!=nullptr){
        parent->delete_child(patternNode);
        parent->reform();
    }
    else{
        while (!patternNode->childs.empty())
        {
            PatternNode* child=patternNode->childs.back();
            patternNode->delete_child(child);
        }
    }
}

std::vector<DeleteOption> LNS::get_deletables(){
    std::vector<DeleteOption> result;
    std::deque<PatternNode*> Q;
    for(auto blueprint:solution->blueprints){
        Q.push_back(blueprint->top);
        while (!Q.empty())
        {
            PatternNode* u=Q[0];
            Q.pop_front();
            if(u->is_cutloss()) continue;
            for(auto child:u->childs){
                Q.push_back(child);
            }
            if((u->parent==nullptr && !(u->childs.empty())) || (u->parent!=nullptr && u->has_sibling())){
                result.push_back({blueprint,u,u->cal_utilization_rate()});
            }
        }
        
    }
    return result;
}

std::vector<DeleteOption> LNS::get_deletable_blueprints(){
    std::vector<DeleteOption> result;
    for(auto blueprint:solution->blueprints){
        result.push_back({blueprint,blueprint->top,blueprint->top->cal_utilization_rate()});
    }
    return result;
}

DeleteOption LNS::select_delete_option(const std::vector<DeleteOption>& options){
    DeleteOption best=options[0];
    for(auto option:options){
        if(std::get<2>(option)>std::get<2>(best)){
            best=option;
        }
    }
    return best;
}


// Node* LNS::to_node(const PatternNode* patternNode) const{
//     if(patternNode->is_cutloss()){
//         return new Node(problem,patternNode->size.size[0],patternNode->size.size[1],patternNode->size.size[2],patternNode->next_cut_orient);
//     }
//     else if (patternNode->is_struct())
//     {
//         Node* self=new Node(problem,patternNode->size,problem->STRUCT,patternNode->next_cut_orient);
//         for(auto child:patternNode->childs){
//             Node* child_node=to_node(child);
//             self->addChild(child_node);
//         }
//         return self;
//     }
//     else{
//         Pattern pattern(patterns[patternNode->stageLocation.first][patternNode->stageLocation.second],patternNode->rotate);
//         pattern.resize_force(Orient::X,patternNode->size.size[0]);
//         pattern.resize_force(Orient::Y,patternNode->size.size[1]);
//         pattern.resize_force(Orient::Z,patternNode->size.size[2]);
//         Node* top=pattern.top;
//         pattern.top=nullptr;
//         return top;
//     }
    
// }

// json LNS::to_json(const PatternNode* patternNode){
//     Node* node=to_node(patternNode);
//     json result=node->to_json();
//     delete node;
//     return result;
// }

// json LNS::to_json(const Blueprint* blueprint){
//     json result;
//     result["Tree"]=to_json(blueprint->top);
//     result["SheetID"]=blueprint->sheetID;
//     return result;
// }

// json LNS::to_json(Solution* solution){
//     json result=json::array();
//     for(const Blueprint* blueprint:solution->blueprints){
//         result.push_back(to_json(blueprint));
//     }
//     for(const GroupNum& gn:solution->groupNums){
//         json j;
//         j["Group"]=gn.first;
//         j["Num"]=gn.second;
//         result.push_back(j);
//     }
//     return result;
// }