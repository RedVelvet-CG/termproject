#ifndef __MIRROR_H__
#define __MIRROR_H__

#include "cgmath.h"
#include "cgut.h"

struct mirror {
	vec3	center = vec3(0);		// 2D position for translation
	int		plane = 0; //0: front	1: left		2: right	3: top	4: bottom	5:back
	int		dir = 0; //0: left	1: up	2: right	3: back
	vec3	planevec[6] = { {0,0,0}, {0, -PI / 2, 0}, {0, PI / 2, 0}, {-PI / 2, 0, 0}, {PI / 2, 0, 0}, {0, PI, 0} };
	float	dirvec[4] = { 0, -PI / 2, PI, PI / 2 };
	float	theta = 0.0f; //rotation angle
	float	radius = 10.0f;		// radius
	vec4	color;				// RGBA color in [0,1]
	mat4	model_matrix;		// modeling transformation
	int		creation_val = 3 * 2 * 6;

	inline void update();
};

void create_mirror_vertices(std::vector<vertex>& v) {
	//front
	v.push_back({ vec3(1.f,1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 1.f) });
	v.push_back({ vec3(-1.f,1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 1.f) });
	v.push_back({ vec3(-1.f,-1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(1.f,-1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });

	//left
	v.push_back({ vec3(-1.f,1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.75f) });
	v.push_back({ vec3(-1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(-1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });
	v.push_back({ vec3(-1.f,-1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.75f) });

	//right
	v.push_back({ vec3(1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(1.f,1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.75f) });
	v.push_back({ vec3(1.f,-1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.75f) });
	v.push_back({ vec3(1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });

	//top
	v.push_back({ vec3(1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(-1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });
	v.push_back({ vec3(-1.f,1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.75f) });
	v.push_back({ vec3(1.f,1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.75f) });

	//bottom
	v.push_back({ vec3(1.f,-1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.75f) });
	v.push_back({ vec3(-1.f,-1.f,1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.75f) });
	v.push_back({ vec3(-1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });

	//back
	v.push_back({ vec3(-1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 0.f) });
	v.push_back({ vec3(1.f,1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 0.f) });
	v.push_back({ vec3(1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(1.f, 1.f) });
	v.push_back({ vec3(-1.f,-1.f,-1.0f), vec3(0.f,0.f,0.0f), vec2(0.f, 1.f) });

}

inline void create_front_mirror(std::vector<mirror>& mirrors, int plane = 0) {
	mirror w;
	w = { vec3(-60.0f, 60.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 40.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 20.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -20.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -40.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -60.0f, 100.0f), plane };
	mirrors.emplace_back(w);

	w = { vec3(-20.0f, 60.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, 20.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, 0.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -20.0f, 100.0f), plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(0.0f, 60.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, -60.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, 0.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 40.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -40.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
}

inline void create_left_mirror(std::vector<mirror>& mirrors, bool life, int plane = 1) {
	mirror w;
	w = { vec3(-60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-40.0f, 20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(40.0f, 20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
}

inline void create_right_mirror(std::vector<mirror>& mirrors, bool life, int plane = 2) {
	mirror w;
	w = { vec3(-60.0f, 0.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-40.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-20.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, 20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(0.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, 0.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(40.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 0.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
}

inline void create_top_mirror(std::vector<mirror>& mirrors, bool life, int plane = 3) {
	mirror w;
	w = { vec3(-60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-40.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(0.0f, 60.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, -60.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(40.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
}

inline void create_bottom_mirror(std::vector<mirror>& mirrors, bool life, int plane = 4) {
	mirror w;
	w = { vec3(-60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-40.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, 0.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-20.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(0.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(20.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(40.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, 0.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
}

inline void create_back_mirror(std::vector<mirror>& mirrors, bool life, int plane = 5) {
	mirror w;
	w = { vec3(-60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, 0.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-40.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, 40.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -40.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-40.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(-20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, 20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(-20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(0.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, 0.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(0.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(20.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, 20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -20.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(20.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(40.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, 40.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -40.0f, 100.0f),false,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(40.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);

	w = { vec3(60.0f, 60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, 0.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -20.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -40.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
	w = { vec3(60.0f, -60.0f, 100.0f),true,false,false, plane };
	mirrors.emplace_back(w);
}

inline std::vector<mirror> create_mirror() {
	std::vector<mirror> mirrors;
	//front
	create_front_mirror(mirrors, true);
	//left
	create_left_mirror(mirrors, false);
	//right
	create_right_mirror(mirrors, false);
	//top
	create_top_mirror(mirrors, false);
	//bottom
	create_bottom_mirror(mirrors, false);
	//back
	create_back_mirror(mirrors, false);
	return mirrors;
}

inline void make_mirror_part(std::vector<uint>& v, int offset) {
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

inline void make_mirror_indices(std::vector<uint>& v, uint N) {
	//mirror
	make_mirror_part(v, N + 0);
}

inline void mirror::update() {
	model_matrix = mat4::rotate(vec3(1, 0, 0), planevec[plane].x) *  //rotation around sun
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 1, 0), planevec[plane].y) *
		mat4::translate(0, 0, 0) *
		mat4::rotate(vec3(0, 0, 1), planevec[plane].z) *
		mat4::translate(center.x, center.y, center.z) *
		mat4::rotate(vec3(0, 0, 1), dirvec[dir]) *
		mat4::scale(radius, radius, radius);
}

#endif