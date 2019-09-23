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

	tree.insert(nanosuit);

	while (!window.close()) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window.getWindow());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		VertexArray VAO2;
		float* ptr = (selectedPoints.size() == 0) ? NULL : &selectedPoints[0].x;
		VertexBuffer VBO2(ptr, selectedPoints.size() * 12);
		VertexBufferLayout layout;
		layout.push<float>(3);
		VAO2.addBuffer(VBO2, layout);

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

		if (renderLines) {
			lineShader.use(model, view, projection);
			for (Line& l : lines) {
				l.draw();
			}
			lineShader.unuse();
		}

		//draw model
		modelShader.use(model, view, projection);
		nanosuit.draw(modelShader);
		modelShader.unuse();
		window.swapBuffers();
		window.getEvents();
	}
	return 0;
}