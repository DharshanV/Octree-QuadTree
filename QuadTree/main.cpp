#include <iostream>
#include <ctime>
#include <Utitlites/GLFW.h>
#include <Utitlites/Camera.h>
#include <Utitlites/VertexArray.h>
#include <Utitlites/VertexBuffer.h>
#include <Utitlites/VertexBufferLayout.h>
#include <Utitlites/Shader.h>
#include "QuadTree.h"
#include "Point.h"
using namespace glm;
void processInput(GLFWwindow *window);
void resizeCallBack(GLFWwindow* window, int width, int height);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
float random(float min, float max) {
	float temp = static_cast <float> (rand());
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}
int screenWidth = 800;
int screenHeight = 600;
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

GLFW window(screenWidth, screenHeight, "Octree");
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool renderbox = false;

int main() {
	srand(time(0));
	if (!window.isGood()) { glfwTerminate(); return -1; }
	window.setResizeCallBack(resizeCallBack);
	window.setMouseMovementCallBack(mouseCallBack);
	window.setCursor(GLFW_CURSOR_DISABLED);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Shader treeBoxShader("boxShader.vert", "boxShader.frag");
	Shader searchBoxShader("searchBoxShader.vert", "searchBoxShader.frag");
	Shader pointsShader("pointsShader.vert", "pointsShader.frag");
	Shader selectedShader("selectedShader.vert", "selectedShader.frag");

	vector<vec3> points;
	vector<vec3> selectedPoints;
	float treeSize = 8.0f;
	float searchSize = 2.5f;
	int count = 50000;
	Boundary treeBound(vec3(-treeSize, treeSize, treeSize),vec3(treeSize,-treeSize,-treeSize));
	Boundary searchBound(vec3(-searchSize, searchSize, searchSize), vec3(searchSize, -searchSize, -searchSize));
	QuadTree tree(treeBound, 30);
#pragma omp parallel
#pragma omp for
	for (int i = 0; i < count; i++) {
		float x = random(-treeSize, treeSize);
		float y = random(-treeSize, treeSize);
		float z = random(-treeSize, treeSize);
		tree.insert(vec3(x,y,z));
		points.push_back(vec3(x, y, z));
	}

	tree.searchArea(searchBound, selectedPoints);

	VertexArray VAO1;
	VertexBuffer VBO1(&points[0].x, points.size() * 12);
	VertexBufferLayout layout1;
	layout1.push<float>(3);
	VAO1.addBuffer(VBO1, layout1);

	VertexArray VAO2;
	float* ptr = (selectedPoints.size() == 0) ? NULL : &selectedPoints[0].x;
	VertexBuffer VBO2(ptr, selectedPoints.size() * 12);
	VertexBufferLayout layout2;
	layout2.push<float>(3);
	VAO2.addBuffer(VBO2, layout2);

	while (!window.close()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window.getWindow());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 model(1.0f);
		mat4 view = camera.GetViewMatrix();
		mat4 projection = glm::perspective(45.0f, (float)screenWidth / screenHeight, 0.1f, 100.0f);

		treeBoxShader.use();
		treeBoxShader.setMat4f("model", &model[0][0]);
		treeBoxShader.setMat4f("view", &view[0][0]);
		treeBoxShader.setMat4f("projection", &projection[0][0]);
		if (renderbox) tree.draw();

		searchBoxShader.use();
		searchBoxShader.setMat4f("model", &model[0][0]);
		searchBoxShader.setMat4f("view", &view[0][0]);
		searchBoxShader.setMat4f("projection", &projection[0][0]);
		searchBound.draw();

		pointsShader.use();
		pointsShader.setMat4f("model", &model[0][0]);
		pointsShader.setMat4f("view", &view[0][0]);
		pointsShader.setMat4f("projection", &projection[0][0]);
		VAO1.bind();
		glPointSize(3.0f);
		glDrawArrays(GL_POINTS, 0, points.size());

		selectedShader.use();
		selectedShader.setMat4f("model", &model[0][0]);
		selectedShader.setMat4f("view", &view[0][0]);
		selectedShader.setMat4f("projection", &projection[0][0]);
		VAO2.bind();
		glPointSize(6.0f);
		glDrawArrays(GL_POINTS, 0, selectedPoints.size());

		window.swapBuffers();
		window.getEvents();
	}
	return 0;
}

void processInput(GLFWwindow* glfwwindow) {
	if (glfwGetKey(glfwwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwwindow, true);
	if (glfwGetKey(glfwwindow, GLFW_KEY_0) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(glfwwindow, GLFW_KEY_9) == GLFW_PRESS)
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
}

void resizeCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	screenWidth = width;
	screenHeight = height;
}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}