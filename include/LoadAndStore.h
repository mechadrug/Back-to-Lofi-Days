#ifndef LOADANDSTORE_H
#define LOADANDSTORE_H
#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include "../include/Player.h"
#include "../include/GlobalVar.h"
using json = nlohmann::json;
void loadMovableObjectFromJson(const std::string& filename, MovableObject& obj);
void saveMovableObjectToJson(const std::string& filename, const MovableObject& obj);
void saveToFile(const std::string& filename);
void loadFromFile(const std::string& filename);

#endif
