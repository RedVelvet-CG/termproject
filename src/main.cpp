#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#define STB_IMAGE_IMPLEMENTATION
#include "trackball.h"	// virtual trackball
#include"field.h"
#include "tank.h"
#include "wall.h"
#include "stb_image.h"
#include "bullet.h"
#include "mirror.h"
#include "physics.h"
#include <cstdlib>
#include <ctime>
#include "irrKlang\irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

//*******************************************************************
// forward declarations for freetype text
bool init_text();
void render_text( std::string text, GLint x, GLint y, GLfloat scale, vec4 color, GLfloat dpi_scale=1.0f );

// global constants
static const char* window_name = "RedVelvetHorde - Long live the KINGO";
static const char* vert_shader_path = "../bin/shaders/trackball.vert";
static const char* frag_shader_path = "../bin/shaders/trackball.frag";
static const char* brick_path = "../bin/images/brick.jpg";
static const char* iron_path = "../bin/images/iron.jpg";
static const char* skku_path = "../bin/images/skku.jpg";
static const char* intro_path = "../bin/images/tank_intro.jpg";
static const char* win_path = "../bin/images/win.jpg";
static const char* lose_path = "../bin/images/game_over.jpg";
static const char* fire_sound_path = "../bin/sounds/tank_fire.wav";
static const char* base_attack_sound_path = "../bin/sounds/Base_under_attack.mp3";
static const char* base_explode_sound_path = "../bin/sounds/base_explode.wav";
static const char* tank_explode_sound_path = "../bin/sounds/tank_explode.wav";
static const char* bgm_path = "../bin/sounds/bgm.mp3";
static const char* winbgm_path = "../bin/sounds/skkusong.mp3";
static const char* losebgm_path = "../bin/sounds/Game-over-yeah.mp3";
uint				NUM_TESS = 50;		// initial tessellation factor of the circle as a polygon

// common structures
struct camera {
	vec3	eye = vec3(0, 30, 400), at = vec3(0, 0, 0), up = vec3(0, 1, 0);
	mat4	view_matrix = mat4::look_at(eye, at, up);
	float	fovy = PI / 4.0f; // must be in radian
	float	aspect = 0.f;
	float	dnear = 1.0f;
	float	dfar = 1000.0f;
	mat4	projection_matrix = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
};

// window objects
GLFWwindow* window = nullptr;
ivec2		window_size = ivec2(1280, 720);// cg_default_window_size(); // initial window size
bool		fullscreen_flag = false;

// OpenGL objects
GLuint	program = 0;	// ID holder for GPU program
GLuint	vertex_array = 0;
GLuint  vertex_array_intro = 0;
GLuint	brick = 0;
GLuint	iron = 0;
GLuint	skku = 0;
GLuint	intro = 0;
GLuint	youwin = 0;
GLuint  youlose = 0;

//*******************************************************************
// irrKlang objects
irrklang::ISoundEngine* engine;
irrklang::ISoundSource* fire_sound = nullptr;
irrklang::ISoundSource* tank_death = nullptr;
irrklang::ISoundSource* base_explode_sound = nullptr;
irrklang::ISoundSource* base_attack_sound = nullptr;
irrklang::ISoundSource* bgm = nullptr;
irrklang::ISoundSource* winbgm = nullptr;
irrklang::ISoundSource* losebgm = nullptr;

// global variables
int		frame = 0;				// index of rendering frames
float	t = 0.0f;
float	start_time = 0.f;
auto	fields = create_field();
auto	tanks = create_tank();
auto	walls = create_wall();
auto	mirrors = create_mirror();
int		zoom = 0;
int		pan = 0;
int		zoomval = 50;
int		panval = 30;
bool	b_wireframe = false;
int		base_health = 6;
int		game_mode = 0;			// 0 = intro, 1 = main game, 2 = ending(win), 3 = game over
bool	lose_flag = false;
double	lose_time = 0;

