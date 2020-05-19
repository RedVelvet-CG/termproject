#pragma once
#ifndef __TANK_H__
#define __TANK_H__

#include "cgmath.h"
#include <random>

struct tank {
	vec3	center = vec3(0);		// 2D position for translation
	int		dir = 0; //0: left	1: up	2: right	3: back
	vec4	color;				// RGBA color in [0,1]
	bool	isenemy = true;
	int		health = 4;
	mat4	model_matrix;		// modeling transformation
	float	dirPI[4] = { 0, -PI / 2, PI, PI / 2};
	int		creation_val = 3 * 2 * 6 * 3;
	int		plane = 0; //0: front	1: left		2: right	3: top	4: bottom	5:back
	bool	created = false;
	bool	is_dead = false;
	float	radius = 10.0f;		// radius
	vec3	movval = vec3(0);
	bool	movflag = false;
	float	timestamp = 0.0f;

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
	tank mytank = { vec3(-40.0f, -80.0f, 100.0f), 1, vec4(0.637f,1.0f,0.611f,0.0f), false};
	tank enemy1 = { vec3(-80.0f, 80.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f) };
	tank enemy2 = { vec3(0.0f, 80.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f) };
	tank enemy3 = { vec3(80.0f, 80.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f) };
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
		mat4::rotate(vec3(0, 0, 1), dirPI[dir]) *
		mat4::scale(radius, radius, radius);
}

inline void player_activate(tank* player, int dir, bool activate){
	if (activate) {
		player->dir = dir;
		player->movflag = true;
	}
	else {
		player->movflag = false;
	}
}

inline void player_move(tank* player) {
	int dir = player->dir;
	if (dir == 0) {
		if (player->plane == 0) if (player->center.x <= -80) return;
		player->center.x -= 0.05f;
	}
	else if (dir == 1) {
		if (player->plane == 0) if (player->center.y >= 80) return;
		player->center.y += 0.05f;
	}
	else if (dir == 2) {
		if (player->plane == 0) if (player->center.x >= 80) return;
		player->center.x += 0.05f;
	}
	else {
		if (player->plane == 0) if (player->center.y <= -80) return;
		player->center.y -= 0.05f;
	}
}

inline void enemy_move(tank* player, tank* enemy, float hash) {
	if (player->plane != enemy->plane) return;
	if (enemy->timestamp < 3.0f) {
		int dir = enemy->dir;
		if (dir == 0) {
			if (enemy->plane == 0) if (enemy->center.x <= -80) { enemy->timestamp = 3.0f;  return; }
			enemy->center.x -= 0.05f;
		}
		else if (dir == 1) {
			if (enemy->plane == 0) if (enemy->center.y >= 80) { enemy->timestamp = 3.0f;  return; }
			enemy->center.y += 0.05f;
		}
		else if (dir == 2) {
			if (enemy->plane == 0) if (enemy->center.x >= 80) { enemy->timestamp = 3.0f;  return; }
			enemy->center.x += 0.05f;
		}
		else {
			if (enemy->plane == 0) if (enemy->center.y <= -80) { enemy->timestamp = 3.0f;  return; }
			enemy->center.y -= 0.05f;
		}

		enemy->timestamp += 0.005f;
	}
	else {
		enemy->timestamp = 0.0f;
		srand((unsigned int)time(0));
		//printf("%d\n", (rand() * (unsigned int)hash) % 4);
		player_activate(enemy, (rand() * (unsigned int)hash) % 4, true);
	}
}


#endif