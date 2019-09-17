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
#include "Quad.h"

void processInput(GLFWwindow *window);
void resizeCallBack(GLFWwindow* window, int width, int height);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
int temp1 = 0;
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

	Shader pointsShader("pointsShader.vert", "pointsShader.frag");
	Shader selectedShader("selectedShader.vert", "selectedShader.frag");

	Vector3 points[] = { 
		{0,0.1,0.0},
		{0.5,0.1,0.0},
	};

	Vector3 searchPoints[] = { 
		{0,0.1,0.0},
		{0.5,0.1,0.0},
	};

	unsigned int VBO1, VAO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4, &points[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int VBO2, VAO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4, &searchPoints[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!window.close()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window.getWindow());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw
		pointsShader.use();
		glBindVertexArray(VAO1);
		glPointSize(8.0f);
		glDrawArrays(GL_POINTS, 0, 2);

		selectedShader.use();
		glBindVertexArray(VAO2);
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 0, 2);

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