std::vector<bullet> bullets;
std::vector<int>	del_bullets;
std::vector<int>	del_walls;
std::vector<int>	del_tanks;

// scene objects
camera		cam;
trackball	tb;

// holder of vertices and indices of a unit circle
std::vector<vertex> unit_field_vertices; // host-side vertices for field
std::vector<vertex>	unit_tank_vertices;	// host-side vertices for tank
std::vector<vertex>	unit_wall_vertices;	// host-side vertices for wall
std::vector<vertex> unit_combine_vertices; //to draw one set of vertices
std::vector<vertex> unit_bullet_vertices;
std::vector<vertex> unit_mirror_vertices;

//game variables
tank* player = &tanks[0];
int		movdir = 1;
float	gametick = 0.0f;


void update(float elapsedTime) {
	if (game_mode == 1) {
		cam.aspect = window_size.x / float(window_size.y);
		cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect, cam.dnear, cam.dfar);
		// build the model matrix for oscillating scale
		float t = float(glfwGetTime());
		GLint uloc;
		uloc = glGetUniformLocation(program, "view_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
		uloc = glGetUniformLocation(program, "projection_matrix");	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);
	}
	else {
		glUseProgram(program);
	}
}

void render_text_part() {
	// render texts
	render_text("Dashboard", 50, 50, 0.8f, vec4(0.5f, 0.8f, 0.2f, 1.0f), 1.0f);
	std::string enemy_string = "Enemy left: ";
	std::string enemy_value = std::to_string(player->health > 0 ? tanks.size() - 1 : tanks.size());
	for (int i = 0; i < (int)enemy_value.length(); i++) {
		enemy_string.push_back(enemy_value[i]);
	}
	render_text(enemy_string, 50, 75, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);

	std::string your_health_string = "Your health: ";
	std::string your_health_value = std::to_string(player->health);
	if (player->tank_id == 0) {
		for (int i = 0; i < (int)your_health_value.length(); i++) {
			your_health_string.push_back(your_health_value[i]);
		}
	}
	else {
		your_health_string.push_back('0');
	}
	render_text(your_health_string, 50, 100, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);

	std::string base_health_string = "Base health: ";
	std::string base_health_value = std::to_string(base_health);
	for (int i = 0; i < (int)base_health_value.length(); i++) {
		base_health_string.push_back(base_health_value[i]);
	}
	render_text(base_health_string, 50, 125, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);

	render_text("Instructions", 50, 500, 0.8f, vec4(0.5f, 0.8f, 0.2f, 1.0f), 1.0f);
	render_text("Move with arrow keys", 50, 525, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);
	render_text("Press 's' to stop", 50, 550, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);
	render_text("Press 'a' to attack", 50, 575, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);
	render_text("Press 'r' to reset", 50, 600, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);
	render_text("mouse l: move view", 50, 625, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);
	render_text("Ctrl -> mouse r: zoom", 50, 650, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);
	render_text("Save SKKU and survive", 50, 675, 0.5f, vec4(0.7f, 0.4f, 0.1f, 0.8f), 1.0f);
}

void render_field() {
	for (auto& f : fields) {
		f.update();
		glUniform1i(glGetUniformLocation(program, "mode"), 4);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, f.model_matrix);
		glDrawElements(GL_TRIANGLES, f.creation_val, GL_UNSIGNED_INT, (void*)(f.creation_val * 0 * sizeof(GLuint)));
	}
}

