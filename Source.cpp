#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/imgui.h"
#include "vendor/imgui_impl_glfw.h"
#include "vendor/imgui_impl_opengl3.h"

#include "TestClearColor.h"

int main(int argc, char** argv)
{
	GLFWwindow* window;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	window = glfwCreateWindow(640, 480, "", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// create context
	glfwMakeContextCurrent(window);

	// sync max fps to monitor refresh (vsync)
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {

	}

	{
		/*
		// 4 vertexes for draw square
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
		};

		// what vertexes use to draw a 2 triangles from vertex buffer
		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3
		};

		// setup blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		VertexArray va;

		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);

		va.addBuffer(vb, layout);

		IndexBuffer ib(indices, 6);
		
		// ortographic projection matrix (pixel mapped)
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		// view matrix
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		Shader shader("resources/shaders/Basic.shader");
		shader.bind();

		Texture texture("resources/textures/avatar.png");
		texture.bind();
		shader.setUniform1i("u_texture", 0);

		// unbind everything
		va.unbind();
		shader.unbind();
		vb.unbind();
		ib.unbind();
		*/
		Renderer renderer;


		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		ImGui::StyleColorsDark();

		/*
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);
		*/

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->registerTest<test::ClearColor>("Clear color");

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			renderer.clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest) {
				currentTest->onUpdate(0.0f);
				currentTest->onRender();

				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-")) {
					delete currentTest;
					
					currentTest = testMenu;
				}

				currentTest->onImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// swap buffers
			glfwSwapBuffers(window);
			// poll events
			glfwPollEvents();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

/*


			{
				// model matrix
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				// resulting model view projection matrix (reverse because of opengl column major matrix handling)
				glm::mat4 mvp = proj * view * model;
				// set proj matrix
				shader.bind();
				shader.setUniformMat4f("u_mvp", mvp);

				renderer.draw(va, ib, shader);
			}

			{
				// model matrix
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				// resulting model view projection matrix (reverse because of opengl column major matrix handling)
				glm::mat4 mvp = proj * view * model;
				// set proj matrix
				shader.bind();
				shader.setUniformMat4f("u_mvp", mvp);

				renderer.draw(va, ib, shader);
			}

			{
				ImGui::Begin("Translation");
				ImGui::Text("Matrix A");
				ImGui::SliderFloat("A X", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat("A Y", &translationA.y, 0.0f, 540.0f);
				ImGui::Text("Matrix B");
				ImGui::SliderFloat("B X", &translationB.x, 0.0f, 960.0f);
				ImGui::SliderFloat("B Y", &translationB.y, 0.0f, 540.0f);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
*/