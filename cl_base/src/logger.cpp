#include <logger.hpp>

Logger::Logger(std::string name,std::string dir):enable(true),enableJSON(true),enableTXT(true),name(name),dir(dir),file(dir+name+".txt"){
    if (!std::filesystem::exists(dir)) std::filesystem::create_directory(dir);
}

Logger::~Logger(){
    file.close();
}

void Logger::log(std::string text){
    if(!(enable && enableTXT)) return;
    file << text << std::endl;
}

void Logger::log_json(std::string jsonName,json json){
    if(!(enable && enableJSON)) return;
    // std::cout<<jsonName<<std::endl;
    std::string output_path = dir+name+"@"+jsonName+".json";
    std::ofstream jsonFile(output_path);
    jsonFile << json.dump(4);
    jsonFile.close();
}
