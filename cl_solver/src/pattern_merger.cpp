#include "utils.hpp"
#include <pattern_merger.hpp>

PatternMerger::PatternMerger(Problem* problem,Timer timer,SolverConfig config)
:problem(problem),mergeChecker(this->problem),timer(timer),config(config){
    timer.print(Color::PURPLE,"刀缝宽度",config.CUT_LOSS,", 时间限制",config.TIME_LIMIT,"\n");
}

// PatternMerger::PatternMerger(Problem* problem,MergeChecker mergeChecker):problem(problem),mergeChecker(mergeChecker){
// }

/// @brief 模式生成
StagePatterns PatternMerger::generate_patterns(const int level) const{
    StagePatterns patterns(problem);
    size_t pattern_stage=1;
    patterns.new_stage(pattern_stage);
    for(const auto& part:problem->parts){
        Pattern pattern(problem,part,level);
        patterns[1].push_back(std::move(pattern));
    }
    double time_limit=timer.get_remain_time()/3;
    while(!timer.is_overtime(time_limit)){
        ++pattern_stage;
        patterns.new_stage(pattern_stage);
        ExistChecker existChecker;
        // 判断是否离开模式生成循环
        if(pattern_stage>config.MAX_STAGE) break;;
        int check_start=(pattern_stage+1)/2;
        bool exit=true;
        for(int i=check_start;i<pattern_stage;++i){
            if(!patterns[i].empty()){
                exit=false;
                break;
            }
        }
        if(exit) break;

        //模式生成
        std::vector<std::pair<size_t,size_t>> stagePairs=get_subpattern_pair(pattern_stage);
        
        for(auto stagePair:stagePairs){
            if(timer.is_overtime(time_limit)) break;
            // std::cout<<stagePair.first<<","<<stagePair.second<<std::endl;
            auto [leftStage,rightStage]=stagePair;
            if(patterns[leftStage].size()*patterns[rightStage].size()==0) continue;
            // std::cout<<"000"<<std::endl;
            for(const auto& left:patterns[leftStage]){
                if(timer.is_overtime(time_limit)) break;
                for(const auto& right:patterns[rightStage]){
                    if(timer.is_overtime(time_limit)) break;
                    if(!mergeChecker.parts_num_fit(left.partsNum,right.partsNum)) continue;
                    logger.log(left.partsNum.to_json().dump()+"   "+right.partsNum.to_json().dump()+" can merge");
                    std::vector<Pattern> generatedPatterns;
                    if(config.MERGE_SIZE_CHECK){
                        generatedPatterns=generate_merged_pattern_with_check(left,right);
                    }
                    else{
                        generatedPatterns=generate_merged_pattern(left,right);
                    }
                    logger.log(std::to_string(generatedPatterns.size()));
                    for(auto& pattern:generatedPatterns){
                        if(existChecker.exist(pattern)) continue;
                        patterns[pattern_stage].push_back(std::move(pattern));
                    }
                }
            }

        }

    }
    timer.print_time(Color::GREEN);
    timer.print(Color::BLUE,"\n模式生成完成\n");
    if(INFO_GENERATE_RESULT){
        for(const auto [stage,stage_patterns]:patterns.patterns){
            if(stage_patterns.size()==0) continue;
            timer.print(Color::BLUE,"Stage ",stage," 模式数量 ",stage_patterns.size()," \n");
        }
    }
    return patterns;
}

std::vector<std::pair<size_t,size_t>> PatternMerger::get_subpattern_pair(size_t i){
    std::vector<std::pair<size_t,size_t>> result;
    size_t a=i-1;
    size_t b=1;
    result.emplace_back(a,b);
    --a;
    ++b;
    while(a>=b){
        result.emplace_back(a,b);
        --a;
        ++b;
    }
    return result;
}

void PatternMerger::resize_or_merge(Pattern& pattern,Orient orient,int newSize) const{
    if(pattern.resize(orient,newSize)) return;
    int newStructSize=newSize-pattern.top->size[orient].first-std::max(pattern.top->size[orient].second,0)-config.CUT_LOSS;
    if(newStructSize<=0){
        logger.log("resize_force");
        pattern.resize_force(orient,newSize);
        return;
    }
    logger.log("resize_merge");
    // std::cout<<newStructSize<<std::endl;
    pattern.resize(orient,pattern.top->size[orient].first+pattern.top->size[orient].second);
    Size size=pattern.top->size;
    size.set(orient,{config.CUT_LOSS,0});
    Pattern cutLoss(pattern.PROBLEM_STRUCT,size.size[0],size.size[1],size.size[2],orient);
    // logger.log_json("resize_or_merge",cutLoss.top->to_json());
    // std::cout<<1<<std::endl;
    pattern.merge(cutLoss,orient);
    size.set(orient,{newStructSize,0});
    Pattern newStruct(pattern.PROBLEM_STRUCT,size,orient,pattern.level);
    newStruct.partsNum[pattern.PROBLEM_STRUCT]=1;
    pattern.merge(newStruct,orient);
}

