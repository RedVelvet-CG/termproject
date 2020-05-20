#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#define STB_IMAGE_IMPLEMENTATION
#include "trackball.h"	// virtual trackball
#include"field.h"
#include "tank.h"
#include "wall.h"
#include "stb_image.h"
#include "bullet.h"
#include <cstdlib>
#include <ctime>

// global constants
static const char* window_name = "Tanks!";
static const char* vert_shader_path = "../bin/shaders/trackball.vert";
static const char* frag_shader_path = "../bin/shaders/trackball.frag";
static const char* brick_path = "../bin/images/brick.jpg";
static const char* iron_path = "../bin/images/iron.jpg";
static const char* skku_path = "../bin/images/skku.jpg";
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

// OpenGL objects
GLuint	program = 0;	// ID holder for GPU program
GLuint	vertex_array = 0;
GLuint	brick = 0;
GLuint	iron = 0;
GLuint	skku = 0;

// global variables
int		frame = 0;				// index of rendering frames
float	t = 0.0f;
float	start_time = 0.f;
auto	fields = create_field();
auto	tanks = create_tank();
auto	walls = create_wall();
int		zoom = 0;
int		pan = 0;
int		zoomval = 50;
int		panval = 30;
bool	b_wireframe = false;
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

//game variables
tank*	player = &tanks[0];
int		movdir = 1;


void update() {
	// update projection matrix
	cam.aspect = window_size.x / float(window_size.y);
	cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect, cam.dnear, cam.dfar);
	// build the model matrix for oscillating scale
	float t = float(glfwGetTime());
	GLint uloc;
	uloc = glGetUniformLocation(program, "view_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
	uloc = glGetUniformLocation(program, "projection_matrix");	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);
	//uloc = glGetUniformLocation( program, "model_matrix" );			if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, model_matrix );
}

