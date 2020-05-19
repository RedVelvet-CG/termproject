#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__

#include "cgmath.h"
#include "cgut.h"
#include "tank.h"

struct bullet {
	vec3	center = vec3(0);
	bool	is_mine = false;
	float	theta = 0.0f;
	float	radius = 10.0f;
	mat4	model_matrix;
	int		creation_val = 14;
	vec4	color = (0.75f, 0.75f, 0.75f, 1.f);

	void update();
};

void create_bullet_vertices(std::vector<vertex>& v) {

	//head
	v.push_back({ vec3(0),vec3(0),vec2(0) });
	v.push_back({ vec3(0.25f,0.2f,0.15f),vec3(0),vec2(0) });
	v.push_back({ vec3(0.25f,0.2f,-0.15f),vec3(0),vec2(0) });
	v.push_back({ vec3(0.25f,-0.2f,-0.15f),vec3(0),vec2(0) });
	v.push_back({ vec3(0.25f,-0.2f,0.15f),vec3(0),vec2(0) });

	//body
	v.push_back({ vec3(0.5f,0.2f,0.15f),vec3(0),vec2(0) });
	v.push_back({ vec3(0.5f,0.2f,-0.15f),vec3(0),vec2(0) });
	v.push_back({ vec3(0.5f,-0.2f,-0.15f),vec3(0),vec2(0) });
	v.push_back({ vec3(0.5f,-0.2f,0.15f),vec3(0),vec2(0) });

}

void make_bullet_part(std::vector<uint>& v, int offset) {
	//head
	v.push_back(offset + 0);	v.push_back(offset + 1);	v.push_back(offset + 2);
	v.push_back(offset + 0);	v.push_back(offset + 2);	v.push_back(offset + 3);
	v.push_back(offset + 0);	v.push_back(offset + 3);	v.push_back(offset + 4);
	v.push_back(offset + 0);	v.push_back(offset + 4);	v.push_back(offset + 1);

	//body
	v.push_back(offset + 1);	v.push_back(offset + 6);	v.push_back(offset + 2);
	v.push_back(offset + 6);	v.push_back(offset + 1);	v.push_back(offset + 5);

	v.push_back(offset + 2);	v.push_back(offset + 7);	v.push_back(offset + 3);
	v.push_back(offset + 7);	v.push_back(offset + 2);	v.push_back(offset + 6);

	v.push_back(offset + 3);	v.push_back(offset + 8);	v.push_back(offset + 4);
	v.push_back(offset + 8);	v.push_back(offset + 3);	v.push_back(offset + 7);

	v.push_back(offset + 4);	v.push_back(offset + 5);	v.push_back(offset + 1);
	v.push_back(offset + 5);	v.push_back(offset + 4);	v.push_back(offset + 8);

	v.push_back(offset + 5);	v.push_back(offset + 6);	v.push_back(offset + 8);
	v.push_back(offset + 8);	v.push_back(offset + 6);	v.push_back(offset + 7);

}

void make_bullet_indices(std::vector<uint>& v, uint N) {
	make_bullet_part(v, N + 0);
}

inline void bullet::update() {
	model_matrix = mat4::rotate(vec3(0, 1, 0), 0) *  //rotation around sun
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 0, 0), 0) * //self-rotation
		mat4::translate(center.x, center.y, center.z) *
		mat4::rotate(vec3(0, 0, 1), theta) *
		mat4::scale(radius, radius, radius);
}

std::vector<bullet> create_bullet(std::vector<bullet> b, tank t) {
	bool players_bullet = true;
	if (t.isenemy) players_bullet = false;
	bullet new_bullet = { vec3(t.center.x - 1.0f,t.center.y,t.center.z + 0.25f),players_bullet };
	if (t.dir == 0) new_bullet.theta = 0;
	else if (t.dir == 1) new_bullet.theta = -PI / 2;
	else if (t.dir == 2) new_bullet.theta = PI;
	else if (t.dir == 3) new_bullet.theta = PI / 2;

	b.emplace_back(new_bullet);
	return b;
}

#endif