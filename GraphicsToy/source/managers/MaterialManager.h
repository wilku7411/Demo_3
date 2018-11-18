#pragma once

#include "../graphics/Material.h"
#include "../IO/File.h"

#include "glm/glm.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

struct MaterialPrefab
{
    std::string name;
    glm::vec3 ambient, diffuse, specular;
    float shininess;
};

class CMaterialManager
{
public:
    CMaterialManager();
    CMaterialManager(const std::string& filePath);
    
    bool Init(const std::string& filePath);

    // Fills material data with information contained in prefab vector
    // Info is provided based on given name
    // If given name is not in material data file, then it will be set to default material (red_plastic)
    void GetMaterialPrefabData(const std::string& name, CMaterial& mat);

    // Fills material data with information contained prefab vector
    // Info is provided based on given id
    // If given id is grater than the number of materials in data file, 
    // it will mod that value, accordingly to the max number of materials in file
    void GetMaterialPrefabData(const unsigned int id, CMaterial& mat);
    
private:
    std::map<std::string, MaterialPrefab>   m_materials;
    std::string                             m_defaultPath{ "source/resources/shaders/_materials_data.txt" };
};