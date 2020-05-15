#pragma once
#ifndef __TANK_H__
#define __TANK_H__

#include "cgmath.h"
#include "cgut.h"

struct sphere {
	vec3	center = vec3(0);		// 2D position for translation
	float	radius = 10.0f;		// radius
	vec3	movval = vec3(0);
	float	theta = 0.0f; //rotation angle
	vec4	color;				// RGBA color in [0,1]
	//mat4	model_matrix;		// modeling transformation
	int		creation_val = 3 * 2 * 6 * 4;//50 * 3 * 50 * 2;

};

void create_circle_vertices(std::vector<vertex>& v, uint N, float xval, float yval, float zval, float diameter) {
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
	v.push_back({ vec3(0.4f,0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-0.4f,0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-0.4f,-0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.4f,-0.4f,0.5f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(0.4f,0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-0.3f,0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-0.4f,-0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(0.4f,-0.4f,0.f), vec3(0.f,0.f,0.0f), vec2(0, 1) });

	//barrel
	v.push_back({ vec3(-0.4f,0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-2.0f,0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-2.0f,-0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-0.4f,-0.2f,0.4f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-0.4f,0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-2.0f,0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(0, 1) });
	v.push_back({ vec3(-2.0f,-0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(1, 0) });
	v.push_back({ vec3(-0.4f,-0.2f,0.1f), vec3(0.f,0.f,0.0f), vec2(0, 1) });

	//body
	v.push_back({ vec3(20.f,20.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-20.f,20.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-20.f,-20.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(20.f,-20.f,-1.f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(20.f,20.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-20.f,20.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(-20.f,-20.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
	v.push_back({ vec3(20.f,-20.f,-1.3f), vec3(0.f,0.f,0.0f), vec2(0, 0) });
}

inline std::vector<sphere> create_universe() {
	std::vector<sphere> spheres;
	sphere sun = { vec3(0.f, 0.f,0.f),10.0f, vec3(0.0f, 0.0f, 0.0f), 0.0f, vec4(0.0f,0.0f,0.0f,0.0f) };
	spheres.emplace_back(sun);
	return spheres;
}

inline void make_cuboid(std::vector<uint>& v, int offset) {
	//front
	v.push_back(offset+0);	v.push_back(offset+1);	v.push_back(offset+2);
	v.push_back(offset+2);	v.push_back(offset+3);	v.push_back(offset+0);

	//left
	v.push_back(offset+1);	v.push_back(offset+5);	v.push_back(offset+6);
	v.push_back(offset+6);	v.push_back(offset+2);	v.push_back(offset+1);

	//right
	v.push_back(offset+4);	v.push_back(offset+0);	v.push_back(offset+3);
	v.push_back(offset+3);	v.push_back(offset+7);	v.push_back(offset+4);

	//top
	v.push_back(offset+4);	v.push_back(offset+5);	v.push_back(offset+1);
	v.push_back(offset+1);	v.push_back(offset+0);	v.push_back(offset+4);

	//bottom
	v.push_back(offset+3);	v.push_back(offset+2);	v.push_back(offset+6);
	v.push_back(offset+6);	v.push_back(offset+7);	v.push_back(offset+3);

	//back
	v.push_back(offset+5);	v.push_back(offset+4);	v.push_back(offset+7);
	v.push_back(offset+7);	v.push_back(offset+6);	v.push_back(offset+5);
}

std::vector<uint> makeindices(uint N) {
	std::vector<uint> rect;
	//body
	make_cuboid(rect, 0);
	//head
	make_cuboid(rect, 8);
	//barrel
	make_cuboid(rect, 16);
	//field
	make_cuboid(rect, 24);
	return rect;
}

#endif