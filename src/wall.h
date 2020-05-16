#pragma once
#ifndef __WALL_H__
#define __wALL_H__

#include "cgmath.h"
#include "cgut.h"

struct wall {
	vec3	center = vec3(0);		// 2D position for translation
	bool breakable = true;
	bool broken = false;
	bool is_base = false;
	float	radius = 10.0f;		// radius
	vec3	movval = vec3(0);
	float	theta = 0.0f; //rotation angle
	vec4	color;				// RGBA color in [0,1]
	mat4	model_matrix;		// modeling transformation
	int		creation_val = 3 * 2 * 6;

	inline void update();
};

void create_wall_vertices(std::vector<vertex>& v) {
	//body

	//front
	v.push_back({ vec3(1.f,1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(-1.f,1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });
	v.push_back({ vec3(-1.f,-1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(1.f, 1.f) });
	v.push_back({ vec3(1.f,-1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(0.f, 1.f) });

	//left
	v.push_back({ vec3(-1.f,1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.5f) });
	v.push_back({ vec3(-1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(-1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });
	v.push_back({ vec3(-1.f,-1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.5f) });

	//right
	v.push_back({ vec3(1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(1.f,1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.5f) });
	v.push_back({ vec3(1.f,-1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.5f) });
	v.push_back({ vec3(1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });

	//top
	v.push_back({ vec3(1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(-1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });
	v.push_back({ vec3(-1.f,1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.5f) });
	v.push_back({ vec3(1.f,1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.5f) });

	//bottom
	v.push_back({ vec3(1.f,-1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.5f) });
	v.push_back({ vec3(-1.f,-1.f,0.5f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.5f) });
	v.push_back({ vec3(-1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });

	//back
	v.push_back({ vec3(-1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });
	v.push_back({ vec3(1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 1.f) });
	v.push_back({ vec3(-1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 1.f) });

}

inline std::vector<wall> create_wall() {
	std::vector<wall> walls;
	wall w;
	w = { vec3(20.0f, 20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(20.0f, 40.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(20.0f, 60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(60.0f, 20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(60.0f, 40.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(60.0f, 60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(0.f, 40.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(-20.0f, 20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-20.0f, 40.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-20.0f, 60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-60.0f, 20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-60.0f, 40.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(-60.0f, 60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-60.0f, -20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-20.0f, -20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(0.f, -20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(20.0f, -20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(60.0f, -20.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-60.0f, -40.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(60.0f, -40.0f, 1.0f),false,false,false };
	walls.emplace_back(w);
	w = { vec3(-60.0f, -60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-20.0f, -60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(0.f, -60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(20.0f, -60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(60.0f, -60.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(20.0f, -80.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	w = { vec3(-20.0f, -80.0f, 1.0f),true,false,false };
	walls.emplace_back(w);
	//w = { vec3(0.0f, -8.0f, 1.0f),true,false,true };
	//walls.emplace_back(w);
	return walls;
}

inline void make_wall_part(std::vector<uint>& v, int offset) {
	//front
	v.push_back(offset + 0);	v.push_back(offset + 1);	v.push_back(offset + 2);
	v.push_back(offset + 2);	v.push_back(offset + 3);	v.push_back(offset + 0);

	//left
	v.push_back(offset + 4);	v.push_back(offset + 5);	v.push_back(offset + 6);
	v.push_back(offset + 6);	v.push_back(offset + 7);	v.push_back(offset + 4);

	//right
	v.push_back(offset + 8);	v.push_back(offset + 9);	v.push_back(offset + 10);
	v.push_back(offset + 10);	v.push_back(offset + 11);	v.push_back(offset + 8);

	//top
	v.push_back(offset + 12);	v.push_back(offset + 13);	v.push_back(offset + 14);
	v.push_back(offset + 14);	v.push_back(offset + 15);	v.push_back(offset + 12);

	//bottom
	v.push_back(offset + 16);	v.push_back(offset + 17);	v.push_back(offset + 18);
	v.push_back(offset + 18);	v.push_back(offset + 19);	v.push_back(offset + 16);

	//back
	v.push_back(offset + 20);	v.push_back(offset + 21);	v.push_back(offset + 22);
	v.push_back(offset + 22);	v.push_back(offset + 23);	v.push_back(offset + 20);
}

inline void make_wall_indices(std::vector<uint>& v, uint N) {
	//wall
	make_wall_part(v, N + 0);
}

inline void wall::update() {
	model_matrix = mat4::rotate(vec3(0, 1, 0), 0) *  //rotation around sun
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 1, 0), 0) * //self-rotation
		mat4::translate(center.x, center.y, center.z) *
		mat4::rotate(vec3(0, 0, 1), 0) *
		mat4::scale(radius, radius, radius);
}

#endif