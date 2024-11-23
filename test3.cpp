#include "json.hpp" // 包含 JSON 头文件
#include <iostream>
#include<fstream>
#include<string>
using json = nlohmann::json;
using namespace std;
int main() {
    // 创建 JSON 对象
    json player = {
        {"name", "John"},
        {"level", 10},
        {"experience", 2300},
        {"inventory", {"sword", "shield", "potion"}}
    };

    // 输出 JSON 数据
    std::cout << "Player Info:\n" << player.dump(4) << std::endl;

    // 读取 JSON 数据
    std::string name = player["name"];
    int level = player["level"];
    int experience = player["experience"];

    std::cout << "Name: " << name << ", Level: " << level
              << ", Experience: " << experience << std::endl;

    // 修改 JSON 数据
    player["level"] = 11;
    player["inventory"].push_back("bow");

    // 保存到文件
    std::ofstream file("save.json");
    file << player.dump(4);
    file.close();

    return 0;
}