void render_tank(float elapsedTime) {
	for (auto& t : tanks) {
		t.update();
		if (t.isenemy) {
			enemy_move(player, &t, (float)glfwGetTime() * 10000, walls, tanks, elapsedTime);
			//player_move(&t, walls, tanks, elapsedTime);
			float time_now = (float)glfwGetTime() - start_time;
			if (time_now - t.bulletstamp > 1.0f) {
				t.bulletstamp = time_now;
				int shot_fire_check = rand() % 2;
				if (shot_fire_check == 1) bullets = create_bullet(bullets, t);
			}
		}
		else if (t.movflag) player_move(&t, walls, tanks, elapsedTime);
		if (t.isenemy == false) {
			glUniform1i(glGetUniformLocation(program, "mode"), 2);
		}
		else {
			glUniform1i(glGetUniformLocation(program, "mode"), 4);
		}
		GLint uloc;
		uloc = glGetUniformLocation(program, "color"); if (uloc > -1) glUniform4fv(uloc, 1, t.color);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, t.model_matrix);
		glDrawElements(GL_TRIANGLES, t.creation_val, GL_UNSIGNED_INT, (void*)(fields[0].creation_val * 1 * sizeof(GLuint)));
	}
}

void render_wall() {
	for (auto& w : walls) {
		w.update();
		if (!w.breakable) {
			glUniform1i(glGetUniformLocation(program, "mode"), 1);
		}
		else if (w.broken) {

		}
		else if (w.is_base) {
			glUniform1i(glGetUniformLocation(program, "mode"), 2);
		}
		else {
			glUniform1i(glGetUniformLocation(program, "mode"), 0);
		}
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, w.model_matrix);
		glDrawElements(GL_TRIANGLES, w.creation_val, GL_UNSIGNED_INT, (void*)((fields[0].creation_val + tanks[0].creation_val) * sizeof(GLuint)));
	}
}

void render_mirror() {
	for (auto& mir : mirrors) {
		mir.update();
		glUniform1i(glGetUniformLocation(program, "mode"), 1);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, mir.model_matrix);
		glDrawElements(GL_TRIANGLES, mir.creation_val, GL_UNSIGNED_INT, (void*)((fields[0].creation_val + tanks[0].creation_val + walls[0].creation_val) * sizeof(GLuint)));
	}
}


void render_bullet(float elapsedTime) {
	int del_bullet_checker = 0;
	for (auto& b : bullets) {
		bool bullet_break_checker = false;
		int del_wall_checker = 0;
		for (auto& w : walls) {
			if (abs(w.center.x - b.center.x) + abs(w.center.y - b.center.y) <= 10.0f && w.plane == b.plane) {
				bullet_break_checker = true;
				if (w.breakable && !w.is_base) del_walls.push_back(del_wall_checker);
				else if (w.is_base) {
					base_health--;
					if (base_health > 0) {
						if (!(engine->isCurrentlyPlaying(base_attack_sound))) engine->play2D(base_attack_sound, false);
					}
					else {
						del_walls.push_back(del_wall_checker);
						engine->stopAllSounds();
						engine->play2D(base_explode_sound, false);
						lose_time = glfwGetTime();
						lose_flag = true;
					}
				}
			}
			del_wall_checker++;
		}

		int del_tank_checker = 0;
		for (auto& t : tanks) {
			if ((!t.isenemy && b.is_mine) || (t.isenemy && !b.is_mine)) {
				del_tank_checker++;
				continue;
			}
			if (abs(t.center.x - b.center.x) + abs(t.center.y - b.center.y) <= 10.0f && t.plane == b.plane)
			{
				physics_push_tank(t, b);
				bullet_break_checker = true;
				if ((t.isenemy && b.is_mine) || (!t.isenemy && !b.is_mine))
				{
					t.health--;
					if (t.health == 0) del_tanks.push_back(del_tank_checker);
					if (!t.isenemy && t.health == 0) {
						lose_flag = true;
						lose_time = glfwGetTime();
						engine->stopAllSounds();
						engine->play2D(tank_death, false);
					}
				}
			}
			del_tank_checker++;
		}

		if (abs(b.center.x*b.center.x) + abs(b.center.y*b.center.y) >= 40000) {
			physics_refract_bullet(b, elapsedTime);
		}

		if (abs(b.center.x*b.center.x) + abs(b.center.y*b.center.y) >= 90000) {
			bullet_break_checker = true;
		}

		del_bullet_checker++;
		if (bullet_break_checker) {
			del_bullets.push_back(del_bullet_checker);
		}
		b.update(elapsedTime);
		glUniform1i(glGetUniformLocation(program, "mode"), 2);
		GLint uloc;
		uloc = glGetUniformLocation(program, "color"); if (uloc > -1) glUniform4fv(uloc, 1, b.color);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, b.model_matrix);
		glDrawElements(GL_TRIANGLES, b.creation_val, GL_UNSIGNED_INT, (void*)((fields[0].creation_val + tanks[0].creation_val + walls[0].creation_val + mirrors[0].creation_val) * sizeof(GLuint)));
	}
}

