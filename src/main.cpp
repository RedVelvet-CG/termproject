#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "trackball.h"	// virtual trackball
#include "object.h"

// global constants
static const char* window_name = "2014312455 - A3 Planets in universe";
static const char* vert_shader_path = "../bin/shaders/trackball.vert";
static const char* frag_shader_path = "../bin/shaders/trackball.frag";
static const char* universe_background_path = "../src/images/universe.png";
uint				NUM_TESS = 50;		// initial tessellation factor of the circle as a polygon

// common structures
struct camera {
	vec3	eye = vec3(0, 30, 300), at = vec3(0, 0, 0), up = vec3(0, 1, 0);
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

// global variables
int		frame = 0;				// index of rendering frames
float	t = 0.0f;
auto	universe = create_universe();
int		zoom = 0;
int		pan = 0;
int		zoomval = 50;
int		panval = 30;
int		dflag = 0;
int		rotspeed = 1;
int		selfrotspeed = 1;
int		rottoggle = 1;
int		selfrottoggle = 1;
float	timeval = 0;
int		pauseflag = 0;
int		planet[10] = { 1,1,1,1,1,1,1,1,1 };
bool	b_wireframe = false;

// scene objects
camera		cam;
trackball	tb;

// holder of vertices and indices of a unit circle
std::vector<vertex>	unit_circle_vertices;	// host-side vertices

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
	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program); // notify GL that we use our own program
	glBindVertexArray(vertex_array); // bind vertex array object	
	int idx = 0;
	for (auto& c : universe) {
		GLint uloc;
		uloc = glGetUniformLocation(program, "solid_color");		if (uloc > -1) glUniform4fv(uloc, 1, c.color);	// pointer version
		float t = float(glfwGetTime());
		if (pauseflag == 0) {
			timeval += 0.0001f;
		}
		mat4 model_matrix = mat4::rotate(vec3(0, 1, 0), timeval * c.movval.y * rotspeed * rottoggle * planet[idx]) *  //rotation around sun
			mat4::translate(c.movval.x, 0, 0) *
			mat4::rotate(vec3(0, 1, 0), timeval * c.theta * selfrotspeed * selfrottoggle * planet[idx]) * //self-rotation
			mat4::translate(0, 0, 0) *
			mat4::rotate(vec3(0, 0, 1), timeval * c.theta * dflag) *
			mat4::scale(c.radius, c.radius, c.radius);
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, c.creation_val, GL_UNSIGNED_INT, nullptr);
		idx++;
	}
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
	printf("*******************Extra features*******************\n");
	printf("- press [ or ] key to adjust zoom speed\n");
	printf("- press < or > key t adjust pan speed\n");
	printf("- press d to diversify self-rotation axis\n");
	printf("- press ( or ) key to adjust rotation speed\n");
	printf("- press + or - to adjust self-rotation speed\n");
	printf("- press t to change rotation direction\n");
	printf("- press y to change self-rotation direction\n");
	printf("- press p to pause time\n");
	printf("- press 1~8 of keypad1~8 to boost/un-boost specific planet's time\n\n");
}

