#pragma once
#ifndef __TANK_H__
#define __TANK_H__

#include "cgmath.h"
#include <random>
#include "wall.h"
#include <cmath>

struct tank {
	vec3	center = vec3(0);		// 2D position for translation
	int		dir = 0; //0: left	1: up	2: right	3: back
	vec4	color;				// RGBA color in [0,1]
	int		plane = 0; //0: front	1: left		2: right	3: top	4: bottom	5:back
	int		tank_id = 0;
	bool	isenemy = true;
	bool	movflag = true;
	vec3	planevec[6] = { {0,0,0}, {0, -PI / 2, 0}, {0, PI / 2, 0}, {-PI / 2, 0, 0}, {PI / 2, 0, 0}, {0, PI, 0} };
	bool	movplane = false;
	int		health = 4;
	mat4	model_matrix;		// modeling transformation
	int		creation_val = 3 * 2 * 6 * 3;
	float	radius = 10.0f;		// radius
	vec3	movval = vec3(0);
	float	timestamp = 0.0f;
	float	bulletstamp = 0.0f;
	int		diroffset = 0;
	int		dirs[4] = { 0,1,2,3 };
	float	dirvec[4] = { 0, -PI / 2, PI, PI / 2 };
	bool	can_change_dir = true;

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
	tank mytank = { vec3(-40.0f, 0.0f, 100.0f), 1, vec4(0.637f,1.0f,0.611f,0.0f), 0, 0, false, false };
	spheres.emplace_back(mytank);