void delete_bullet() {
	int i = 1;
	for (auto& db : del_bullets) {
		bullets.erase(bullets.begin() + db - i);
		i++;
	}
	del_bullets.clear();
}

void delete_wall() {
	int i = 0;
	for (auto& dw : del_walls){
		walls.erase(walls.begin() + dw - i);
		i++;
	}
	del_walls.clear();
}

void delete_tank() {
	int i = 0;
	for (auto& dt : del_tanks){
		tanks.erase(tanks.begin() + dt - i);
		i++;
	}
	del_tanks.clear();
}

void render(float elapsedTime) {
	if (tanks.size() == 1) {
		game_mode = 2;
		if (engine->isCurrentlyPlaying(bgm)) engine->stopAllSounds();
	}
	if (game_mode == 1) {
		glUniform1i(glGetUniformLocation(program, "intro"), 1);
		glUniform1i(glGetUniformLocation(program, "game_mode"), game_mode);
		srand((unsigned int)time(0));

		// clear screen (with background color) and clear depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_text_part();

		glUseProgram(program); // notify GL that we use our own program
		glBindVertexArray(vertex_array); // bind vertex array object	

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brick);
		glUniform1i(glGetUniformLocation(program, "TEX0"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, iron);
		glUniform1i(glGetUniformLocation(program, "TEX1"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, skku);
		glUniform1i(glGetUniformLocation(program, "TEX2"), 2);

		render_field();
		render_tank(elapsedTime);
		render_wall();
		render_mirror();
		render_bullet(elapsedTime);

		physics_reflect_bullet(bullets, mirrors);

		delete_bullet();
		delete_wall();
		delete_tank();

		// swap front and back buffers, and display to screen
		glfwSwapBuffers(window);
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind program
		glUseProgram(program);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, intro);
		glUniform1i(glGetUniformLocation(program, "TEX3"), 3);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, youwin);
		glUniform1i(glGetUniformLocation(program, "TEX4"),4);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, youlose);
		glUniform1i(glGetUniformLocation(program, "TEX5"), 5);

		glUniform1i(glGetUniformLocation(program, "intro"), 0);
		glUniform1i(glGetUniformLocation(program, "game_mode"), game_mode);

		if (game_mode == 2) {
			if (!engine->isCurrentlyPlaying(winbgm)) engine->play2D(winbgm, true);
		}
		else if (game_mode == 3) {
			if (!engine->isCurrentlyPlaying(losebgm)) engine->play2D(losebgm, false);
		}
	
		glBindVertexArray(vertex_array_intro);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
	}
}

void reshape(GLFWwindow* window, int width, int height) {
	window_size = ivec2(width, height); // set current viewport in pixels (win_x, win_y, win_width, win_height)
	glViewport(0, 0, width, height); // viewport: the window area that are affected by rendering 
}

void print_help() {
	printf("\n[help]\n");
	printf("- press ESC or 'q' to terminate the program\n");
	printf("- press F1 or 'h' to see help\n");
	printf("- press Home to reset camera\n");
	printf("- press Control key and right buton of mouse to zoom\n");
	printf("- press Shift key and middle button of mouse to pan\n");
	printf("- press w to toggle from wireframe mode and solid mode\n");
	printf("*******************Game features*******************\n");
	printf("- press [ or ] key to adjust zoom speed\n");
	printf("- press < or > key t adjust pan speed\n");
	printf("- press 's' to stay still\n");
	printf("- press 'a' to fire\n");
	printf("- press 'f' to toggle fullscreen mode\n");
	printf("- press 'r' to reset game\n");

}


