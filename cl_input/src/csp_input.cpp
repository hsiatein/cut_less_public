#include <csp_input.hpp>

int RawPartType::NEXT_ID=0;
int RawSheetType::NEXT_ID=0;

RawPartType::RawPartType(int width,int height,int thick,bool rotatable,int qty,std::string material):
id(NEXT_ID),width(width),height(height),thick(thick),rotatable(rotatable),qty(qty),hardness(std::nullopt),material(material){
    NEXT_ID++;
}

RawPartType::RawPartType(int width,int height,int thick,bool rotatable,int qty,std::array<double,2> hardness,std::string material):
id(NEXT_ID),width(width),height(height),thick(thick),rotatable(rotatable),qty(qty),hardness(std::nullopt),material(material){
    if(hardness[0]<=hardness[1]){
        this->hardness=hardness;
    }
    else{
        throw cleanAndError("硬度输入错误，第一个值大于第二个值");
    }
    NEXT_ID++;
}

RawPartType::RawPartType(int width,int height,int thick,bool rotatable,int qty,std::optional<std::array<double,2>> hardness,std::string material):
id(NEXT_ID),width(width),height(height),thick(thick),rotatable(rotatable),qty(qty),hardness(std::nullopt),material(material){
    if(hardness!=std::nullopt){
        if(hardness.value()[0]<=hardness.value()[1]){
            this->hardness=hardness;
        }
        else{
            throw cleanAndError("硬度输入错误，第一个值大于第二个值");
        }
    }
    NEXT_ID++;
}

std::string RawPartType::to_string(){
    std::string s="";
    s=s+this->material+"\t"+std::to_string(this->width)+"\t"+std::to_string(this->height)+"\t"+std::to_string(this->thick)+"\t"+std::to_string(this->qty)+"\t";
    if(this->hardness!=std::nullopt){
        s=s+std::to_string(this->hardness.value()[0])+"-"+std::to_string(this->hardness.value()[1]);
    }
    return s;
}


RawSheetType::RawSheetType(int width,int height,int thick,int qty,std::string material,std::string stock,std::string stockLocation):
id(NEXT_ID),width(width),height(height),thick(thick),qty(qty),hardness(std::nullopt),material(material),stock(stock),stockLocation(stockLocation){
    NEXT_ID++;
}

RawSheetType::RawSheetType(int width,int height,int thick,int qty,std::array<double,2> hardness,std::string material,std::string stock,std::string stockLocation):
id(NEXT_ID),width(width),height(height),thick(thick),qty(qty),hardness(std::nullopt),material(material),stock(stock),stockLocation(stockLocation){
    if(hardness[0]<=hardness[1]){
        this->hardness=hardness;
    }
    else{
        throw cleanAndError("硬度输入错误，第一个值大于第二个值");
    }
    NEXT_ID++;
}
RawSheetType::RawSheetType(int width,int height,int thick,int qty,std::optional<std::array<double,2>> hardness,std::string material,std::string stock,std::string stockLocation):
id(NEXT_ID),width(width),height(height),thick(thick),qty(qty),hardness(std::nullopt),material(material),stock(stock),stockLocation(stockLocation){
    if(hardness!=std::nullopt){
        if(hardness.value()[0]<=hardness.value()[1]){
            this->hardness=hardness;
        }
        else{
            throw cleanAndError("硬度输入错误，第一个值大于第二个值");
        }
    }

    NEXT_ID++;
}
bool RawSheetType::canContain(RawPartType& rawPartType){
    // 判断硬度是否满足要求
    if(rawPartType.hardness!=std::nullopt){
        if(this->hardness==std::nullopt) return false;
        if(this->hardness.value()[0]<rawPartType.hardness.value()[0] || this->hardness.value()[1]>rawPartType.hardness.value()[1]) return false;
    }
    // 判断尺寸是否满足要求
    Vec3i rawPart={rawPartType.height,rawPartType.width,rawPartType.thick};
    Vec3i sheet={this->height,this->thick,this->width};
    std::sort(rawPart.begin(),rawPart.end());
    std::sort(sheet.begin(),sheet.end());
    if(rawPart[0]<=sheet[0] && rawPart[1]<=sheet[1] && rawPart[2]<=sheet[2]) return true;
    else return false;

}

