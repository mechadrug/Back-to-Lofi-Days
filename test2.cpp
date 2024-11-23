#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;

int main() {
    // 1. 读取并解析 XML 文件
    XMLDocument doc;
    if (doc.LoadFile("example.xml") != XML_SUCCESS) {
        std::cerr << "Failed to load XML file!" << std::endl;
        return -1;
    }

    // 2. 查找根元素
    XMLElement* root = doc.RootElement();  // <root>
    if (root == nullptr) {
        std::cerr << "Failed to find root element!" << std::endl;
        return -1;
    }

    // 3. 遍历并读取所有 <person> 元素的信息
    for (XMLElement* person = root->FirstChildElement("person"); person != nullptr; person = person->NextSiblingElement("person")) {
        XMLElement* nameElement = person->FirstChildElement("name");
        XMLElement* ageElement = person->FirstChildElement("age");

        if (nameElement && ageElement) {
            const char* name = nameElement->GetText();  // 获取 <name> 元素的文本内容
            int age = 0;
            ageElement->QueryIntText(&age);  // 获取 <age> 元素的文本并转为整数

            std::cout << "Name: " << name << ", Age: " << age << std::endl;
        }
    }

    // 4. 修改 XML 文件 (添加一个新的 <person>)
    XMLElement* newPerson = doc.NewElement("person");
    XMLElement* newName = doc.NewElement("name");
    newName->SetText("Alice Johnson");
    XMLElement* newAge = doc.NewElement("age");
    newAge->SetText(35);

    newPerson->InsertEndChild(newName);
    newPerson->InsertEndChild(newAge);
    root->InsertEndChild(newPerson);

    // 5. 保存修改后的 XML 文件
    if (doc.SaveFile("example_modified.xml") != XML_SUCCESS) {
        std::cerr << "Failed to save modified XML file!" << std::endl;
        return -1;
    }

    std::cout << "New person added and saved to example_modified.xml!" << std::endl;

    return 0;
}