std::vector<Pattern> PatternMerger::generate_merged_pattern(const Pattern& p1,const Pattern& p2) const{
    std::vector<Pattern> patterns;
    std::vector<OrientMatch> matches=p1.collect_match_1D(p2);
    std::vector<RotateOrientMatch> rotateOrientmatches=matches_to_rotateOrientMatches(matches);

    for(const RotateOrientMatch& match:rotateOrientmatches){
        Pattern pattern_left(p1,std::get<0>(match));
        Pattern pattern_right(p2,std::get<1>(match));
        pattern_left.resize(Orient::X,std::get<2>(match));
        pattern_right.resize(Orient::X,std::get<2>(match));
        int newSizeZ=std::max(pattern_left.top->size[Orient::Z].first,pattern_right.top->size[Orient::Z].first);
        resize_or_merge(pattern_left,Orient::Z,newSizeZ);
        resize_or_merge(pattern_right,Orient::Z,newSizeZ);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());

        // 判断是否达到利用率界限

        double newVolume=pattern_left.top->size.get_volume()+pattern_right.top->size.get_volume();
        // double newVolume=std::get<2>(match)*newSizeZ*(pattern_left.top->size[Orient::Y].first+pattern_right.top->size[Orient::Y].first);
        double partsVolume=pattern_left.get_parts_volume()+pattern_right.get_parts_volume();
        logger.log(std::to_string(partsVolume));
        logger.log(std::to_string(newVolume));
        logger.log(std::to_string(partsVolume/newVolume));
        if(partsVolume/newVolume<config.UTILIZATION_RATE_LIMIT) continue;

        // 达到利用率界限的模式继续生成

        Pattern cutLoss(pattern_left.PROBLEM_STRUCT,pattern_left.top->size[Orient::X].first,config.CUT_LOSS,pattern_left.top->size[Orient::Z].first,Orient::Y);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());
        pattern_left.merge(cutLoss,Orient::Y);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());
        pattern_left.merge(pattern_right,Orient::Y);
        patterns.push_back(std::move(pattern_left));
    }
    return patterns;
}

std::vector<Pattern> PatternMerger::generate_merged_pattern_with_check(const Pattern& p1,const Pattern& p2) const{
    std::vector<Pattern> patterns;
    std::vector<OrientMatch> matches=p1.collect_match_1D(p2);
    std::vector<RotateOrientMatch> rotateOrientmatches=matches_to_rotateOrientMatches(matches);

    for(const RotateOrientMatch& match:rotateOrientmatches){
        Pattern pattern_left(p1,std::get<0>(match));
        Pattern pattern_right(p2,std::get<1>(match));
        pattern_left.resize(Orient::X,std::get<2>(match));
        pattern_right.resize(Orient::X,std::get<2>(match));
        const int newSizeZ=std::max(pattern_left.top->size[Orient::Z].first,pattern_right.top->size[Orient::Z].first);
        resize_or_merge(pattern_left,Orient::Z,newSizeZ);
        resize_or_merge(pattern_right,Orient::Z,newSizeZ);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());

        // 判断是否达到利用率界限

        const double newVolume=pattern_left.top->size.get_volume()+pattern_right.top->size.get_volume();
        const double partsVolume=pattern_left.get_parts_volume()+pattern_right.get_parts_volume();
        logger.log(std::to_string(partsVolume));
        logger.log(std::to_string(newVolume));
        logger.log(std::to_string(partsVolume/newVolume));
        if(partsVolume/newVolume<config.UTILIZATION_RATE_LIMIT) continue;
        if(!mergeChecker.size_fit({std::get<2>(match),newSizeZ,pattern_left.top->size[Orient::Y].first+pattern_right.top->size[Orient::Y].first})) continue;
        
        // 达到利用率界限的模式继续生成

        Pattern cutLoss(pattern_left.PROBLEM_STRUCT,pattern_left.top->size[Orient::X].first,config.CUT_LOSS,pattern_left.top->size[Orient::Z].first,Orient::Y);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());
        pattern_left.merge(cutLoss,Orient::Y);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());
        pattern_left.merge(pattern_right,Orient::Y);
        patterns.push_back(std::move(pattern_left));
    }
    return patterns;
}

std::vector<RotateOrientMatch> PatternMerger::matches_to_rotateOrientMatches(const std::vector<OrientMatch>& matches){
    std::vector<RotateOrientMatch> rotateOrientmatches;
    for(const auto& match:matches){
        auto [left_rotate_method_1, left_rotate_method_2]=regularizeRotate(std::get<0>(match));
        auto [right_rotate_method_1,right_rotate_method_2]=regularizeRotate(std::get<1>(match));
        int newSize=std::get<2>(match);
        rotateOrientmatches.emplace_back(left_rotate_method_1,right_rotate_method_1,newSize);
        rotateOrientmatches.emplace_back(left_rotate_method_1,right_rotate_method_2,newSize);
        rotateOrientmatches.emplace_back(left_rotate_method_2,right_rotate_method_1,newSize);
        rotateOrientmatches.emplace_back(left_rotate_method_2,right_rotate_method_2,newSize);
    }
    return rotateOrientmatches;
}


RotateOrientPair PatternMerger::regularizeRotate(const Orient orient){
    switch (orient){
        case Orient::X:
            return {RotateOrient::I,RotateOrient::X};
        case Orient::Y:
            return {RotateOrient::Z,RotateOrient::XY};
        case Orient::Z:
            return {RotateOrient::XZ,RotateOrient::Y};
        default:
            break;
    }
    throw cleanAndError("utils::regularizeRotate : 输入方向对不正确");
}