GLuint create_texture(const char* image_path, bool b_mipmap)
{
	// load the image with vertical flipping
	image* img = cg_load_image(image_path); if (!img) return -1;
	int w = img->width, h = img->height;

	// create a src texture (lena texture)
	GLuint texture; glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->ptr);
	if (img) delete img; // release image

	// build mipmap
	if (b_mipmap && glGenerateMipmap)
	{
		int mip_levels = 0; for (int k = max(w, h); k; k >>= 1) mip_levels++;
		for (int l = 1; l < mip_levels; l++)
			glTexImage2D(GL_TEXTURE_2D, l, GL_RGB8, max(1, w >> l), max(1, h >> l), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// set up texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, b_mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	return texture;
}


void update_vertex_buffer(const std::vector<vertex>& vertices, uint N) {
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer
	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; } // check exceptions
	std::vector<uint> indices;

	//make_field_indices(indices, N); // create buffers
	make_field_indices(indices, 0);
	make_tank_indices(indices, 8); // create buffers
	make_wall_indices(indices, 32);
	make_mirror_indices(indices, 56);
	make_bullet_indices(indices, 80);

	// generation of vertex buffer: use vertices as it is
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// geneation of index buffer
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);
	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (vertex_array) glDeleteVertexArrays(1, &vertex_array);
	vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }

	brick = create_texture(brick_path, true);		if (brick == -1) printf("brick image not loaded!\n");
	iron = create_texture(iron_path, true);			if (iron == -1) printf("iron image not loaded!\n");
	skku = create_texture(skku_path, true);			if (skku == -1) printf("skku image not loaded!\n");
}

void user_reset();

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)	glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key == GLFW_KEY_H || key == GLFW_KEY_F1)	print_help();
		else if (key == GLFW_KEY_HOME)					cam = camera();
		else if (key == GLFW_KEY_LEFT_SHIFT) {
			if (zoom == 1) {
				printf("disabled zoom!\n");
			}
			pan = 1 - pan; zoom = 0;
			printf("%s\n", pan == 1 ? "enabled pan!" : "disabled pan!");
		}
		else if (key == GLFW_KEY_LEFT_CONTROL) {
			if (pan == 1) {
				printf("disabled pan!\n");
			}
			zoom = 1 - zoom;	pan = 0;
			printf("%s\n", zoom == 1 ? "enabled zoom!" : "disabled zoom!");
		}
		else if (key == GLFW_KEY_LEFT) {
			if (game_mode == 1) {
				player_activate(player, 0, true);
			}			
		}
		else if (key == GLFW_KEY_UP) {
			if (game_mode == 1) {
				player_activate(player, 1, true);
			}
		}
		else if (key == GLFW_KEY_RIGHT) {
			if (game_mode == 1) {
				player_activate(player, 2, true);
			}
		}
		else if (key == GLFW_KEY_DOWN) {
			if (game_mode == 1) {
				player_activate(player, 3, true);
			}
		}
		else if (key == GLFW_KEY_S) {
			if (game_mode == 1) {
				player_activate(player, player->dir, false);
			}
		}
		else if (key == GLFW_KEY_A) {
			if (game_mode == 1 && !lose_flag) {
				bullets = create_bullet(bullets, tanks[0]);
				engine->play2D(fire_sound, false);
			}
		}
		else if (key == GLFW_KEY_W)
		{
			b_wireframe = !b_wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, b_wireframe ? GL_LINE : GL_FILL);
			printf("> using %s mode\n", b_wireframe ? "wireframe" : "solid");
		}
		else if (key == GLFW_KEY_F) {
			int w_=1920, h_=1080;
			glfwSetWindowPos(window, 0, 40);
			//glfwGetWindowSize(window, &w_, &h_);
			fullscreen_flag = !fullscreen_flag;
			fullscreen_flag ? glfwSetWindowSize(window, w_, h_) : glfwSetWindowSize(window, 1280, 720);
		}
		else if (key == GLFW_KEY_R) {
			game_mode = 0;
			engine->stopAllSounds();
			lose_flag = false;
			cam = camera();
			user_reset();
		}
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (game_mode == 1) {
			dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
			vec2 npos = cursor_to_ndc(pos, window_size);
			if (zoom + pan > 0) {
				zoom = pan = 0;
				printf("disabled zoom and pan!\n");
			}
			if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos);
			else if (action == GLFW_RELEASE)	tb.end();
		}
		else if (game_mode == 0) {
			game_mode = 1;
			engine->play2D(bgm, true);
		}
		else if (game_mode == 3) {
			game_mode = 1;
			lose_flag = false;
			engine->stopAllSounds();
			engine->play2D(bgm, true);
			user_reset();
		}
		
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = cursor_to_ndc(pos, window_size);
		if (action == GLFW_PRESS) {
			if (pan == 1) { tb.begin(cam.view_matrix, npos); }
		}
		else if (action == GLFW_RELEASE) {
			if (pan == 1) { tb.end(); }
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = cursor_to_ndc(pos, window_size);
		if (action == GLFW_PRESS) {
			if (zoom == 1) { tb.begin(cam.view_matrix, npos); }
		}
		else if (action == GLFW_RELEASE) {
			if (zoom == 1) { tb.end(); }
		}
	}
}

