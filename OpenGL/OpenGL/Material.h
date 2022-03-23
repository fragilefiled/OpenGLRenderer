#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
class Material
{
public:
	Texture *albedo;
	Texture *spec;
	Texture *normal;
	Material(Texture* albedo, Texture* spec, Texture* normal) :albedo(albedo), spec(spec), normal(normal) {};
	~Material() 
	{  };






};