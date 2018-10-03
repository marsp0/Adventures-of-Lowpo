#include "Material.hpp"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shine) :
                    ambient(ambient), diffuse(diffuse),
                    specular(specular), shine(shine)
{

}