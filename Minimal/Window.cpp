#include "window.h"
#include "OBJObject.h"
#include "Transform.h"
#include "Geometry.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

const char* window_title = "GLFW Starter Project";
GLint shaderProgram;
glm::vec3 translations[10000];
bool showSphere = false;
bool toggleGrid = false;
bool cullingOn = true;
bool toggleFrustum = false;
int mouse = 0;
glm::vec3 lastPos, curPos;
glm::mat4 rotateGrid = glm::mat4(1.0f);
//top, bottom, left, right, nearP, farP
glm::vec3 planeNorms[6];
glm::vec3 planePoints[6];
float nearDist = 0.1f;
float farDist = 1000.0f;
float ratio;
float sphereRadius = 2.5f;
int numOfRobots = 10000;

Transform * robot;
Transform * sphereT;
Transform * body2head;
Transform * head2eyel;
Transform * head2eyer;
Transform * body2arml;
Transform * body2armr;
Transform * body2legl;
Transform * body2legr;
Transform * arm2sword;
Transform * arm2shield;
Transform * armr2armor;
Transform * arml2armor;
Transform * head2helmet;
Geometry * body;
Geometry * head;
Geometry * arm_l;
Geometry * arm_r;
Geometry * leg_l;
Geometry * leg_r;
Geometry * eye_l;
Geometry * eye_r;
Geometry * sphere;
Geometry * sword;
Geometry * shield;
Geometry * armor;
Geometry * armor2;
Geometry * helmet;


// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 10.0f, 50.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
//glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;
const glm::mat4 I = glm::mat4(1.0f);

