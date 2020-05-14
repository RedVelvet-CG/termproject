#pragma once
#ifndef __OBJECT_H
#define __OBJECT_H__

#include "cgmath.h"
#include "cgut.h"

struct sphere {
	vec3	center = vec3(0);		// 2D position for translation
	float	radius = 10.0f;		// radius
	vec3	movval = vec3(0);
	float	theta = 0.0f; //rotation angle
	vec4	color;				// RGBA color in [0,1]
	//mat4	model_matrix;		// modeling transformation
	int		creation_val = 50 * 3 * 50 * 2;

};

void create_circle_vertices(std::vector<vertex>& v, uint N, float xval, float yval, float zval, float diameter) {
	for (uint stack = 0; stack <= N; stack++) {
		float t2 = PI / float(N) * stack, c2 = cos(t2), s2 = sin(t2);
		for (uint k = 0; k <= N; k++) {
			float t = 2.0f * PI / float(N) * k, c = cos(t), s = sin(t);
			v.push_back({ vec3(c * s2 * diameter + xval,s * s2 * diameter + yval,c2 * diameter + zval), vec3(0.f,0.f,0.0f), vec2(t / (2 * PI), 1 - t2 / PI) });
		}
	}
}

inline std::vector<sphere> create_universe() {
	std::vector<sphere> spheres;
	//solar system planet size reference: http://study.zum.com/book/12324
	//solar system rotation reference: http://javalab.org/lee/2015/scale_of_solar_system/
	//solar system self-rotation reference: http://dl.dongascience.com/magazine/view/S198802N024
	sphere sun = { vec3(0.f, 0.f,0.f),10.0f, vec3(0.0f, 0.0f, 0.0f), 0.5f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere mercury = { vec3(0.f, 0.0f,0.f),0.9f, vec3(13.5f, 6.f, 0.0f), 0.59f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere venus = { vec3(0.f, 0.f,0.f),1.4f, vec3(18.5f, 4.5f, 0.0f), 0.243f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere earth = { vec3(0.f, 0.f,0.f),1.5f, vec3(22.5f, 2.5f, 0.0f), 1.0f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere mars = { vec3(0.f, 0.f,0.f),1.0f, vec3(28.5f, 1.8f, 0.0f), 1.1f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere jupitor = { vec3(0.f, 0.f,0.f),5.6f, vec3(40.f, 0.5f, 0.0f), 2.0f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere saturn = { vec3(0.f, 0.f,0.f),4.7f, vec3(58.5f, 0.25f, 0.0f), 2.1f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere uranus = { vec3(0.f, 0.f,0.f),2.f, vec3(80.f, 0.1f, 0.0f), 1.3f, vec4(0.0f,0.0f,0.0f,0.0f) };
	sphere neptune = { vec3(0.f, 0.f,0.f),1.9f, vec3(105.f, 0.08f, 0.0f), 3.0f, vec4(0.0f,0.0f,0.0f,0.0f) };
	spheres.emplace_back(sun);
	spheres.emplace_back(mercury);
	spheres.emplace_back(venus);
	spheres.emplace_back(earth);
	spheres.emplace_back(mars);
	spheres.emplace_back(jupitor);
	spheres.emplace_back(saturn);
	spheres.emplace_back(uranus);
	spheres.emplace_back(neptune);
	return spheres;
}

std::vector<uint> makeindices(uint N) {
	std::vector<uint> indices;
	for (uint k = 0; k < N; k++) {
		for (uint kk = 0; kk < N; kk++) {
			indices.push_back((k - 1) * (N + 1) + kk + 1);
			indices.push_back((k - 1) * (N + 1) + kk);
			indices.push_back(k * (N + 1) + kk + 1);
			indices.push_back(k * (N + 1) + kk);
			indices.push_back(k * (N + 1) + kk + 1);
			indices.push_back((k - 1) * (N + 1) + kk);
		}
	}
	return indices;
}

#endif