#pragma once
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "cgmath.h"
#include "cgut.h"
#include "bullet.h"
#include "tank.h"

void physics_reflect_bullet(std::vector<bullet>& bullets, std::vector<mirror>& mirrors) {
	for (auto& b : bullets) {
		for (auto& mir : mirrors) {
			if (abs(mir.center.x - b.center.x) + abs(mir.center.y - b.center.y) <= 2.0f && mir.plane == b.plane) {
				if (b.dir == 0) {
					b.dir = 3;
				}
				else if (b.dir == 1) {
					b.dir = 2;
				}
				else if (b.dir == 2) {
					b.dir = 1;
				}
				else {
					b.dir = 0;
				}
			}
		}
	}
}

void physics_push_tank(tank& t, bullet& b) {
	if (b.dir == 0) {
		t.center.x -= 5.f;
	}
	else if (b.dir == 1) {
		t.center.y += 5.f;
	}
	else if (b.dir == 2) {
		t.center.x += 5.f;
	}
	else {
		t.center.y -= 5.f;
	}
}

void physics_refract_bullet(bullet& b, float elapsedTime) {
	b.planevec[b.plane][0] > 0 ? b.planevec[b.plane][0] += elapsedTime*0.5f: b.planevec[b.plane][0] -= elapsedTime*0.5f;
	b.planevec[b.plane][1] > 0 ? b.planevec[b.plane][1] += elapsedTime*0.5f: b.planevec[b.plane][1] -= elapsedTime*0.5f;
	b.planevec[b.plane][2] > 0 ? b.planevec[b.plane][2] += elapsedTime*0.5f: b.planevec[b.plane][2] -= elapsedTime*0.5f;
}

#endif