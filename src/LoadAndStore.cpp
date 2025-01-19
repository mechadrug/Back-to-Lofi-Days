#include "../include/LoadAndStore.h"

void loadMovableObjectFromJson(const std::string& filename, MovableObject& obj) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return;
    }

    json j;
    inputFile >> j;
    inputFile.close();

    // 解析 MovableObject 的数据
    auto& data = j["MovableObject"];

    // 解析基本数值类型
    obj.position.x = data["position"]["x"].get<float>();
    obj.position.y = data["position"]["y"].get<float>();
    obj.speed = data["speed"].get<float>();
    obj.verticalSpeed = data["verticalSpeed"].get<float>();
    obj.gravity = data["gravity"].get<float>();
    obj.jumpHeight = data["jumpHeight"].get<float>();
    obj.isJumping = data["isJumping"].get<bool>();
    obj.inertiaSpeed = data["inertiaSpeed"].get<float>();
    obj.Health = data["Health"].get<int>();
    obj.Attack = data["Attack"].get<int>();
    obj.isDead = data["isDead"].get<bool>();
    obj.attackCooldown = data["attackCooldown"].get<float>();
    obj.lastAttackTime = std::chrono::steady_clock::time_point(std::chrono::milliseconds(data["lastAttackTime"].get<int>()));
    obj.intoWind = data["intoWind"].get<bool>();
    obj.invisibilityDuration = data["invisibilityDuration"].get<float>();
    obj.probWithoutBeingAttacked = data["probWithoutBeingAttacked"].get<float>();
    obj.atkRange = data["atkRange"].get<int>();
    obj.HealthCap = data["HealthCap"].get<int>();
    obj.money = data["money"].get<int>();
    obj.isInvisible = data["isInvisible"].get<bool>();
    obj.invisibilityCooldown = data["invisibilityCooldown"].get<float>();
    obj.isDodging = data["isDodging"].get<bool>();
    obj.isFlashing = data["isFlashing"].get<bool>();

    // 解析原始值结构体
    obj.originalValues.invisiTime = data["originalValues"]["invisiTime"].get<float>();
    obj.originalValues.attack = data["originalValues"]["attack"].get<int>();
    obj.originalValues.attackCooldown = data["originalValues"]["attackCooldown"].get<float>();
    obj.originalValues.health = data["originalValues"]["health"].get<int>();
    obj.originalValues.jumpHeight = data["originalValues"]["jumpHeight"].get<float>();
    obj.originalValues.speed = data["originalValues"]["speed"].get<float>();
    obj.originalValues.initprob = data["originalValues"]["initprob"].get<float>();
    obj.originalValues.atkRange = data["originalValues"]["atkRange"].get<int>();
    obj.originalValues.healthCap = data["originalValues"]["healthCap"].get<int>();
}
void saveMovableObjectToJson(const std::string& filename, const MovableObject& obj) {
    json j;

    // 写入 MovableObject 的数据
    j["MovableObject"] = {
        {"position", {{"x", obj.position.x}, {"y", obj.position.y}}},
        {"speed", obj.speed},
        {"verticalSpeed", obj.verticalSpeed},
        {"gravity", obj.gravity},
        {"jumpHeight", obj.jumpHeight},
        {"isJumping", obj.isJumping},
        {"inertiaSpeed", obj.inertiaSpeed},
        {"Health", obj.Health},
        {"Attack", obj.Attack},
        {"isDead", obj.isDead},
        {"attackCooldown", obj.attackCooldown},
        {"lastAttackTime", std::chrono::duration_cast<std::chrono::milliseconds>(obj.lastAttackTime.time_since_epoch()).count()},
        {"intoWind", obj.intoWind},
        {"invisibilityDuration", obj.invisibilityDuration},
        {"probWithoutBeingAttacked", obj.probWithoutBeingAttacked},
        {"atkRange", obj.atkRange},
        {"HealthCap", obj.HealthCap},
        {"money", obj.money},
        {"isInvisible", obj.isInvisible},
        {"invisibilityCooldown", obj.invisibilityCooldown},
        {"isDodging", obj.isDodging},
        {"isFlashing", obj.isFlashing},
        {"originalValues", {
            {"invisiTime", obj.originalValues.invisiTime},
            {"attack", obj.originalValues.attack},
            {"attackCooldown", obj.originalValues.attackCooldown},
            {"health", obj.originalValues.health},
            {"jumpHeight", obj.originalValues.jumpHeight},
            {"speed", obj.originalValues.speed},
            {"initprob", obj.originalValues.initprob},
            {"atkRange", obj.originalValues.atkRange},
            {"healthCap", obj.originalValues.healthCap}
        }}
    };

    // 打开文件并写入 JSON 数据
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << j.dump(4);  // 4 个空格缩进
        outputFile.close();
    } else {
        std::cerr << "Failed to open file!" << std::endl;
    }
}

void saveToFile(const std::string& filename) {
    json j;

    // 填充 JSON 数据
    j["acCheck"] = json::object();
    for (const auto& [key, value] : acCheck) {
        j["acCheck"][std::to_string(key)] = value;  // 将 acCheck 中的键值对转换为 JSON 格式
    }

    j["noteOwnStatus"] = json::object();
    for (const auto& [key, value] : noteOwnStatus) {
        j["noteOwnStatus"][std::to_string(key)] = value;  // 将 noteOwnStatus 中的键值对转换为 JSON 格式
    }

    // 其他数据
    j["c_idx"] = c_idx;
    j["map4win"] = map4win;
    j["map5win"] = map5win;
    j["map9win"] = map9win;
    j["getSevenNotesAndWin"] = getSevenNotesAndWin;
    j["getCat"] = getCat;

    // 将 JSON 数据写入文件
    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4); // 美化输出
        file.close();
        std::cout << "Data saved successfully to " << filename << std::endl;
    } else {
        std::cerr << "Failed to open file for saving!" << std::endl;
    }
}

void loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        json j;
        file >> j;  // 读取 JSON 数据

        // 定义字段解析函数映射
        std::unordered_map<std::string, std::function<void(const json&)>> field_map = {
            {"acCheck", [](const json& j) {
                for (const auto& item : j.items()) {
                    acCheck[std::stoi(item.key())] = item.value().get<bool>();
                }
            }},
            {"noteOwnStatus", [](const json& j) {
                for (const auto& item : j.items()) {
                    noteOwnStatus[std::stoi(item.key())] = item.value().get<bool>();
                }
            }},
            {"c_idx", [](const json& j) { c_idx = j.get<int>(); }},
            {"map4win", [](const json& j) { map4win = j.get<bool>(); }},
            {"map5win", [](const json& j) { map5win = j.get<bool>(); }},
            {"map9win", [](const json& j) { map9win = j.get<bool>(); }},
            {"getSevenNotesAndWin", [](const json& j) { getSevenNotesAndWin = j.get<bool>(); }},
            {"getCat", [](const json& j) { getCat = j.get<bool>(); }},
        };

        // 遍历并根据字段名执行相应的操作
        for (const auto& [key, value] : j.items()) {
            if (field_map.find(key) != field_map.end()) {
                field_map[key](value);
            } else {
                std::cerr << "Unknown field: " << key << std::endl;
            }
        }

        file.close();
        std::cout << "Data loaded successfully from " << filename << std::endl;
    } else {
        std::cerr << "Failed to open file for loading!" << std::endl;
    }
}

