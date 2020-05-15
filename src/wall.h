#pragma once
#ifndef __WALL_H__
#define __WALL_H__

#include "cgmath.h"
#include "cgut.h"

struct wall_t
{
	vec3 center = vec3(0);
	bool breakable = true;
	bool broken = false;
	bool is_base = false;
	mat4 model_matrix;

	void update();
};

inline std::vector<wall_t> create_walls()
{
	std::vector<wall_t> walls;
	wall_t w;

	w = { vec3(2.0f, 2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(2.0f, 4.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(2.0f, 6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(6.0f, 2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(6.0f, 4.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(6.0f, 6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(0.f, 4.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(-2.0f, 2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-2.0f, 4.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-2.0f, 6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-6.0f, 2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-6.0f, 4.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(-6.0f, 6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-6.0f, -2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-2.0f, -2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(0.f, -2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(2.0f, -2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(6.0f, -2.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-6.0f, -4.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(6.0f, -4.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(-6.0f, -6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-2.0f, -6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(0.f, -6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(2.0f, -6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(6.0f, -6.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(2.0f, -8.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-2.0f, -8.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(0.0f, -8.0f, 1.0f),true,false,true };
	walls.emplace_back(w);

	return walls;

}

inline void wall_t::update()
{
	mat4 scale_matrix =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	mat4 translate_matrix =
	{
		1, 0, 0, center.x,
		0, 1, 0, center.y,
		0, 0, 1, center.z,
		0, 0, 0, 1
	};

	model_matrix = translate_matrix * scale_matrix;
}

#endif