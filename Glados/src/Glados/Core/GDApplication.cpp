#include "gladospch.h"
#include "GDApplication.h"
#include "Glados/Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include "Renderer.h"

namespace Glados {

	Application::Application(unsigned int width, unsigned int height, const std::string& title)
	{
        Renderer::Init();
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	}

	Application::~Application()
	{
        delete window;
	}

	void Application::Run()
	{
        /* Create a windowed mode window and its OpenGL context */
        // width, height, name, fullscreen, ...
        if (!window)
        {
            // close the library
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

        if (glewInit() != GLEW_OK)
            GD_CORE_ERROR("Error: GLEW_NOT_OK");

        GD_CORE_INFO(glGetString(GL_VERSION));

        Renderer renderer;

		while (!glfwWindowShouldClose(window))
		{
            /* Render here */

            // Start the Dear ImGui frame
            //ImGui_ImplOpenGL3_NewFrame();
            //ImGui_ImplGlfw_NewFrame();
            //ImGui::NewFrame();


            // TODO: implement layers
            //if (currentTest)
            //{
            //    if (!ImGui::IsAnyWindowFocused())
            //    {
            //    }
            //    currentTest->OnUpdate(0.0f);
            //    currentTest->OnRender();

            //    ImGui::Begin("Test");
            //    if (currentTest != testMenu && ImGui::Button("<-"))
            //    {
            //        delete currentTest;
            //        currentTest = testMenu;
            //    }
            //    currentTest->OnImGuiRender();
            //    ImGui::End();
            //}

            //// imgui rendering
            //ImGui::Render();
            //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            // resets color of window
            renderer.Clear();
		}
	}

}