void render() {
	srand((unsigned int)time(0));

	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	for (auto& f : fields) {
		f.update();
		glUniform1i(glGetUniformLocation(program, "mode"), 4);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, f.model_matrix);
		glDrawElements(GL_TRIANGLES, f.creation_val, GL_UNSIGNED_INT, (void*)(f.creation_val * 0 * sizeof(GLuint)));
	}
	
	for (auto& t : tanks) {
		t.update();
		if (t.isenemy) {
			enemy_move(player, &t, (float)glfwGetTime() * 10000, walls, tanks);
			float time_now = (float)glfwGetTime() - start_time;
			if (time_now - t.bulletstamp > 2.0f) {
				t.bulletstamp = time_now;
				int shot_fire_check = rand() % 2;
				if (shot_fire_check == 1) bullets = create_bullet(bullets, t);
			}
		}
		else if (t.movflag) player_move(&t, walls, tanks);
		glUniform1i(glGetUniformLocation(program, "mode"), 3);
		GLint uloc;
		uloc = glGetUniformLocation(program, "color"); if (uloc > -1) glUniform4fv(uloc, 1, t.color);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, t.model_matrix);
		glDrawElements(GL_TRIANGLES, t.creation_val, GL_UNSIGNED_INT, (void*)(fields[0].creation_val * 1*sizeof(GLuint)));
	}

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

	int del_bullet_checker = 0;
	for (auto& b : bullets) {
		bool bullet_break_checker = false;
		int del_wall_checker = 0;
		for (auto& w : walls)
		{
			if (abs(w.center.x - b.center.x) <= 10.0f && abs(w.center.y - b.center.y) <= 10.0f)
			{
				if (player->plane != w.plane) continue;
				bullet_break_checker = true;
				if(w.breakable) del_walls.push_back(del_wall_checker);
			}
			del_wall_checker++;
		}

		int del_tank_checker = 0;
		for (auto& t : tanks)
		{
			if ((!t.isenemy && b.is_mine) || (t.isenemy && !b.is_mine))
			{
				del_tank_checker++;
				continue;
			}
			if (abs(t.center.x - b.center.x) <= 10.0f && abs(t.center.y - b.center.y) <= 10.0f)
			{
				bullet_break_checker = true;
				if ((t.isenemy && b.is_mine) || (!t.isenemy && !b.is_mine))
				{
					t.health--;
					if (t.health == 0) del_tanks.push_back(del_tank_checker);
				}
			}
			del_tank_checker++;
		}

		del_bullet_checker++;
		if (bullet_break_checker)
		{
			del_bullets.push_back(del_bullet_checker);
		}
		b.update();
		glUniform1i(glGetUniformLocation(program, "mode"), 3);
		if (b.theta == 0) b.center.x -= 0.5f;
		else if (b.theta == -PI / 2) b.center.y += 0.5f;
		else if (b.theta == PI) b.center.x += 0.5f;
		else if (b.theta == PI / 2) b.center.y -= 0.5f;
		GLint uloc;
		uloc = glGetUniformLocation(program, "color"); if (uloc > -1) glUniform4fv(uloc, 1, b.color);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE,b.model_matrix);
		glDrawElements(GL_TRIANGLES, b.creation_val, GL_UNSIGNED_INT, (void*)((fields[0].creation_val + tanks[0].creation_val + walls[0].creation_val) * sizeof(GLuint)));
		
	}
	
	int i = 1;
	for (auto& db : del_bullets)
	{
		bullets.erase(bullets.begin() + db - i);
		i++;
	}
	del_bullets.clear();

	i = 0;
	for (auto& dw : del_walls)
	{
		walls.erase(walls.begin() + dw - i);
		i++;
	}
	del_walls.clear();

	i = 0;
	for (auto& dt : del_tanks)
	{
		tanks.erase(tanks.begin() + dt - i);
		i++;
	}
	del_tanks.clear();

	// swap front and back buffers, and display to screen
	glfwSwapBuffers(window);
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
	make_bullet_indices(indices, 56);
								   
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
			player_activate(player, 0, true);
		}
		else if (key == GLFW_KEY_UP) {
			player_activate(player, 1, true);
		}
		else if (key == GLFW_KEY_RIGHT) {
			player_activate(player, 2, true);
		}
		else if (key == GLFW_KEY_DOWN) {
			player_activate(player, 3, true);
		}
		else if (key == GLFW_KEY_S) {
			player_activate(player, player->dir, false);
		}
		else if (key == GLFW_KEY_A) {
			bullets = create_bullet(bullets, tanks[0]);
		}
		else if (key == GLFW_KEY_W)
		{
			b_wireframe = !b_wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, b_wireframe ? GL_LINE : GL_FILL);
			printf("> using %s mode\n", b_wireframe ? "wireframe" : "solid");
		}
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = cursor_to_ndc(pos, window_size);
		if (zoom + pan > 0) {
			zoom = pan = 0;
			printf("disabled zoom and pan!\n");
		}
		if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos);
		else if (action == GLFW_RELEASE)	tb.end();
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
	glEnable(GL_CULL_FACE);								// turn on backface culling
	glEnable(GL_DEPTH_TEST);								// turn on depth tests
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	//create_field_vertices(unit_field_vertices);
	create_field_vertices(unit_field_vertices);
	create_tank_vertices(unit_tank_vertices);
	create_wall_vertices(unit_wall_vertices);
	create_bullet_vertices(unit_bullet_vertices);
	//combine_vertices(unit_field_vertices);
	combine_vertices(unit_field_vertices);
	combine_vertices(unit_tank_vertices);
	combine_vertices(unit_wall_vertices);
	combine_vertices(unit_bullet_vertices);
	// create vertex buffer; called again when index buffering mode is toggled
	
	update_vertex_buffer(unit_combine_vertices, NUM_TESS);
	//update_vertex_buffer(unit_tank_vertices, NUM_TESS);
	
	return true;
}

void user_finalize() {}

int main(int argc, char* argv[]) {
	
	// create window and initialize OpenGL extensions
	if (!(window = cg_create_window(window_name, window_size.x, window_size.y))) { glfwTerminate(); return 1; }
	if (!cg_init_extensions(window)) { glfwTerminate(); return 1; }	// version and extensions
	// initializations and validations
	if (!(program = cg_create_program(vert_shader_path, frag_shader_path))) { glfwTerminate(); return 1; }	// create and compile shaders/program
	if (!user_init()) { printf("Failed to user_init()\n"); glfwTerminate(); return 1; }					// user initialization
	// register event callbacks
	glfwSetWindowSizeCallback(window, reshape);	// callback for window resizing events
	glfwSetKeyCallback(window, keyboard);			// callback for keyboard events
	glfwSetMouseButtonCallback(window, mouse);	// callback for mouse click inputs
	glfwSetCursorPosCallback(window, motion);		// callback for mouse movement
	// enters rendering/event loop
	start_time = (float)glfwGetTime();
	for (frame = 0; !glfwWindowShouldClose(window); frame++) {
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		render();			// per-frame render
	}
	// normal termination
	user_finalize();
	cg_destroy_window(window);
	return 0;
}