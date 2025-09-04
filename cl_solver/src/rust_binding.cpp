#include <solver.hpp>

extern "C"{
    struct Result{
        std::string result;
    };

    Result* solve(const char * problem_string,const char * config_string){

        std::cout<<"read problem"<<std::endl;
        SolverConfig config(json::parse(config_string));
        json problem_json=json::parse(problem_string);
        Problem problem(problem_json);

        std::cout<<"solve"<<std::endl;
        Solver solver(&problem,config);
        Solution solution=solver.solve();

        std::cout<<"return"<<std::endl;
        Result* result=new Result();
        result->result=solution.to_json().dump();
        return result;
    }

    const char* get_c_str(Result* result) {
        return result->result.c_str();
    }

    void free_result(Result* result) {
        delete result;
    }
}

