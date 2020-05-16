#pragma once
#ifndef __TANK_H__
#define __TANK_H__

#include "cgmath.h"
#include "cgut.h"

struct tank {
	vec3	center = vec3(0);		// 2D position for translation
	float	theta = 0.0f; //rotation angle
	vec4	color;				// RGBA color in [0,1]
	mat4	model_matrix;		// modeling transformation
	int		creation_val = 3 * 2 * 6 * 3;
	bool	created = false;
	bool	is_dead = false;
	float	radius = 10.0f;		// radius
	vec3	movval = vec3(0);

	inline void update();
};

void create_tank_vertices(std::vector<vertex>& v) {
	//body
	v.push_back({ vec3(1.f,1.f,0.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-1.f,1.f,0.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-1.f,-1.f,0.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(1.f,-1.f,0.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(1.f,1.f,-1.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-1.f,1.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-1.f,-1.f,-1.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(1.f,-1.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	
	//head
	v.push_back({ vec3(0.9f,0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.1f,0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(0.1f,-0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.9f,-0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(0.9f,0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.1f,0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(0.1f,-0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.9f,-0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });	
	
	//barrel
	v.push_back({ vec3(0.1f,0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-1.0f,0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-1.0f,-0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.1f,-0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(0.1f,0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-1.0f,0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-1.0f,-0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.1f,-0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	
}

inline std::vector<tank> create_tank() {
	std::vector<tank> spheres;
	tank mytank = { vec3(-40.0f, -80.0f, 0.0f), -PI/2, vec4(0.0f,0.0f,0.0f,0.0f) };
	tank enemy1 = { vec3(-80.0f, 80.0f, 0.0f), PI/2, vec4(0.0f,0.0f,0.0f,0.0f) };
	tank enemy2 = { vec3(0.0f, 80.0f, 0.0f), PI/2, vec4(0.0f,0.0f,0.0f,0.0f) };
	tank enemy3 = { vec3(80.0f, 80.0f, 0.0f), PI/2, vec4(0.0f,0.0f,0.0f,0.0f) };
	spheres.emplace_back(mytank);
	spheres.emplace_back(enemy1);
	spheres.emplace_back(enemy2);
	spheres.emplace_back(enemy3);
	//spheres.emplace_back(tank2);
	return spheres;
}

inline void make_tank_part(std::vector<uint>& v, int offset) {
	//front
	v.push_back(offset + 0);	v.push_back(offset + 1);	v.push_back(offset + 2);
	v.push_back(offset + 2);	v.push_back(offset + 3);	v.push_back(offset + 0);

	//left
	v.push_back(offset + 1);	v.push_back(offset + 5);	v.push_back(offset + 6);
	v.push_back(offset + 6);	v.push_back(offset + 2);	v.push_back(offset + 1);

	//right
	v.push_back(offset + 4);	v.push_back(offset + 0);	v.push_back(offset + 3);
	v.push_back(offset + 3);	v.push_back(offset + 7);	v.push_back(offset + 4);

	//top
	v.push_back(offset + 4);	v.push_back(offset + 5);	v.push_back(offset + 1);
	v.push_back(offset + 1);	v.push_back(offset + 0);	v.push_back(offset + 4);

	//bottom
	v.push_back(offset + 3);	v.push_back(offset + 2);	v.push_back(offset + 6);
	v.push_back(offset + 6);	v.push_back(offset + 7);	v.push_back(offset + 3);

	//back
	v.push_back(offset + 5);	v.push_back(offset + 4);	v.push_back(offset + 7);
	v.push_back(offset + 7);	v.push_back(offset + 6);	v.push_back(offset + 5);
}

inline void make_tank_indices(std::vector<uint>& v, uint N) {
	//body
	make_tank_part(v, N+0);
	//head
	make_tank_part(v, N+8);
	//barrel
	make_tank_part(v, N+16);
}

inline void tank::update() {
	model_matrix = mat4::rotate(vec3(0, 1, 0), 0) *  //rotation around sun
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 0, 0), 0) * //self-rotation
		mat4::translate(center.x, center.y, center.z) *
		mat4::rotate(vec3(0, 0, 1), theta) *
		mat4::scale(radius, radius, radius);
}

#endif