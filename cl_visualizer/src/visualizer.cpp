#include <visualizer.hpp>

size_t Visualizer::num=0;
int Visualizer::current=0;
int Visualizer::current_solution=0;
std::vector<Cube> Visualizer::layouts={};
std::map<std::string,Vec3i> Visualizer::cubeStatus;
StagePatterns* Visualizer::patterns=nullptr;
History* Visualizer::history=nullptr;
int Visualizer::history_length=0;
Solutions* Visualizer::solutions=nullptr;
int Visualizer::solutions_length=0;
std::vector<polyscope::SurfaceMesh*> Visualizer::cuts;
int Visualizer::current_cut=0;
char selected[100]="";



void Callback() {
    if(ImGui::InputInt("Current Blueprint", &Visualizer::current)){
        if(Visualizer::current<-1) Visualizer::current=0;
        else if(Visualizer::current==-1) Visualizer::current=Visualizer::layouts.size()-1;
        else if(Visualizer::current>Visualizer::layouts.size()) Visualizer::current=Visualizer::layouts.size()-1;
        else if(Visualizer::current==Visualizer::layouts.size()) Visualizer::current=0;
        polyscope::removeAllStructures();
        Visualizer::display(Visualizer::current);
    }
    if(ImGui::InputInt("Current Solution", &Visualizer::current_solution)){
        if(Visualizer::current_solution<-1) Visualizer::current_solution=0;
        else if(Visualizer::current_solution==-1) Visualizer::current_solution=Visualizer::Visualizer::history_length-1;
        else if(Visualizer::current_solution>Visualizer::Visualizer::history_length) Visualizer::current_solution=Visualizer::history_length-1;
        else if(Visualizer::current_solution==Visualizer::Visualizer::history_length) Visualizer::current_solution=0;
        polyscope::removeAllStructures();
        Visualizer::from_pattern_solution(Visualizer::patterns,(*Visualizer::history)[Visualizer::current_solution]);
    }

    if(ImGui::InputInt("Current Cut", &Visualizer::current_cut)){
        if(Visualizer::current_cut<0) Visualizer::current_cut=0;
        else if(Visualizer::current_cut>=Visualizer::cuts.size()) Visualizer::current_cut=Visualizer::cuts.size()-1;
        Visualizer::cuts[Visualizer::current_cut]->setSurfaceColor({0.5,0.5,0});
        Visualizer::current_cut=(Visualizer::current_cut+1)%Visualizer::cuts.size();
        Visualizer::cuts[Visualizer::current_cut]->setSurfaceColor({1,0,0});
    }
    // char text[100]="";
    // std::string str=Visualizer::cuts[Visualizer::current_cut]->name;
    // for(int i=0;i<str.size();i++){
    //     text[i]=str[i];
    // }
    // ImGui::Text(text);

    if(Visualizer::history_length!=0 && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_UpArrow)){
        Visualizer::current_solution=(Visualizer::current_solution+1)%Visualizer::history_length;
        polyscope::removeAllStructures();
        Visualizer::from_pattern_solution(Visualizer::patterns,(*Visualizer::history)[Visualizer::current_solution]);
    }

    if(Visualizer::history_length!=0 && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_DownArrow)){
        Visualizer::current_solution=(Visualizer::current_solution+Visualizer::history_length-1)%Visualizer::history_length;
        polyscope::removeAllStructures();
        Visualizer::from_pattern_solution(Visualizer::patterns,(*Visualizer::history)[Visualizer::current_solution]);
    }

    if(Visualizer::solutions_length!=0 && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_UpArrow)){
        Visualizer::current_solution=(Visualizer::current_solution+1)%Visualizer::solutions_length;
        polyscope::removeAllStructures();
        Visualizer::from_solution(*(*Visualizer::solutions)[Visualizer::current_solution]);
    }

    if(Visualizer::solutions_length!=0 && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_DownArrow)){
        Visualizer::current_solution=(Visualizer::current_solution+Visualizer::solutions_length-1)%Visualizer::solutions_length;
        polyscope::removeAllStructures();
        Visualizer::from_solution(*(*Visualizer::solutions)[Visualizer::current_solution]);
    }

    if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftArrow)){
        Visualizer::current=(Visualizer::current+Visualizer::layouts.size()-1)%Visualizer::layouts.size();
        polyscope::removeAllStructures();
        Visualizer::display(Visualizer::current);
    }

    if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_RightArrow)){
        Visualizer::current=(Visualizer::current+1)%Visualizer::layouts.size();
        polyscope::removeAllStructures();
        Visualizer::display(Visualizer::current);
    }

    if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Space)){
        polyscope::removeAllStructures();
        Visualizer::display(Visualizer::current);
    }

    if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_D)){
        Visualizer::cuts[Visualizer::current_cut]->setSurfaceColor({0.5,0.5,0});
        Visualizer::current_cut=(Visualizer::current_cut+1)%Visualizer::cuts.size();
        Visualizer::cuts[Visualizer::current_cut]->setSurfaceColor({1,0,0});
    }

    if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_A)){
        Visualizer::cuts[Visualizer::current_cut]->setSurfaceColor({0.5,0.5,0});
        Visualizer::current_cut=(Visualizer::current_cut+Visualizer::cuts.size()-1)%Visualizer::cuts.size();
        Visualizer::cuts[Visualizer::current_cut]->setSurfaceColor({1,0,0});
    }

    if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R)){
        polyscope::removeAllStructures();
        Visualizer::display(Visualizer::current);
    }

    ImGui::InputText("Selected", selected,100);
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseClicked[0]) { // if clicked
        glm::vec2 screenCoords{io.MousePos.x, io.MousePos.y};
        polyscope::PickResult pickResult = polyscope::pickAtScreenCoords(screenCoords);
        if(Visualizer::cubeStatus.find(pickResult.structureName)!=Visualizer::cubeStatus.end()){
            Vec3i size=Visualizer::cubeStatus[pickResult.structureName];
            std::string output=pickResult.structureName+": ("+std::to_string(size[0])+","+std::to_string(size[1])+","+std::to_string(size[2])+")";
            for(int i=0;i<output.size();i++){
                selected[i]=output[i];
            }
            for(int i=output.size();i<100;i++){
                selected[i]=' ';
            }
        }
    }
}

