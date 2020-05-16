#pragma once
#ifndef __FIELD_H__
#define __FIELD_H__

#include "cgmath.h"
#include "cgut.h"

struct field {
	vec3	center = vec3(0);		// 2D position for translation
	float	radius = 10.0f;		// radius
	vec3	movval = vec3(0);
	float	theta = 0.0f; //rotation angle
	vec4	color;				// RGBA color in [0,1]
	mat4	model_matrix;		// modeling transformation
	int		creation_val = 3 * 2 * 6;

	inline void update();
};

void create_field_vertices(std::vector<vertex>& v) {
	//body
	v.push_back({ vec3(9.f,9.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-9.f,9.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-9.f,-9.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(9.f,-9.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(9.f,9.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-9.f,9.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-9.f,-9.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(9.f,-9.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
}

inline std::vector<field> create_field() {
	std::vector<field> spheres;
	field sun = { vec3(0.f, 0.f,0.f),10.0f, vec3(0.0f, 0.0f, 0.0f), 0.0f, vec4(0.0f,0.0f,0.0f,0.0f) };
	spheres.emplace_back(sun);
	return spheres;
}

inline void make_field_part(std::vector<uint>& v, int offset) {
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

inline void make_field_indices(std::vector<uint>& v, uint N) {
	//field
	make_field_part(v, N+0);
}

inline void field::update() {
	model_matrix = mat4::rotate(vec3(0, 1, 0),movval.y) *  //rotation around sun
		mat4::translate(movval.x, 0, 0) *
		mat4::rotate(vec3(0, 1, 0),0) * //self-rotation
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 0, 1),0) *
		mat4::scale(radius, radius, radius);
}

#endif