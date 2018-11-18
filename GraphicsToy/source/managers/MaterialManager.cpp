#include "MaterialManager.h"


CMaterialManager::CMaterialManager()
{
    if (!Init(m_defaultPath))
    {
        printf_s("Error, couldn't read path: %s", m_defaultPath.c_str());
    }
}

CMaterialManager::CMaterialManager(const std::string& filePath)
{
    if (!Init(filePath))
    {
        printf_s("Error, couldn't read path: %s", filePath.c_str());
    }

}

bool CMaterialManager::Init(const std::string& filePath)
{
    std::ifstream file{ filePath };
    
    std::cout << "\nNumber of materials: " << std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n') + 1;

    file.clear();
    file.seekg(0);

    if (file.is_open())
    {
        MaterialPrefab a;
        while (file >> a.name >> a.ambient.r >> a.ambient.g >> a.ambient.b >> a.diffuse.r >> a.diffuse.g >> a.diffuse.b >> a.specular.r >> a.specular.g >> a.specular.b >> a.shininess)
        {
            m_materials[a.name] = a;
        }

        //// Check loaded values
        //for (const auto& e : m_materials)
        //{
        //    printf_s("\nName: %s\n\tambient[%2.4f,%2.4f,%2.4f]\n\tdiffuse[%2.4f,%2.4f,%2.4f]\n\tspecular[%2.4f,%2.4f,%2.4f]\n\tshininess[%2.4f]"
        //        , e.second.name.c_str()
        //        , e.second.ambient.x, e.second.ambient.y, e.second.ambient.z
        //        , e.second.diffuse.x, e.second.diffuse.y, e.second.diffuse.z
        //        , e.second.specular.x, e.second.specular.y, e.second.specular.z
        //        , e.second.shininess
        //    );
        //}
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void CMaterialManager::GetMaterialPrefabData(const std::string& name, CMaterial& mat)
{
    if (m_materials.find(name) == m_materials.end())
    {
        printf_s("\n\nCound not find information about %s material.\nPlease check if name is correct, and it corresponds to entry in \"resources\\shaders\\_materials_data.txt\"", name.c_str());
        printf_s("\nSetting default material from list [red_plastic]");
        mat.name = "red_plastic";
        mat.Ambient(m_materials["red_plastic"].ambient);
        mat.Diffuse(m_materials["red_plastic"].diffuse);
        mat.Specular(m_materials["red_plastic"].specular);
        mat.Shininess(m_materials["red_plastic"].shininess * 128.0f);
        return;
    }
    mat.name = name;
    mat.Ambient(m_materials[name].ambient);
    mat.Diffuse(m_materials[name].diffuse);
    mat.Specular(m_materials[name].specular);
    mat.Shininess(m_materials[name].shininess * 128.0f);
}

void CMaterialManager::GetMaterialPrefabData(const unsigned int id, CMaterial& mat)
{
    int i = id%m_materials.size();
    int counter = 0;

    for (auto elem : m_materials)
    {
        if (counter++ != i)
            continue;
        else
        {
            mat.name = elem.first;
            mat.Ambient(elem.second.ambient);
            mat.Diffuse(elem.second.diffuse);
            mat.Specular(elem.second.specular);
            mat.Shininess(elem.second.shininess * 128.0f);
            return;
        }
    }

    
}