void update_vertex_buffer(const std::vector<vertex>& vertices, uint N) {
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer
	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; } // check exceptions
	std::vector<uint> indices = makeindices(N); // create buffers
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
		else if (key == GLFW_KEY_RIGHT_BRACKET) {
			zoomval += 10;
			printf("zooms faster!\tzoomspeed: %d\n", zoomval);
		}
		else if (key == GLFW_KEY_LEFT_BRACKET) {
			zoomval = max(0, zoomval - 10);
			printf("zooms slower!\tzoomspeed: %d\n", zoomval);
		}
		else if (key == GLFW_KEY_PERIOD) {
			panval += 10;
			printf("pans faster!\tpanspeed: %d\n", panval);
		}
		else if (key == GLFW_KEY_COMMA) {
			panval = max(0, panval - 10);
			printf("pans slower!\tpanspeed: %d\n", panval);
		}
		else if (key == GLFW_KEY_D) {
			dflag = 1 - dflag;
			printf("%s\n", dflag == 1 ? "diversified self-rotation axis" : "neutralized self-rotation axis");
		}
		else if (key == GLFW_KEY_0) {
			rotspeed += 1;
			printf("rotating in x%d speed\n", rotspeed);
		}
		else if (key == GLFW_KEY_9) {
			rotspeed = max(1, rotspeed - 1);
			if (rotspeed == 1) {
				printf("rotating in minimum speed\n");
			}
			else {
				printf("rotating in x%d speed\n", rotspeed);
			}
		}
		else if (key == GLFW_KEY_1) {
			planet[1]++;
			printf("Mercury's time flows in x%d speed!\n", planet[1]);
		}
		else if (key == GLFW_KEY_KP_1) {
			planet[1] = max(1, planet[1] - 1);
			printf("Mercury's time flows in x%d speed!\n", planet[1]);
		}
		else if (key == GLFW_KEY_2) {
			planet[2]++;
			printf("Venus' time flows in x%d speed!\n", planet[2]);
		}
		else if (key == GLFW_KEY_KP_2) {
			planet[2] = max(1, planet[2] - 1);
			printf("Venus' time flows in x%d speed!\n", planet[2]);
		}
		else if (key == GLFW_KEY_3) {
			planet[3]++;
			printf("Earth's time flows in x%d speed!\n", planet[3]);
		}
		else if (key == GLFW_KEY_KP_3) {
			planet[3] = max(1, planet[3] - 1);
			printf("Earth's time flows in x%d speed!\n", planet[3]);
		}
		else if (key == GLFW_KEY_4) {
			planet[4]++;
			printf("Mars' time flows in x%d speed!\n", planet[4]);
		}
		else if (key == GLFW_KEY_KP_4) {
			planet[4] = max(1, planet[4] - 1);
			printf("Mars' time flows in x%d speed!\n", planet[4]);
		}
		else if (key == GLFW_KEY_5) {
			planet[5]++;
			printf("Jupitor's time flows in x%d speed!\n", planet[5]);
		}
		else if (key == GLFW_KEY_KP_5) {
			planet[5] = max(1, planet[5] - 1);
			printf("Jupitor's time flows in x%d speed!\n", planet[5]);
		}
		else if (key == GLFW_KEY_6) {
			planet[6]++;
			printf("Saturn's time flows in x%d speed!\n", planet[6]);
		}
		else if (key == GLFW_KEY_KP_6) {
			planet[6] = max(1, planet[6] - 1);
			printf("Saturn's time flows in x%d speed!\n", planet[6]);
		}
		else if (key == GLFW_KEY_7) {
			planet[7]++;
			printf("Uranus' time flows in x%d speed!\n", planet[7]);
		}
		else if (key == GLFW_KEY_KP_7) {
			planet[7] = max(1, planet[7] - 1);
			printf("Uranus' time flows in x%d speed!\n", planet[7]);
		}
		else if (key == GLFW_KEY_8) {
			planet[8]++;
			printf("Neptune's time flows in x%d speed!\n", planet[8]);
		}
		else if (key == GLFW_KEY_KP_8) {
			planet[8] = max(1, planet[8] - 1);
			printf("Neptune's time flows in x%d speed!\n", planet[8]);
		}
		else if (key == GLFW_KEY_EQUAL) {
			selfrotspeed += 1;
			printf("self-rotating in x%d speed\n", selfrotspeed);
		}
		else if (key == GLFW_KEY_MINUS) {
			selfrotspeed = max(1, selfrotspeed - 1);
			if (selfrotspeed == 1) {
				printf("self-rotating in minimum speed\n");
			}
			else {
				printf("selfrotating in x%d speed\n", selfrotspeed);
			}
		}
		else if (key == GLFW_KEY_T) {
			rottoggle *= -1;
			printf("%s\n", rottoggle == 1 ? "rotating anti-clockwise from god's view" : "rotating clockwise from god's view");
		}
		else if (key == GLFW_KEY_Y) {
			selfrottoggle *= -1;
			printf("%s\n", selfrottoggle == 1 ? "rotating anti-clockwise from god's view" : "rotating clockwise from god's view");
		}
		else if (key == GLFW_KEY_P) {
			pauseflag = 1 - pauseflag;
			printf("%s\n", pauseflag == 1 ? "time paused" : "time resume");
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

bool user_init() {
	print_help(); // log hotkeys
	// init GL states
	glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
	glEnable(GL_CULL_FACE);								// turn on backface culling
	glEnable(GL_DEPTH_TEST);								// turn on depth tests
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	create_circle_vertices(unit_circle_vertices, NUM_TESS, 0, 0, 0, 1.f);
	// create vertex buffer; called again when index buffering mode is toggled
	update_vertex_buffer(unit_circle_vertices, NUM_TESS);
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