void motion(GLFWwindow* window, double x, double y) {
	if (!tb.is_tracking()) return;
	vec2 npos = cursor_to_ndc(dvec2(x, y), window_size);
	cam.view_matrix = tb.update(npos, zoom, pan, zoomval, panval);
}

inline void combine_vertices(std::vector<vertex> v) {
	for (auto i : v) {
		unit_combine_vertices.push_back(i);
	}
}

bool user_init() {
	print_help(); // log hotkeys
	// init GL states
	glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);								// turn on backface culling
	glEnable(GL_DEPTH_TEST);								// turn on depth tests
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//create_field_vertices(unit_field_vertices);
	create_field_vertices(unit_field_vertices);
	create_tank_vertices(unit_tank_vertices);
	create_wall_vertices(unit_wall_vertices);
	create_mirror_vertices(unit_mirror_vertices);
	create_bullet_vertices(unit_bullet_vertices);
	//combine_vertices(unit_field_vertices);
	combine_vertices(unit_field_vertices);
	combine_vertices(unit_tank_vertices);
	combine_vertices(unit_wall_vertices);
	combine_vertices(unit_mirror_vertices);
	combine_vertices(unit_bullet_vertices);
	// create vertex buffer; called again when index buffering mode is toggled

	update_vertex_buffer(unit_combine_vertices, NUM_TESS);
	//update_vertex_buffer(unit_tank_vertices, NUM_TESS);

	engine = irrklang::createIrrKlangDevice();
	if (!engine) return false;

	fire_sound = engine->addSoundSourceFromFile(fire_sound_path);
	tank_death = engine->addSoundSourceFromFile(tank_explode_sound_path);
	base_attack_sound = engine->addSoundSourceFromFile(base_attack_sound_path);
	base_explode_sound = engine->addSoundSourceFromFile(base_explode_sound_path);
	bgm = engine->addSoundSourceFromFile(bgm_path);
	winbgm = engine->addSoundSourceFromFile(winbgm_path);
	losebgm = engine->addSoundSourceFromFile(losebgm_path);

	fire_sound->setDefaultVolume(0.4f);
	tank_death->setDefaultVolume(0.5f);
	base_attack_sound->setDefaultVolume(0.4f);
	base_explode_sound->setDefaultVolume(0.4f);
	bgm->setDefaultVolume(0.6f);
	winbgm->setDefaultVolume(0.5f);
	losebgm->setDefaultVolume(0.5f);

	vertex corners[4];
	corners[0].pos = vec3(-1.0f, -1.0f, 0.0f);	corners[0].tex = vec2(0.0f, 0.0f);
	corners[1].pos = vec3(+1.0f, -1.0f, 0.0f);	corners[1].tex = vec2(1.0f, 0.0f);
	corners[2].pos = vec3(+1.0f, +1.0f, 0.0f);	corners[2].tex = vec2(1.0f, 1.0f);
	corners[3].pos = vec3(-1.0f, +1.0f, 0.0f);	corners[3].tex = vec2(0.0f, 1.0f);
	vertex intro_vertices[6] = { corners[0], corners[1], corners[2], corners[0], corners[2], corners[3] };

	GLuint vertex_buffer_intro;
	glGenBuffers(1, &vertex_buffer_intro);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_intro);
	glBufferData(GL_ARRAY_BUFFER, sizeof(intro_vertices), intro_vertices, GL_STATIC_DRAW);

	if (vertex_array_intro) glDeleteVertexArrays(1, &vertex_array_intro);
	vertex_array_intro = cg_create_vertex_array(vertex_buffer_intro);
	if (!vertex_array_intro) { printf("%s(): failed to create vertex aray\n", __func__); return false; }

	intro = create_texture(intro_path, true);	if (intro == -1) return false;
	youwin = create_texture(win_path, true);	if (youwin == -1) return false;
	youlose = create_texture(lose_path, true);	if (youlose == -1) return false;

	if (!init_text()) return false;

	return true;
}

