#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

using json = nlohmann::json;

json load_map(const std::string& filename){
    ifstream file(filename);
    json map_data;
    file >> map_data;
    return map_data;
}

int main() {
    // 数据数组
    json loading=load_map("../configs/testmap.json");

    auto layers=loading["layers"];
    auto firstlayer=layers[0];
    int width=firstlayer["width"];
    int height=firstlayer["height"];
    // 定义一个 10x10 的二维数组
    vector<int>data =firstlayer["data"];
    vector<vector<int>> array(10, vector<int>(10, 1));

    // 将数据存储到二维数组中
    int index = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            array[i][j] = data[index++];
        }
    }

    // 打印二维数组
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << array[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