std::string RawSheetType::to_string(){
    std::string s="";
    s=s+this->material+"\t"+std::to_string(this->width)+"\t"+std::to_string(this->height)+"\t"+std::to_string(this->thick)+"\t"+std::to_string(this->qty)+"\t"+this->stock+"\t"+this->stockLocation+"\t";
    if(this->hardness!=std::nullopt){
        s=s+std::to_string(this->hardness.value()[0])+"-"+std::to_string(this->hardness.value()[1]);
    }
    return s+"\n";
}

void RawProblem::readInventoryFromXlsx(std::string path){
    inventory.load(path);
    auto ws = inventory.active_sheet();
    auto str2Length=[](std::string s){
        std::string temp="";
        bool is_int=true;
        for(auto c:s){
            if(c=='.') is_int=false;
            else{
                temp=temp+c;
            }
        }
        if(is_int) temp=temp+'0';
        return std::stoi(temp);

    };
    auto str2Hardness=[](std::string s)->std::optional<std::array<double,2>>{
        std::regex pattern1(R"((\d+(\.\d+)?)-(\d+(\.\d+)?))");
        std::smatch matches1;
        if (std::regex_search(s, matches1, pattern1)) {
            double num1 = std::stod(matches1[1].str());
            double num2 = std::stod(matches1[3].str());
            return std::optional<std::array<double,2>>({num1,num2});
        }
        std::regex pattern2(R"(^\d+(\.\d+)?$)");
        std::smatch matches2;
        if (std::regex_search(s, matches2, pattern2)) {
            double num = std::stod(matches2[0].str());
            return std::optional<std::array<double,2>>({num,num});
        }
        return std::nullopt;
    };
    // 读取单元格数据
    for (const auto& row : ws.rows()) {
        int count=0;
        std::vector<std::string> rowStr;
        for (const auto& cell : row) {
            // std::cout << cell.to_string() << "\t"; // 打印单元格内容
            count++;
            rowStr.push_back(cell.to_string());
        }
        // std::cout<< std::endl;
        

        auto material=rowStr[0];
        // std::cout << material << std::endl;
        if(material=="" || material=="Material_Name") continue;
        auto height=str2Length(rowStr[1]);
        auto width=str2Length(rowStr[2]);
        auto thick=str2Length(rowStr[3]);

        auto qty=std::stoi(rowStr[4]);
        auto stock=rowStr[5];
        auto stock_location=rowStr[6];
        // std::cout<<"<"<<">"<<std::endl;
        std::optional<std::array<double,2>> hardness=std::nullopt;
        if(count==8){
            hardness=str2Hardness(rowStr[7]);
            if(hardness!=std::nullopt && hardness.value()[0]>hardness.value()[1]) continue;
        }

        if(sheetList.find(material)==sheetList.end()){
            sheetList.emplace(material,std::vector<RawSheetType>());
        }
        // std::cout<<4<<std::endl;
        sheetList[material].emplace_back(height,width,thick,qty,hardness,material,stock,stock_location);
        // sheetList[material].emplace_back(height,width,thick,qty,hardness,material,stock,stock_location);
        // std::cout<<sheetList[material].back().to_string();

    }
}

