#include "Window.h"

int main() {
	srand((unsigned int)time(0));
	if (!window.isGood()) { glfwTerminate(); return -1; }
	window.setResizeCallBack(resizeCallBack);
	window.setMouseMovementCallBack(mouseCallBack);
	window.setMouseButtonCallBack(mouseButtonCallBack);
	window.setCursor(GLFW_CURSOR_DISABLED);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	points.reserve(spawnCount);

#pragma omp parallel
#pragma omp for
	for (int i = 0; i < spawnCount; i++) {
		float x = random(-treeSize, treeSize);
		float y = random(-treeSize, treeSize);
		float z = random(-treeSize, treeSize);
		tree.insert(vec3(x,y,z));
		points.push_back(vec3(x, y, z));
	}

	VertexArray VAO1;
	VertexBuffer VBO1(&points[0].x, points.size() * 12);
	VertexBufferLayout layout;
	layout.push<float>(3);
	VAO1.addBuffer(VBO1, layout);


	while (!window.close()) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window.getWindow());

		VertexArray VAO2;
		float* ptr = (selectedPoints.size() == 0) ? NULL : &selectedPoints[0].x;
		VertexBuffer VBO2(ptr, selectedPoints.size() * 12);
		VAO2.addBuffer(VBO2, layout);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 model(1.0f);
		mat4 view = camera.GetViewMatrix();
		mat4 projection = glm::perspective(45.0f, (float)screenWidth / screenHeight, 0.1f, 100.0f);

		drawCrossHair();

		treeBoxShader.use(model, view, projection);
		if (renderbox) tree.draw();
		treeBoxShader.unuse();

		pointsShader.use(model, view, projection);
		VAO1.bind();
		glPointSize(3.0f);
		glDrawArrays(GL_POINTS, 0, points.size());
		pointsShader.unuse();

		selectedShader.use(model, view, projection);
		VAO2.bind();
		glPointSize(6.0f);
		glDrawArrays(GL_POINTS, 0, selectedPoints.size());
		selectedShader.unuse();

		lineShader.use(model, view, projection);
		for (Line& l : lines) {
			l.draw();
		}
		lineShader.unuse();

		window.swapBuffers();
		window.getEvents();
	}
	return 0;
}