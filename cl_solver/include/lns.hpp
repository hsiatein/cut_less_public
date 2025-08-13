#pragma once
#include <pattern_solution.hpp>
#include "process.hpp"
#include <stage_patterns.hpp>

enum class LNSStatus{
    SUCCESS,
    FAIL
};

using DeleteOption=std::tuple<Blueprint*,PatternNode*,double>;

class LNS{
public:
    LNS(Problem* problem,Scheme scheme,StagePatterns& patterns);
    ~LNS();
    void run();
    
    void recreate(Process& process);
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
    DeleteOption select_delete_option(const std::vector<DeleteOption>& options);
    void delete_node(DeleteOption delete_option);
    void close_sheets();


    PatternSolution& get_solution() const;
    PatternSolution& get_best() const;
    History& get_history();
    // Node* to_node(const PatternNode* patternNode) const;
    // json to_json(const PatternNode* patternNode);
    // json to_json(const Blueprint* blueprint);
    // json to_json(Solution* solution);

    Process* lastProcess;


private:
    int cal_cutnum(Blueprint* blueprint);
    inline Pattern& get_pattern(StageLocation location){
        return patterns[location.first][location.second];
    }
    Scheme scheme;
    Problem* problem;
    std::vector<int> sheetsNum;
    PatternSolution* solution;
    History history;
    StagePatterns& patterns;
    const static std::array<RotateOrient,6> rotates;
};