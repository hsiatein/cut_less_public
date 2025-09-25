#pragma once
#include <pattern_solution.hpp>
#include <process.hpp>
#include <stage_patterns.hpp>

enum class LNSStatus{
    SUCCESS,
    FAIL,
    OUT_OF_TARGET
};

using DeleteOption=std::tuple<Blueprint*,PatternNode*,double>;

class LNS{
public:
    LNS(const Problem* problem,Scheme scheme,const StagePatterns& patterns,Timer timer,SolverConfig config);
    ~LNS();
    void run();
    
    LNSStatus recreate(Process& process);
    std::vector<std::pair<StageLocation,Size>> get_batch_patterns(const std::vector<StageLocation>& group);
    std::pair<size_t,std::vector<StageLocation>> get_next_group();
    std::vector<Option> generate_options(size_t groupID,StageLocation stageLocation,Size size);
    std::vector<Option> generate_options(size_t groupID,StageLocation stageLocation,Size size,std::vector<Blueprint*>& blueprints);
    Option select_option(const std::vector<Option>& options);
    std::vector<Blueprint*> open_sheets(const std::vector<std::pair<StageLocation,Size>>& batchPatterns);
    std::vector<Blueprint*> keep_nonempty_sheets(std::vector<Blueprint*>& sheets);
    void insert(Option option);

    bool greater(PatternSolution* a,PatternSolution* b);
    int cal_cutnum(PatternSolution* solution);
    void replace_best();
    void replace_solution();

    void ruin();
    void ruin_all();
    std::vector<DeleteOption> get_deletables();
    std::vector<DeleteOption> get_deletable_blueprints();
    DeleteOption select_delete_option(const std::vector<DeleteOption>& options);
    void delete_node(DeleteOption delete_option);
    void close_sheets();


    inline PatternSolution& get_solution() const{
        return *solution;
    }
    
    inline PatternSolution& get_best() const{
        return *history.back();
    }

    inline History& get_history(){
        return history;
    }
    // Node* to_node(const PatternNode* patternNode) const;
    // json to_json(const PatternNode* patternNode);
    // json to_json(const Blueprint* blueprint);
    // json to_json(Solution* solution);

    Process* lastProcess;


private:
    int cal_cutnum(Blueprint* blueprint);
    inline const Pattern& get_pattern(StageLocation location){
        return patterns[location.first][location.second];
    }
    Scheme scheme;
    const Problem* problem;
    std::vector<int> sheetsNum;
    PatternSolution* solution;
    History history;
    const StagePatterns& patterns;
    const static std::array<RotateOrient,6> rotates;
    Timer timer;
    SolverConfig config;
    Random randomEngine;
    double min_volume=1e100;
    int max_patterns=0;
};