void Visualizer::init(){
    polyscope::init();
    polyscope::view::moveScale=2;
    polyscope::state::userCallback = Callback;
    
}

void Visualizer::show(){
    display(current);
    polyscope::show();
}

void Visualizer::display(int i){
    current_cut=0;
    from_cube(layouts[i]);
    cuts[current_cut]->setSurfaceColor({1,0,0});
}


void Visualizer::add_part(const Vec3i location,const Vec3i size){
    auto pair=add_cube(location,size);
    polyscope::SurfaceMesh* mesh=pair.first;
    polyscope::CurveNetwork* line=pair.second;
    mesh->setSurfaceColor({0.5,1,0.5});
}
void Visualizer::add_leftover(const Vec3i location,const Vec3i size){
    auto pair=add_cube(location,size);
    polyscope::SurfaceMesh* mesh=pair.first;
    polyscope::CurveNetwork* line=pair.second;
    mesh->setSurfaceColor({0.5,0.5,1});
    mesh->setTransparency(0.2);
}
void Visualizer::add_cutloss(const Vec3i location,const Vec3i size){
    auto pair=add_cube(location,size);
    polyscope::SurfaceMesh* mesh=pair.first;
    polyscope::CurveNetwork* line=pair.second;
    mesh->setSurfaceColor({0.5,0.5,0});
    mesh->setTransparency(0.7);
    cuts.push_back(mesh);
}
std::pair<polyscope::SurfaceMesh*,polyscope::CurveNetwork*> Visualizer::add_cube(const Vec3i location,const Vec3i size){
    std::vector<std::array<double, 3>> vertices = {
        {0, 0, 0},  // 0
        {1, 0, 0},  // 1
        {1, 1, 0},  // 2
        {0, 1, 0},  // 3
        {0, 0, 1},  // 4
        {1, 0, 1},  // 5
        {1, 1, 1},  // 6
        {0, 1, 1}   // 7
    };
    for(auto& vertice:vertices){
        vertice={((double)location[0]+vertice[0]*(double)size[0])/10,((double)location[1]+vertice[1]*(double)size[1])/10,((double)location[2]+vertice[2]*(double)size[2])/10};
    }
    std::string cubeName="Cube"+std::to_string(num);
    polyscope::SurfaceMesh* mesh=polyscope::registerSurfaceMesh(cubeName, vertices, faces);
    cubeStatus[cubeName]=size;
    polyscope::CurveNetwork* line=polyscope::registerCurveNetwork("Edge"+std::to_string(num), vertices,edges);
    line->setColor({0,0,0});
    line->setRadius(0);
    num++;
    return {mesh,line};
}

