#include <iostream>
#include <string>

#include "LuaScript.h" 

int main() {
    LuaScript script("Player.lua");
    float posX = script.get<float>("player.position.x");
    float posY = script.get<float>("player.position.y");
    std::string filename = script.get<std::string>("player.filename");
    int hp = script.get<int>("player.HP");

    std::cout<<"Position X = "<<posX<<", Y = "<<posY<<std::endl;
    std::cout<<"Filename:"<<filename<<std::endl;
    std::cout<<"HP:"<<hp<<std::endl;

    std::vector<int> v = script.getIntVector("array");
    std::cout<<"Contents of array:";
    for(std::vector<int>::iterator it = v.begin();
    	it != v.end();
    	it++) {
    	std::cout<<*it<<",";
    }
    std::cout<<std::endl;
}