#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__

#include "cgmath.h"
#include "cgut.h"
#include "tank.h"

struct bullet {
	vec3	center = vec3(0);
	bool	is_mine = false;
	int		plane = 0; //0: front	1: left		2: right	3: top	4: bottom	5:back
	int		dir = 0; //0: left	1: up	2: right	3: back
	vec3	planevec[6] = { {0,0,0}, {0, -PI / 2, 0}, {0, PI / 2, 0}, {-PI / 2, 0, 0}, {PI / 2, 0, 0}, {0, PI, 0} };
	float	dirvec[4] = { 0, -PI / 2, PI, PI / 2 };
	float	theta = 0.0f;
	float	radius = 10.0f;
	mat4	model_matrix;
	int		creation_val = 3*14;
	vec4	color = vec4(0.75f, 0.75f, 0.75f, 0.f);
	vec3	movvec[4] = { {-0.5, 0, 0}, {0, 0.5, 0}, {0.5, 0, 0}, {0, -0.5, 0} };


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

	v.push_back(offset + 5);	v.push_back(offset + 8);	v.push_back(offset + 6);
	v.push_back(offset + 8);	v.push_back(offset + 7);	v.push_back(offset + 6);

}

void make_bullet_indices(std::vector<uint>& v, uint N) {
	make_bullet_part(v, N + 0);
}

inline void bullet::update() {
	center += movvec[dir];
	model_matrix = mat4::rotate(vec3(1, 0, 0), planevec[plane].x) *  //rotation around sun
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 1, 0), planevec[plane].y) *
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 0, 1), planevec[plane].z) *
		mat4::translate(center.x, center.y, center.z) *
		mat4::rotate(vec3(0, 0, 1), dirvec[dir]) *
		mat4::scale(radius, radius, radius);
}

std::vector<bullet> create_bullet(std::vector<bullet> b, tank t) {
	bool players_bullet = true;
	if (t.isenemy) players_bullet = false;
	bullet new_bullet = { vec3(t.center.x,t.center.y,t.center.z),players_bullet, t.plane, t.dir, t.tank_id };
	b.emplace_back(new_bullet);
	return b;
}

#endif