const std::vector<std::array<size_t, 3>> Visualizer::faces= {
    {0, 1, 2}, {0, 2, 3},
    {4, 5, 6}, {4, 6, 7},
    {0, 1, 5}, {0, 5, 4},
    {1, 2, 6}, {1, 6, 5},
    {2, 3, 7}, {2, 7, 6},
    {3, 0, 4}, {3, 4, 7}
};

const std::vector<std::array<size_t, 2>> Visualizer::edges = {
    {0, 1}, {1, 2}, {2, 3},
    {4, 5}, {5, 6}, {6, 7},
    {0, 4}, {1, 5}, {3, 0},
    {7, 4}, {2, 6}, {3, 7}
};

void Visualizer::from_cube(Cube& cube){
    cuts.clear();
    std::vector<Cube*> cubes=cube.get_all_cubes();
    // std::cout<<cubes.size()<<std::endl;
    for(auto cube:cubes){
        switch (cube->type)
        {
        case NodeType::PART:
            add_part(cube->location,cube->size);
            break;
        case NodeType::CUTLOSS:
            add_cutloss(cube->location,cube->size);
            break;
        case NodeType::LEFTOVER:
            add_leftover(cube->location,cube->size);
            break;
        default:
            break;
        }
    }
}

void Visualizer::from_process(StagePatterns* patterns,Process* process){
    from_history(patterns,process->history);
}

void Visualizer::from_history(StagePatterns* patterns,History& history){
    Visualizer::patterns=patterns;
    Visualizer::history=&history;
    Visualizer::history_length=history.size();
    Visualizer::current_solution=Visualizer::history_length-1;
    from_pattern_solution(patterns,history[current_solution]);
}


void Visualizer::from_pattern_solution(StagePatterns* patterns,PatternSolution* solution){
    Visualizer::current_cut=0;
    Visualizer::layouts.clear();
    for(const auto& blueprint:solution->blueprints){
        // std::cout<<blueprint->sheetID<<std::endl;
        Node* node=patterns->to_node(blueprint->top);
        Cube cube(node,{0,0,0});
        delete node;
        Visualizer::layouts.push_back(cube);
    }
    from_cube(layouts[0]);
    polyscope::removeAllStructures();
    Visualizer::display(Visualizer::current);
}

void Visualizer::from_solution(Solution& solution){
    Visualizer::current_cut=0;
    Visualizer::layouts.clear();
    for(const auto& node:solution.solution){
        // std::cout<<blueprint->sheetID<<std::endl;
        Cube cube(node,{0,0,0});
        Visualizer::layouts.push_back(cube);
    }
    from_cube(layouts[0]);
    polyscope::removeAllStructures();
    Visualizer::display(Visualizer::current);
}

void Visualizer::from_solutions(Solutions* solutions){
    Visualizer::solutions=solutions;
    Visualizer::solutions_length=solutions->size();
    Visualizer::current_solution=0;
    from_solution(*(*solutions)[current_solution]);
}