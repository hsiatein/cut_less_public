#pragma once
#include <config.hpp>
#include <fstream>

class Logger{
public:
    bool enable;
    bool enableJSON;
    bool enableTXT;
    Logger(std::string name,std::string dir="./output/");
    ~Logger();
    void log(std::string text);
    void log_json(std::string jsonName,json json);

    template<typename... Args>
    void log_json(std::string jsonName,Args... args) {
        std::string output_path = dir+name+"@"+jsonName+".json";
        std::ofstream jsonFile(output_path);
        ((jsonFile << args.dump(4) << '\n'), ...);
        jsonFile.close();

        // json merged_json = json::object();
        // (merged_json.merge_patch(args), ...);
        // log_json(jsonName, merged_json);
    }

private:
    std::string name;
    std::string dir;
    std::ofstream file;
};