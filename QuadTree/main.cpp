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

	float width = tree.getBound()->getWidth();
	float height = tree.getBound()->getHeight();
	float length = tree.getBound()->getLength();
	vec3 start = tree.getBound()->getTopLeft();
	vec3 end = tree.getBound()->getBottomRight();
	float size = 0.5f;
	float delta = 1.0f;

	for (float z = start.z; z > end.z; z-= delta) {
		for (float y = start.y; y > end.y; y-= delta) {
			for (float x = start.x; x < end.x; x += delta) {
				vec3 p1, p2, p3;
				p1 = vec3(x-size, y - size, z);
				p2 = vec3(x, y+size, z);
				p3 = vec3(x+size, y-size, z);
				Triangle t(p1, p2, p3);
				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				tree.insert(t);
			}
		}
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

		selectedShader.use(model, view, projection);
		VAO2.bind();
		glDrawArrays(GL_TRIANGLES, 0, selectedPoints.size());
		selectedShader.unuse();

		pointsShader.use(model, view, projection);
		VAO1.bind();
		glDrawArrays(GL_TRIANGLES, 0, points.size());
		pointsShader.unuse();

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