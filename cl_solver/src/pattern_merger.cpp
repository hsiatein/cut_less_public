#include <pattern_merger.hpp>

PatternMerger::PatternMerger(Problem* problem):problem(problem),mergeChecker(problem){
}

/// @brief 模式生成
StagePatterns PatternMerger::generate_patterns(){
    StagePatterns patterns(problem);
    int level=0;
    size_t pattern_stage=1;
    patterns.new_stage(pattern_stage);
    for(const auto& part:problem->parts){
        Pattern pattern(problem,part,level);
        patterns[1].push_back(std::move(pattern));
    }
    while(true){
        ++pattern_stage;
        patterns.new_stage(pattern_stage);
        ExistChecker existChecker;
        // 判断是否离开模式生成循环
        if(pattern_stage>MAX_STAGE) break;;
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
            // std::cout<<stagePair.first<<","<<stagePair.second<<std::endl;
            size_t leftStage=stagePair.first;
            size_t rightStage=stagePair.second;
            if(patterns[leftStage].size()*patterns[rightStage].size()==0) continue;
            // std::cout<<"000"<<std::endl;
            for(const auto& left:patterns[leftStage])
                for(const auto& right:patterns[rightStage]){
                    if(!mergeChecker.parts_num_fit(left.partsNum,right.partsNum)) continue;
                    logger.log(left.partsNum.to_json().dump()+"   "+right.partsNum.to_json().dump()+" can merge");
                    // auto generatedPatterns=generate_merged_pattern_with_check(mergeChecker,left,right);
                    auto generatedPatterns=generate_merged_pattern(left,right);
                    logger.log(std::to_string(generatedPatterns.size()));
                    for(auto& pattern:generatedPatterns){
                        if(existChecker.exist(pattern)) continue;
                        patterns[pattern_stage].push_back(std::move(pattern));
                    }
                }

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

std::vector<Pattern> PatternMerger::generate_merged_pattern(const Pattern& p1,const Pattern& p2){
    std::vector<Pattern> patterns;
    std::vector<OrientMatch> matches=p1.collect_match_1D(p2);
    std::vector<RotateOrientMatch> rotateOrientmatches=matches_to_rotateOrientMatches(matches);

    for(const RotateOrientMatch& match:rotateOrientmatches){
        Pattern pattern_left(p1,std::get<0>(match));
        Pattern pattern_right(p2,std::get<1>(match));
        pattern_left.resize(Orient::X,std::get<2>(match));
        pattern_right.resize(Orient::X,std::get<2>(match));
        int newSizeZ=std::max(pattern_left.top->size[Orient::Z].first,pattern_right.top->size[Orient::Z].first);
        pattern_left.resize_or_merge(Orient::Z,newSizeZ);
        pattern_right.resize_or_merge(Orient::Z,newSizeZ);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());
        // 判断是否达到利用率界限
        double newVolume=pattern_left.top->size.get_volume()+pattern_right.top->size.get_volume();
        // double newVolume=std::get<2>(match)*newSizeZ*(pattern_left.top->size[Orient::Y].first+pattern_right.top->size[Orient::Y].first);
        double partsVolume=pattern_left.get_parts_volume()+pattern_right.get_parts_volume();
        logger.log(std::to_string(partsVolume));
        logger.log(std::to_string(newVolume));
        logger.log(std::to_string(partsVolume/newVolume));
        if(partsVolume/newVolume<UTILIZATION_RATE_LIMIT) continue;
        // 达到利用率界限的模式继续生成
        Pattern cutLoss(pattern_left.problem,pattern_left.top->size[Orient::X].first,CUT_LOSS,pattern_left.top->size[Orient::Z].first,Orient::Y);
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

std::vector<Pattern> PatternMerger::generate_merged_pattern_with_check(const MergeChecker& mergeCheck,const Pattern& p1,const Pattern& p2){
    std::vector<Pattern> patterns;
    std::vector<OrientMatch> matches=p1.collect_match_1D(p2);
    std::vector<RotateOrientMatch> rotateOrientmatches=matches_to_rotateOrientMatches(matches);

    for(const RotateOrientMatch& match:rotateOrientmatches){
        Pattern pattern_left(p1,std::get<0>(match));
        Pattern pattern_right(p2,std::get<1>(match));
        pattern_left.resize(Orient::X,std::get<2>(match));
        pattern_right.resize(Orient::X,std::get<2>(match));
        int newSizeZ=std::max(pattern_left.top->size[Orient::Z].first,pattern_right.top->size[Orient::Z].first);
        pattern_left.resize_or_merge(Orient::Z,newSizeZ);
        pattern_right.resize_or_merge(Orient::Z,newSizeZ);
        logger.log(pattern_left.to_string());
        logger.log(pattern_right.to_string());
        // 判断是否达到利用率界限
        double newVolume=pattern_left.top->size.get_volume()+pattern_right.top->size.get_volume();
        double partsVolume=pattern_left.get_parts_volume()+pattern_right.get_parts_volume();
        logger.log(std::to_string(partsVolume));
        logger.log(std::to_string(newVolume));
        logger.log(std::to_string(partsVolume/newVolume));
        if(partsVolume/newVolume<UTILIZATION_RATE_LIMIT) continue;
        if(!mergeCheck.size_fit({std::get<2>(match),newSizeZ,pattern_left.top->size[Orient::Y].first+pattern_right.top->size[Orient::Y].first})) continue;
        // 达到利用率界限的模式继续生成
        Pattern cutLoss(pattern_left.problem,pattern_left.top->size[Orient::X].first,CUT_LOSS,pattern_left.top->size[Orient::Z].first,Orient::Y);
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
        RotateOrientPair pair1=regularizeRotate(std::get<0>(match));
        RotateOrientPair pair2=regularizeRotate(std::get<1>(match));
        int newSize=std::get<2>(match);
        rotateOrientmatches.emplace_back(pair1.first,pair2.first,newSize);
        rotateOrientmatches.emplace_back(pair1.first,pair2.second,newSize);
        rotateOrientmatches.emplace_back(pair1.second,pair2.first,newSize);
        rotateOrientmatches.emplace_back(pair1.second,pair2.second,newSize);
    }
    return rotateOrientmatches;
}


RotateOrientPair PatternMerger::regularizeRotate(Orient orient){
    switch (orient){
        case Orient::X:
            return RotateOrientPair(RotateOrient::I,RotateOrient::X);
        case Orient::Y:
            return RotateOrientPair(RotateOrient::Z,RotateOrient::XY);
        case Orient::Z:
            return RotateOrientPair(RotateOrient::XZ,RotateOrient::Y);
        default:
            break;
    }
    throw cleanAndError("utils::regularizeRotate : 输入方向对不正确");
}