void RawProblem::readIngredientFromXlsx(std::string path){
    ingredient.load(path);
    auto ws = ingredient.active_sheet();
    auto str2Length=[](std::string s){
        std::string temp="";
        bool is_int=true;
        for(auto c:s){
            if(c=='.') is_int=false;
            else{
                temp=temp+c;
            }
        }
        if(is_int) temp=temp+'0';
        return std::stoi(temp);

    };
    auto str2Hardness=[](std::string s)->std::optional<std::array<double,2>>{
        std::regex pattern1(R"((\d+(\.\d+)?)-(\d+(\.\d+)?))");
        std::smatch matches1;
        if (std::regex_search(s, matches1, pattern1)) {
            double num1 = std::stod(matches1[1].str());
            double num2 = std::stod(matches1[3].str());
            return std::optional<std::array<double,2>>({num1,num2});
        }
        std::regex pattern2(R"(^\d+(\.\d+)?$)");
        std::smatch matches2;
        if (std::regex_search(s, matches2, pattern2)) {
            double num = std::stod(matches2[0].str());
            return std::optional<std::array<double,2>>({num,num});
        }
        return std::nullopt;
    };
    auto getAsciiPart=[](const std::string& str) {
        std::string asciiPart;
        for (char c : str) {
            // 检查字符是否是 ASCII 范围（0 到 127）
            if (static_cast<unsigned char>(c) <= 127) {
                asciiPart += c; // 添加到结果字符串中
            }
        }
        return asciiPart;
    };
    // 读取单元格数据
    for (const auto& row : ws.rows()) {
        int count=0;
        std::vector<std::string> rowStr;
        for (const auto& cell : row) {
            // std::cout << cell.to_string() << "\t"; // 打印单元格内容
            count++;
            rowStr.push_back(cell.to_string());
        }
        // std::cout<< std::endl;
        if(rowStr[0]=="" || rowStr[0]=="pl_id") continue;
        auto pl_id=std::stoi(rowStr[0]);
        auto material=getAsciiPart(rowStr[2]);
        // std::cout << material << std::endl;
        auto height=str2Length(rowStr[3]);
        auto width=str2Length(rowStr[4]);
        auto thick=str2Length(rowStr[5]);

        auto qty=std::stoi(rowStr[6]);
        // std::cout<<"<"<<">"<<std::endl;
        std::optional<std::array<double,2>> hardness=std::nullopt;
        if(count==8){
            hardness=str2Hardness(rowStr[7]);
            if(hardness!=std::nullopt && hardness.value()[0]>hardness.value()[1]) continue;
        }

        if(partList.find(pl_id)==partList.end()){
            partList.emplace(pl_id,std::vector<RawPartType>());
        }
        // std::cout<<4<<std::endl;
        bool exist=false;
        for(auto& part:partList[pl_id]){
            // if(pl_id<30) std::cout<<part.width<<" "<<width<<std::endl;
            if(part.width==width && part.height==height && part.thick==thick && part.material==material){
                part.qty+=qty;
                exist=true;
                break;
            }
        }
        if(!exist) partList[pl_id].emplace_back(width,height,thick,true,qty,hardness,material);
        // sheetList[material].emplace_back(height,width,thick,qty,hardness,material,stock,stock_location);
        // std::cout<<partList[pl_id].back().to_string();

    }
}

std::vector<RawPartType*> RawProblem::plids2vecRawPartType(std::vector<int> pl_ids){
    std::vector<RawPartType*> result={};
    for(int i:pl_ids){
        for(int j=0;j<partList[i].size();j++){
            result.push_back(&(partList[i][j]));
        }
    }
    return result;
}

std::vector<Problem> RawProblem::constructProblem(std::vector<int> pl_ids){
    std::vector<RawPartType*> rawPartTypes=plids2vecRawPartType(pl_ids);
    std::unordered_map<std::string,std::vector<RawPartType*>> rawProblemParts;
    std::unordered_map<std::string,std::vector<RawSheetType*>> rawProblemSheets;
    // 把订单里的不同parts按材料分开
    for(auto rpt:rawPartTypes){
        if(rawProblemParts.find(rpt->material)==rawProblemParts.end()){
            rawProblemParts[rpt->material]={};
            rawProblemSheets[rpt->material]={};
        }
        rawProblemParts[rpt->material].push_back(rpt);
    }
    // 找到对应材料的母板
    for(auto& pair:rawProblemParts){
        auto materialPair=sheetList.find(pair.first);
        if(materialPair==sheetList.end()) continue;
        auto& candSheetList=materialPair->second;
        for(auto& sheet:candSheetList){
            for(auto& part:rawProblemParts[pair.first]){
                if(sheet.canContain(*part)){
                    rawProblemSheets[materialPair->first].push_back(&sheet);
                    break;
                }
            }
            
        }
    }

    std::vector<Problem> problems;
    // 对每种材料的母板构建问题
    for(auto& pair:rawProblemSheets){
        problems.emplace_back();
        Problem& problem = problems.back();
        for(RawSheetType* sheet:pair.second){
            // std::cout<<sheet->stock<<std::endl;
            problem.addSheet(sheet->width,sheet->height,sheet->thick,sheet->qty,(sheet->stock=="立体货架仓")?true:false);
        }
        for(RawPartType* part:rawProblemParts[pair.first]){
            problem.addPart(part->width,part->height,part->thick,true,part->qty);
        }
    }
    return problems;
}

