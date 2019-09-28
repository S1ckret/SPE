#pragma once
#include "Graphics\Shader.h"
enum Material_Table {
	Rock = 0,
	Wood,
	Metal,
	BouncyBall,
	SuperBall,
	Pillow,
	Static
};

struct MassData
{
	MassData() :
		mass(1.f),
		inv_mass(1.f),
		I(1.f),
		inv_I(1.f)
	{}
	float mass;
	float inv_mass;

	float I;
	float inv_I;
};

struct Material
{
	float density;
	float restitution;

	unsigned int draw_type;
	Shader shader;

	Material() :
		density(0.f),
		restitution(0.f),
		draw_type(GL_LINE_LOOP)
	{
		shader.SetFilePath("res/shaders/BasicColor.shader");
	}

	void Set(Material_Table Type)
	{
		shader.SetFilePath("res/shaders/BasicColor.shader");
		switch (Type) {
		case Material_Table::Rock: density = 0.6f; restitution = 0.1f;
			break;
		case Material_Table::Wood: density = 0.3f; restitution = 0.2f;
			break;
		case Material_Table::Metal: density = 1.2f; restitution = 0.05f;
			break;
		case Material_Table::BouncyBall: density = 0.3f; restitution = 0.8f;
			break;
		case Material_Table::SuperBall: density = 0.3f; restitution = 0.95f;
			break;
		case Material_Table::Pillow: density = 0.1f; restitution = 0.2f;
			break;
		case Material_Table::Static: density = 0.0f; restitution = 0.4f;
			break;
		default: density = 0.0f; restitution = 0.0f;
			break;
		}
	}
};