#pragma once

#include "../UniformChangesEvent.h"

class CMaterial : public IUniformAffect
{
public:
    std::string name;

public:
    CMaterial();

    void Update();
    void Notify() override;
public:
    glm::vec3 Ambient() const;
    glm::vec3 Diffuse() const;
    glm::vec3 Specular() const;
    float Shininess() const;
 
    void Ambient(glm::vec3 val);
    void Diffuse(glm::vec3 val);
    void Specular(glm::vec3 val);
    void Shininess(float val);

private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    bool b_needsUpdate = true;
};