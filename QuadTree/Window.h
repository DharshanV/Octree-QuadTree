#pragma once
#include <Utitlites/GLFW.h>
#include <Utitlites/Camera.h>
#include <Utitlites/VertexArray.h>
#include <Utitlites/VertexBuffer.h>
#include <Utitlites/VertexBufferLayout.h>
#include <Utitlites/Shader.h>
#include <iostream>
#include <ctime>
#include "QuadTree.h"
#include "Model.h"
using namespace glm;

//Variables
int screenWidth = 1600;
int screenHeight = 900;
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool renderbox = false;
bool renderLines = true;
int spawnCount = 5;
float treeSize = 10.0f;
float crossHair[] = {
	0.0f,0.015f,0.0f,
	0.0f,-0.017f,0.0f,
	-0.012f,0.0f,0.0f,
	0.012f,0.f,0.0f
};

GLFW window(screenWidth, screenHeight, "Octree");
Camera camera(glm::vec3(0.0f, 0.0f, 3.0));
Boundary treeBound(vec3(-treeSize, treeSize + 10, treeSize), vec3(treeSize, -treeSize + 10, -treeSize));
QuadTree tree(treeBound, 10);


//vectors
vector<vec3> points;
vector<vec3> selectedPoints;
vector<Triangle> selectedTriangles;

//lists
vector<Line> lines;

//Shaders
Shader treeBoxShader("boxShader.vert", "boxShader.frag");
Shader pointsShader("pointsShader.vert", "pointsShader.frag");
Shader selectedShader("selectedShader.vert", "selectedShader.frag");
Shader lineShader("lineShader.vert", "lineShader.frag");
Shader modelShader("modelShader.vert", "modelShader.frag");

//Model
Model nanosuit("Nanosuit//nanosuit.obj");

//Functions
void processInput(GLFWwindow* glfwwindow) {
	if (glfwGetKey(glfwwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwwindow, true);
	if (glfwGetKey(glfwwindow, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(glfwwindow, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(glfwwindow, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(glfwwindow, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(glfwwindow, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(glfwwindow, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(glfwwindow, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(glfwwindow, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(glfwwindow, GLFW_KEY_0) == GLFW_PRESS)
		renderbox = false;
	if (glfwGetKey(glfwwindow, GLFW_KEY_9) == GLFW_PRESS)
		renderbox = true;
	if (glfwGetKey(glfwwindow, GLFW_KEY_3) == GLFW_PRESS)
		renderLines = false;
	if (glfwGetKey(glfwwindow, GLFW_KEY_4) == GLFW_PRESS)
		renderLines = true;
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		lines.push_back(Line(camera.Position, camera.Front));
		tree.searchTriangles(lines[lines.size() - 1], selectedPoints);
	}
}

void resizeCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	screenWidth = width;
	screenHeight = height;
}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

float random(float min, float max) {
	float temp = static_cast <float> (rand());
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

void drawCrossHair() {
	VertexArray VAO3;
	VertexBuffer VBO3(crossHair, sizeof(crossHair));
	VertexBufferLayout layout;
	layout.push<float>(3);
	VAO3.addBuffer(VBO3, layout);
	glDrawArrays(GL_LINES, 0, 4);
}