	//front enemy	
	tank enemy1, enemy2, enemy3, enemy4;
	enemy1 = { vec3(-60.0f, 80.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 0,1 };
	enemy2 = { vec3(-60.0f, -80.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 0,2 };
	enemy3 = { vec3(60.0f, 80.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 0,3 };
	enemy4 = { vec3(60.0f, -80.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 0,4 };
	spheres.emplace_back(enemy1);
	spheres.emplace_back(enemy2);
	spheres.emplace_back(enemy3);
	spheres.emplace_back(enemy4);

	//left enemy
	enemy1 = { vec3(-40.0f, 40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 1,5 };
	enemy2 = { vec3(-40.0f, -40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 1,6 };
	enemy3 = { vec3(40.0f, 40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 1,7 };
	enemy4 = { vec3(40.0f, -40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 1,8 };
	spheres.emplace_back(enemy1);
	spheres.emplace_back(enemy2);
	spheres.emplace_back(enemy3);
	spheres.emplace_back(enemy4);

	//right enemy
	enemy1 = { vec3(-40.0f, 40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 2,9 };
	enemy2 = { vec3(-40.0f, -40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 2,10 };
	enemy3 = { vec3(40.0f, 40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 2,11 };
	enemy4 = { vec3(40.0f, -40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 2, 12 };
	spheres.emplace_back(enemy1);
	spheres.emplace_back(enemy2);
	spheres.emplace_back(enemy3);
	spheres.emplace_back(enemy4);

	//top enemy
	enemy1 = { vec3(-40.0f, 40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 3,13 };
	enemy2 = { vec3(-40.0f, -40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 3,14 };
	enemy3 = { vec3(40.0f, 40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 3,15 };
	enemy4 = { vec3(40.0f, -40.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 3,16 };
	spheres.emplace_back(enemy1);
	spheres.emplace_back(enemy2);
	spheres.emplace_back(enemy3);
	spheres.emplace_back(enemy4);

	//bottom enemy
	enemy1 = { vec3(-20.0f, 20.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 4,17 };
	enemy2 = { vec3(-20.0f, -20.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 4,18 };
	enemy3 = { vec3(20.0f, 20.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 4,19 };
	enemy4 = { vec3(20.0f, -20.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 4,20 };
	spheres.emplace_back(enemy1);
	spheres.emplace_back(enemy2);
	spheres.emplace_back(enemy3);
	spheres.emplace_back(enemy4);

	//back enemy
	enemy1 = { vec3(-20.0f, 0.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 5,21 };
	enemy2 = { vec3(0.0f, 20.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 5,22 };
	enemy3 = { vec3(0.0f, -20.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 5,23 };
	enemy4 = { vec3(20.0f, 0.0f, 100.0f), 3, vec4(1.0f,0.0f,0.0f,0.0f), 5,24 };
	spheres.emplace_back(enemy1);
	spheres.emplace_back(enemy2);
	spheres.emplace_back(enemy3);
	spheres.emplace_back(enemy4);

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
	make_tank_part(v, N + 0);
	//head
	make_tank_part(v, N + 8);
	//barrel
	make_tank_part(v, N + 16);
}

inline void tank::update() {
	model_matrix = mat4::rotate(vec3(1, 0, 0), planevec[plane].x) *  //rotation around sun
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 1, 0), planevec[plane].y) *
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 0, 1), planevec[plane].z) *
		mat4::translate(center.x, center.y, center.z) *
		mat4::rotate(vec3(0, 0, 1), dirvec[dir]) *
		mat4::scale(radius, radius, radius);
}

inline void player_activate(tank* player, int dir, bool activate) {
	if (activate) {
		player->dir = player->dirs[(dir + player->diroffset) % 4];
		player->movflag = true;
	}
	else {
		player->movflag = false;
	}
}

inline void move_plane(tank* player, int plane, vec3 movvec, int offset, int lrchange, int udchange) {
	if (player->movplane == false) {
		player->movplane = true;
		player->movflag = false;
	}
	else {
		player->movplane = false;
		player->movflag = false;
		player->plane = plane;
		player->center = movvec;
		player->diroffset = (player->diroffset + offset) % 4;
		player->dir = (player->dir + offset) % 4;
		if (lrchange == 1) { std::swap(player->dirs[0], player->dirs[2]); }
		if (udchange == 1) { std::swap(player->dirs[1], player->dirs[3]); }
	}
}

inline bool tank_wall_collision(tank* player, std::vector<wall> walls, char lrud) {
	if (lrud == 'l') {
		for (auto& w : walls) {
			if (player->plane != w.plane) continue;
			if (player->center.x - w.center.x <= 20.0f && player->center.x > w.center.x && abs(player->center.y - w.center.y) < 19.9f) {
				if (player->center.y - w.center.y > 18.0f) { player->center.y = ceil(player->center.y + 1); }
				else if (w.center.y - player->center.y > 18.0f) { player->center.y = floor(player->center.y - 1); }
				else {return true; }
			}
		}
	}
	else if (lrud == 'u') {
		for (auto& w : walls) {
			if (player->plane != w.plane) continue;
			if (w.center.y - player->center.y <= 20.0f && w.center.y > player->center.y && abs(player->center.x - w.center.x) < 19.9f) {
				if (player->center.x - w.center.x > 18.0f) { player->center.x = ceil(player->center.x + 1); }
				else if (w.center.x - player->center.x > 18.0f) { player->center.x = floor(player->center.x - 1); }
				else {return true; }
			}
		}
	}
	else if (lrud == 'r') {
		for (auto& w : walls) {
			if (player->plane != w.plane) continue;
			if (w.center.x - player->center.x <= 20.0f && player->center.x < w.center.x && abs(player->center.y - w.center.y) < 19.9f) {
				if (player->center.y - w.center.y > 18.0f) { player->center.y = ceil(player->center.y + 1); }
				else if (w.center.y - player->center.y > 18.0f) { player->center.y = floor(player->center.y - 1); }
				else {return true; }
			}
		}
	}
	else {
		for (auto& w : walls) {
			if (player->plane != w.plane) continue;
			if (player->center.y - w.center.y <= 20.0f && player->center.y > w.center.y && abs(player->center.x - w.center.x) < 19.9f) {
				if (player->center.x - w.center.x > 18.0f) { player->center.x = ceil(player->center.x + 1); }
				else if (w.center.x - player->center.x > 18.0f) { player->center.x = floor(player->center.x - 1); }
				else { return true; }
			}
		}
	}
	return false;
}

inline bool tank_tank_collision(tank* player, std::vector<tank> tanks, char lrud) {
	if (lrud == 'l') {
		for (auto& t : tanks) {
			if (!t.isenemy || player->plane != t.plane) continue;
			if (player->center.x - t.center.x <= 20.0f && player->center.x > t.center.x && abs(player->center.y - t.center.y) < 19.9f) {
				return true;
			}
		}
	}
	else if (lrud == 'u') {
		for (auto& t : tanks) {
			if (!t.isenemy || player->plane != t.plane) continue;
			if (t.center.y - player->center.y <= 20.0f && t.center.y > player->center.y && abs(player->center.x - t.center.x) < 19.9f) {
				return true;
			}
		}
	}
	else if (lrud == 'r') {
		for (auto& t : tanks) {
			if (!t.isenemy || player->plane != t.plane) continue;
			if (t.center.x - player->center.x <= 20.0f && player->center.x < t.center.x && abs(player->center.y - t.center.y) < 19.9f) {
				return true;
			}
		}
	}
	else {
		for (auto& t : tanks) {
			if (!t.isenemy) continue;
			if (player->plane != t.plane) continue;
			if (player->center.y - t.center.y <= 20.0f && player->center.y > t.center.y && abs(player->center.x - t.center.x) < 19.9f) {
				return true;
			}
		}
	}
	return false;
}

inline void player_move(tank* player, std::vector<wall> walls, std::vector<tank> tanks, float elapsedTime) {
	int dir = player->dir;
	if (dir == 0) {
		if (player->center.x <= -80) {
				if (player->plane == 0) { move_plane(player, 1, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);	return; }
				else if (player->plane == 1) { move_plane(player, 5, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);	return; }
				else if (player->plane == 2) { move_plane(player, 0, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);	return; }
				else if (player->plane == 3) { move_plane(player, 1, { -player->center.x, -player->center.y, player->center.z }, 3, 0, 0);	return; }
				else if (player->plane == 4) { move_plane(player, 1, { player->center.y, player->center.x, player->center.z }, 1, 0, 0);	return; }
				else { move_plane(player, 2, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);	return; }
		}
		else { player->movplane = false; }
		if (tank_wall_collision(player, walls, 'l') || tank_tank_collision(player, tanks, 'l')) return;
		player->center.x -= 0.1f * elapsedTime * 500;
	}
	else if (dir == 1) {
		if (player->center.y >= 80) {
			if (player->plane == 0) { move_plane(player, 3, { player->center.x, -player->center.y, player->center.z }, 0, 0, 0);	return; }
			else if (player->plane == 1) { move_plane(player, 3, { -player->center.y, -player->center.x, player->center.z }, 1, 0, 0);		return; }
			else if (player->plane == 2) { move_plane(player, 3, { player->center.y, player->center.x, player->center.z }, 3, 0, 0);	return; }
			else if (player->plane == 3) { move_plane(player, 5, { -player->center.x, player->center.y, player->center.z }, 2, 0, 0);	return; }
			else if (player->plane == 4) { move_plane(player, 0, { player->center.x, -player->center.y, player->center.z }, 0, 0, 0);	return; }
			else { move_plane(player, 3, { -player->center.x, player->center.y, player->center.z }, 2, 0, 0);	return; }
		}
		else { player->movplane = false; }
		if (tank_wall_collision(player, walls, 'u') || tank_tank_collision(player, tanks, 'u')) return;
		player->center.y += 0.1f * elapsedTime * 500;
	}
	else if (dir == 2) {
		if (player->center.x >= 80) {
			if (player->plane == 0) { move_plane(player, 2, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);	return; }
			else if (player->plane == 1) { move_plane(player, 0, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);		return; }
			else if (player->plane == 2) { move_plane(player, 5, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);	return; }
			else if (player->plane == 3) { move_plane(player, 2, { -player->center.x, -player->center.y, player->center.z }, 1, 0, 0);	return; }
			else if (player->plane == 4) { move_plane(player, 2, { -player->center.y, -player->center.x, player->center.z }, 3, 0, 0);	return; }
			else { move_plane(player, 1, { -player->center.x, player->center.y, player->center.z }, 0, 0, 0);	return; }
		}
		else { player->movplane = false; }
		if (tank_wall_collision(player, walls, 'r') || tank_tank_collision(player, tanks, 'r')) return;
		player->center.x += 0.1f * elapsedTime * 500;
	}
	else {
		if (player->center.y <= -80) {
			if (player->plane == 0) { move_plane(player, 4, { player->center.x, -player->center.y, player->center.z }, 0, 0, 0);	return; }
			else if (player->plane == 1) { move_plane(player, 4, { player->center.y, player->center.x, player->center.z }, 3, 0, 0);	return; }
			else if (player->plane == 2) { move_plane(player, 4, { -player->center.y, -player->center.x, player->center.z }, 1, 0, 0);	return; }
			else if (player->plane == 3) { move_plane(player, 0, { player->center.x, -player->center.y, player->center.z }, 0, 0, 0);	return; }
			else if (player->plane == 4) { move_plane(player, 5, { -player->center.x, player->center.y, player->center.z }, 2, 0, 0);	return; }
			else { move_plane(player, 4, { -player->center.x, player->center.y, player->center.z }, 2, 0, 0);	return; }
		}
		else { player->movplane = false; }
		if (tank_wall_collision(player, walls, 'd') || tank_tank_collision(player, tanks, 'd')) return;
		player->center.y -= 0.1f * elapsedTime * 500;
	}
}

inline void enemy_move(tank* player, tank* enemy, float hash, std::vector<wall> walls, std::vector<tank> tanks, float elapsedTime) {
	if (enemy->timestamp < 1.f*elapsedTime) {
		int dir = enemy->dir;
		if (dir == 0) {
			if (enemy->center.x <= -80) {
				if (enemy->plane == 0)		{ move_plane(enemy, 1, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 1)	{ move_plane(enemy, 5, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 2) { move_plane(enemy, 0, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 3) { move_plane(enemy, 2, { -enemy->center.x, -enemy->center.y, enemy->center.z }, 1, 0, 0);	return; }
				else if (enemy->plane == 4) { move_plane(enemy, 1, { enemy->center.y, enemy->center.x, enemy->center.z }, 1, 0, 0);		return; }
				else						{ move_plane(enemy, 2, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
			}
			for (auto& w : walls) {
				if (enemy->plane != w.plane) continue;
				if (enemy->center.x - w.center.x <= 20.0f && enemy->center.x > w.center.x && abs(enemy->center.y - w.center.y) < 19.9f) {
					if (enemy->center.y - w.center.y > 17.0f) {
						enemy->center.y = ceil(enemy->center.y);
					}
					else if (w.center.y - enemy->center.y > 17.0f) {
						enemy->center.y = floor(enemy->center.y);
					}
					else {
						enemy->timestamp += 0.1f * elapsedTime;  return;
					}
				}
			}
			for (auto& t : tanks) {
				if (enemy->tank_id == t.tank_id) continue;
				if (enemy->plane != t.plane) continue;
				if (enemy->center.x - t.center.x <= 20.0f && enemy->center.x > t.center.x && abs(enemy->center.y - t.center.y) < 19.9f) { 
					enemy->timestamp += 0.1f * elapsedTime;  return; }
			}
			enemy->center.x -= 0.1f * elapsedTime * 500;
		}
		else if (dir == 1) {
			if (enemy->center.y >= 80) { 
				if (enemy->plane == 0) { move_plane(enemy, 3, { enemy->center.x, -enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 1) { move_plane(enemy, 3, { -enemy->center.y, -enemy->center.x, enemy->center.z }, 1, 0, 0);		return; }
				else if (enemy->plane == 2) { move_plane(enemy, 3, { enemy->center.y, enemy->center.x, enemy->center.z }, 3, 0, 0);	return; }
				else if (enemy->plane == 3) { move_plane(enemy, 2, { -enemy->center.x, -enemy->center.y, enemy->center.z }, 1, 0, 0);	return; }
				else if (enemy->plane == 4) { move_plane(enemy, 0, { enemy->center.x, -enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else { move_plane(enemy, 3, { -enemy->center.x, enemy->center.y, enemy->center.z }, 2, 0, 0);	return; }
			}
			for (auto& w : walls) {
				if (enemy->plane != w.plane) continue;
				if (w.center.y - enemy->center.y <= 20.0f && w.center.y > enemy->center.y && abs(enemy->center.x - w.center.x) < 19.9f) {
					if (enemy->center.x - w.center.x > 17.0f) {
						enemy->center.x = ceil(enemy->center.x);
					}
					else if (w.center.x - enemy->center.x > 17.0f) {
						enemy->center.x = floor(enemy->center.x);
					}
					else {
						enemy->timestamp += 0.1f * elapsedTime;  return;
					}
				}
			}
			for (auto& t : tanks) {
				if (enemy->tank_id == t.tank_id) continue;
				if (enemy->plane != t.plane) continue;
				if (t.center.y - enemy->center.y <= 20.0f && t.center.y > enemy->center.y && abs(enemy->center.x - t.center.x) < 19.9f) { 
					enemy->timestamp += 0.1f * elapsedTime;  return; }
			}
			enemy->center.y += 0.1f * elapsedTime * 500;
		}
		else if (dir == 2) {
			if (enemy->center.x >= 80) {
				if (enemy->plane == 0) { move_plane(enemy, 2, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 1) { move_plane(enemy, 0, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);		return; }
				else if (enemy->plane == 2) { move_plane(enemy, 5, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 3) { move_plane(enemy, 2, { -enemy->center.x, -enemy->center.y, enemy->center.z }, 1, 0, 0);	return; }
				else if (enemy->plane == 4) { move_plane(enemy, 2, { -enemy->center.y, -enemy->center.x, enemy->center.z }, 3, 0, 0);	return; }
				else { move_plane(enemy, 1, { -enemy->center.x, enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
			}
			for (auto& w : walls) {
				if (enemy->plane != w.plane) continue;
				if (w.center.x - enemy->center.x <= 20.0f && enemy->center.x < w.center.x && abs(enemy->center.y - w.center.y) < 19.9f) {
					if (enemy->center.y - w.center.y > 17.0f) {
						enemy->center.y = ceil(enemy->center.y);
					}
					else if (w.center.y - enemy->center.y > 17.0f) {
						enemy->center.y = floor(enemy->center.y);
					}
					else {
						enemy->timestamp += 0.1f * elapsedTime;  return;
					}
				}
			}
			for (auto& t : tanks) {
				if (enemy->tank_id == t.tank_id) continue;
				if (enemy->plane != t.plane) continue;
				if (t.center.x - enemy->center.x <= 20.0f && enemy->center.x < t.center.x && abs(enemy->center.y - t.center.y) < 19.9f) { 
					enemy->timestamp += 0.1f * elapsedTime;  return; }
			}
			enemy->center.x += 0.1f * elapsedTime * 500;
		}
		else {
			if (enemy->center.y <= -80) {
				if (enemy->plane == 0) { move_plane(enemy, 4, { enemy->center.x, -enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 1) { move_plane(enemy, 4, { enemy->center.y, enemy->center.x, enemy->center.z }, 3, 0, 0);	return; }
				else if (enemy->plane == 2) { move_plane(enemy, 4, { -enemy->center.y, -enemy->center.x, enemy->center.z }, 1, 0, 0);	return; }
				else if (enemy->plane == 3) { move_plane(enemy, 0, { enemy->center.x, -enemy->center.y, enemy->center.z }, 0, 0, 0);	return; }
				else if (enemy->plane == 4) { move_plane(enemy, 5, { -enemy->center.x, enemy->center.y, enemy->center.z }, 2, 0, 0);	return; }
				else { move_plane(enemy, 4, { -enemy->center.x, enemy->center.y, enemy->center.z }, 2, 0, 0);	return; }
			}
			for (auto& w : walls) {
				if (enemy->plane != w.plane) continue;
				if (enemy->center.y - w.center.y <= 20.0f && enemy->center.y > w.center.y && abs(enemy->center.x - w.center.x) < 19.9f) {
					if (enemy->center.x - w.center.x > 17.0f) {
						enemy->center.x = ceil(enemy->center.x);
					}
					else if (w.center.x - enemy->center.x > 17.0f) {
						enemy->center.x = floor(enemy->center.x);
					}
					else {
						enemy->timestamp += 0.1f * elapsedTime;  return;
					}
				}
			}
			for (auto& t : tanks) {
				if (enemy->tank_id == t.tank_id) continue;
				if (enemy->plane != t.plane) continue;
				if (enemy->center.y - t.center.y <= 20.0f && enemy->center.y > t.center.y && abs(enemy->center.x - t.center.x) < 19.9f) { 
					enemy->timestamp += 0.1f * elapsedTime;  return; }
			}
			enemy->center.y -= 0.1f * elapsedTime * 500;
		}
		enemy->timestamp += 0.002f * elapsedTime;
	}
	else {
		enemy->timestamp = 0.0f;
		srand((unsigned int)time(0));
		player_activate(enemy, (rand() * (unsigned int)hash) % 4, true);
	}
}

#endif