void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	int index = 0;
	for (int z = -50; z < 50; z++)
	{
		for (int x = -50; x < 50; x++)
		{
			glm::vec3 translation;
			translation.x = x * 5.0f;
			translation.y = 0.0f;
			translation.z = z* -5.0f;
			translations[index] = translation;
			index++;
		}
	}

	robot = new Transform(I);
	body = new Geometry();
	body->init("body_s.obj", shaderProgram, 1.0f);
	robot->addChild(body);
	head = new Geometry();
	head->init("head_s.obj", shaderProgram, 1.0f);
	body2head = new Transform(glm::translate(I, glm::vec3(0, 1.5f, 0)));
	robot->addChild(body2head);
	body2head->addChild(head);
	glm::mat4 S = glm::scale(I, glm::vec3(1.5f, 1.5f, 1.5f));
	
	eye_l = new Geometry();
	eye_l->init("eyeball_s.obj", shaderProgram, 1.5f);
	eye_r = new Geometry();
	eye_r->init("eyeball_s.obj", shaderProgram, 1.5f);
	head2eyel = new Transform(glm::translate(I, glm::vec3(-0.3f, 0.0f, 0.75f)));
	head2eyer = new Transform(glm::translate(I, glm::vec3(0.3f, 0.0f, 0.75f)));
	body2head->addChild(head2eyel);
	body2head->addChild(head2eyer);
	head2eyel->addChild(eye_r);
	head2eyer->addChild(eye_r);
	
	arm_l = new Geometry();
	arm_l->init("limb_s.obj", shaderProgram, 1.2f);
	arm_r = new Geometry();
	arm_r->init("limb_s.obj", shaderProgram, 1.2f);
	body2arml = new Transform(glm::translate(I, glm::vec3(-1.25f, 0.0f, 0.0f)));
	body2armr = new Transform(glm::translate(I, glm::vec3(1.25f, 0.0f, 0.0f)));
	robot->addChild(body2arml);
	robot->addChild(body2armr);
	body2arml->addChild(arm_l);
	body2armr->addChild(arm_r);

	sword = new Geometry();
	sword->init("antenna_s.obj", shaderProgram, 0.8f);
	glm::mat4 T = glm::rotate(I, glm::radians(90.0f), glm::vec3(1.0f, 0, 0));
	T = T*glm::translate(I, glm::vec3(0, 1.5f, 0));
	T = T*glm::translate(I, glm::vec3(0, 0.0f, 0.5f));
	arm2sword = new Transform(T);
	body2armr->addChild(arm2sword);
	arm2sword->addChild(sword);
	
	shield = new Geometry();
	shield->init("body_s.obj", shaderProgram, 1.0f);
	T = glm::translate(I, glm::vec3(-0.3f, -0.3f, 0));
	T = glm::rotate(T, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	T = glm::rotate(T, glm::radians(85.0f), glm::vec3(0.0f, 0, -1.0f));
	T = glm::scale(T, glm::vec3(1.0f, 0.1f, 1.0f));
	arm2shield = new Transform(T);
	body2arml->addChild(arm2shield);
	arm2shield->addChild(shield);

	armor = new Geometry();
	armor->init("head_s.obj", shaderProgram, 0.5f);
	T = glm::rotate(I, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	T = glm::translate(T, glm::vec3(-0.4f, 0.5f, 0));
	T = glm::scale(T, glm::vec3(1.0f, 0.5f, 1.0f));
	armr2armor = new Transform(T);
	body2armr->addChild(armr2armor);
	armr2armor->addChild(armor);

	armor2 = new Geometry();
	armor2->init("head_s.obj", shaderProgram, 0.5f);
	T = glm::rotate(I, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	T = glm::translate(T, glm::vec3(0.4f, 0.5f, 0));
	T = glm::scale(T, glm::vec3(1.0f, 0.5f, 1.0f));
	arml2armor = new Transform(T);
	body2arml->addChild(arml2armor);
	arml2armor->addChild(armor2);

	helmet = new Geometry();
	helmet->init("body_s.obj", shaderProgram, 0.8f);
	T = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0, -1.0f));
	T = glm::scale(T, glm::vec3(1.5f, 0.1f, 1.0f));
	head2helmet = new Transform(T);
	body2head->addChild(head2helmet);
	head2helmet->addChild(helmet);
	
	leg_l = new Geometry();
	leg_l->init("limb_s.obj", shaderProgram, 1.2f);
	leg_r = new Geometry();
	leg_r->init("limb_s.obj", shaderProgram, 1.2f);
	body2legl = new Transform(glm::translate(I, glm::vec3(-0.5f, -1.25f, 0.0f)));
	body2legr = new Transform(glm::translate(I, glm::vec3(0.5f, -1.25f, 0.0f)));
	robot->addChild(body2legl);
	robot->addChild(body2legr);
	body2legl->addChild(leg_l);
	body2legr->addChild(leg_r);
	
	sphereT = new Transform(glm::mat4(1.0f));
	sphere = new Geometry();
	sphere->init("eyeball_s.obj", shaderProgram, 30.0f);
	sphereT->addChild(sphere);
	calculateFrustum();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(glm::radians(45.0f), (float)width / (float)height, nearDist, farDist);
		ratio = (float)width / (float)height;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	body2arml->update(1.0f);
	body2armr->update(-1.0f);
	body2legl->update(-1.0f);
	body2legr->update(1.0f);
	//robot->goUp();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	drawRobotArmy();
	//robot->draw(I);

	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float speed = 0.2f;
	if (action == GLFW_REPEAT) {
		if (key == GLFW_KEY_LEFT) {
			//V = glm::translate(V, -cam_pos);
			//V = glm::rotate(V, speed, glm::vec3(0, 1, 0));
			//V = glm::translate(V, cam_pos);
			cam_look_at += glm::vec3(-speed, 0.0f, 0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_RIGHT) {
			cam_look_at += glm::vec3(speed, 0.0f, 0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_UP) {
			cam_look_at += glm::vec3(0, speed, 0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_DOWN) {
			cam_look_at += glm::vec3(0, -speed, 0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_EQUAL) {
			glm::vec3 dir = cam_pos - cam_look_at;
			cam_pos += dir * 0.01f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_MINUS) {
			glm::vec3 v = cam_look_at - cam_pos;
			cam_pos += v*0.01f;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
	}
	
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_Q) {
			showSphere = !showSphere;
		}
		if (key == GLFW_KEY_T) {
			toggleGrid = !toggleGrid;
		}
		if (key == GLFW_KEY_C) {
			cullingOn = !cullingOn;
		}
		if (key == GLFW_KEY_F) {
			toggleFrustum = !toggleFrustum;
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
		mouse = 1;
	}
	else {
		mouse = 0;
	}
}

void Window::cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	if (mouse == 1) {
		lastPos = trackBallMapping(x, y);
		mouse = 2;
	}
	else if (mouse == 2) {
		curPos = trackBallMapping(x, y);
		glm::vec3 rotAxis = glm::cross(lastPos, curPos);
		float angle = asin(glm::length(rotAxis)) / 30.0f;
		if (toggleGrid) {
			rotateGrid = glm::rotate(rotateGrid, angle, rotAxis);
		}
		else {
			//rotateGrid = glm::mat4(1.0f);
			glm::mat4 R = glm::rotate(I, angle, rotAxis);

			glm::vec3 cam_dir = cam_pos - cam_look_at;
			cam_dir = glm::vec3(glm::vec4(cam_dir, 1.0f) * R);

			cam_look_at = cam_pos - cam_dir;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
	}
}

glm::vec3 Window::trackBallMapping(double x, double y) {
	glm::vec3 v(0.0f);

	v.x = (2 * x - width) / width;
	v.y = -(2 * y - height) / height;

	float d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d*d);
	v = glm::normalize(v);

	return v;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
	}
}

void Window::calculateFrustum() {
	glm::vec3 up = cam_up;
	glm::vec3 p = cam_pos;
	glm::vec3 d = glm::normalize(cam_look_at - cam_pos);
	glm::vec3 right = glm::normalize(glm::cross(d, cam_up));
	float Hnear = nearDist * glm::tan(glm::radians(22.5f)) * 2;
	float Wnear = Hnear * ratio;
	float Hfar = farDist * glm::tan(glm::radians(22.5f)) * 2;
	float Wfar = Hfar*ratio;
	glm::vec3 fc = p + d*farDist;
	
	glm::vec3 ftl = fc + (up * Hfar / 2.0f) - (right * Wfar / 2.0f);
	glm::vec3 ftr = fc + (up * Hfar / 2.0f) + (right * Wfar / 2.0f);
	glm::vec3 fbl = fc - (up * Hfar / 2.0f) - (right * Wfar / 2.0f);
	glm::vec3 fbr = fc - (up * Hfar / 2.0f) + (right * Wfar / 2.0f);
	glm::vec3 nc = p + d * nearDist;
	glm::vec3 ntl = nc + (up * Hnear / 2.0f) - (right * Wnear / 2.0f);
	glm::vec3 ntr = nc + (up * Hnear / 2.0f) + (right * Wnear / 2.0f);
	glm::vec3 nbl = nc - (up * Hnear / 2.0f) - (right * Wnear / 2.0f);
	glm::vec3 nbr = nc - (up * Hnear / 2.0f) + (right * Wnear / 2.0f);
	createPlane(ntr, ntl, ftl, 0);
	createPlane(nbl, nbr, fbr, 1);
	createPlane(ntl, nbl, fbl, 2);
	createPlane(nbr, ntr, fbr, 3);
	createPlane(ntl, ntr, nbr, 4);
	createPlane(ftr, ftl, fbl, 5);
	//print(planeNorms[0]);
}

void Window::createPlane(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, int id) {
	glm::vec3 aux1 = v1 - v2;
	glm::vec3 aux2 = v3 - v2;
	glm::vec3 normal = glm::normalize(glm::cross(aux2, aux1));
	planeNorms[id] = normal;
	planePoints[id] = v2;
}

void Window::print(glm::vec3 &v) {
	cout << v.x << " " << v.y << " " << v.z << endl;
}

bool Window::sphereInFrustum(glm::vec3 &p, float radius) {
	float dist = 0.0f;
	for (int i = 0; i < 6; i++) {
		dist = glm::dot((p - planePoints[i]), planeNorms[i]);
		if (dist < -radius) {
			return false;
		}
	}
	return true;
}

void Window::drawRobotArmy() {
	if (!toggleFrustum && cullingOn) {
		calculateFrustum();
	}
	int count = 0;
	for (int i = 0; i < 10000; i++) {
		glm::mat4 T = glm::translate(I, translations[i]);
		//glm::mat4 R = glm::rotate(I, 5.0f, glm::vec3(0.0f,0,1.0f));
		glm::mat4 P = rotateGrid * T;
		glm::vec3 pos = glm::vec3(P[3][0], P[3][1], P[3][2]);
		if (!cullingOn) {
			robot->draw(P);
			if (showSphere) {
				sphereT->draw(P);
			}
			count++;
		}
		else if (sphereInFrustum(pos, sphereRadius)) {
			robot->draw(P);
			if (showSphere) {
				sphereT->draw(P);
			}
			count++;
		}
	}
	if (count != numOfRobots) {
		cout << count << endl;
		numOfRobots = count;
	}
}