void user_reset() {
	fields.clear();
	tanks.clear();
	walls.clear();
	mirrors.clear();
	bullets.clear();

	fields = create_field();
	tanks = create_tank();
	walls = create_wall();
	mirrors = create_mirror();

	unit_field_vertices.clear();
	unit_tank_vertices.clear();
	unit_wall_vertices.clear();
	unit_mirror_vertices.clear();
	unit_bullet_vertices.clear();

	player = &tanks[0];
	player->health = 6;
}

void user_finalize() {
	engine->drop();
}

int main(int argc, char* argv[]) {

	// create window and initialize OpenGL extensions
	if (!(window = cg_create_window(window_name, window_size.x, window_size.y))) { glfwTerminate(); return 1; }
	if (!cg_init_extensions(window)) { glfwTerminate(); return 1; }	// version and extensions
	// initializations and validations
	if (!(program = cg_create_program(vert_shader_path, frag_shader_path))) { glfwTerminate(); return 1; }	// create and compile shaders/program
	user_reset();
	if (!user_init()) { printf("Failed to user_init()\n"); glfwTerminate(); return 1; }					// user initialization
	// register event callbacks
	glfwSetWindowSizeCallback(window, reshape);	// callback for window resizing events
	glfwSetKeyCallback(window, keyboard);			// callback for keyboard events
	glfwSetMouseButtonCallback(window, mouse);	// callback for mouse click inputs
	glfwSetCursorPosCallback(window, motion);		// callback for mouse movement
	// enters rendering/event loop
	start_time = (float)glfwGetTime();
	double lastTime = glfwGetTime();

	for (frame = 0; !glfwWindowShouldClose(window); frame++) {
		double currentTime = glfwGetTime();
		glfwPollEvents();	// polling and processing of events
		if (lose_flag && currentTime - lose_time > 2) game_mode = 3;
		update((float)(currentTime-lastTime)*0.66f);			// per-frame update
		render((float)(currentTime-lastTime)*0.66f);			// per-frame render
		lastTime = currentTime;
	}
	// normal termination
	user_finalize();
	cg_destroy_window(window);
	return 0;
}