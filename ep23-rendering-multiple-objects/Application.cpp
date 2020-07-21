// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"
float leftProj = 0.0f;
float rightProj = 960.0f;
float bottomProj = 0.0f;
float topProj = 540.0f;
float zNear = -1.0f;
float zFar = 1.0f;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key) {
        case GLFW_KEY_S:
            zFar -= 1.0f;
            break;
        case GLFW_KEY_W:
            zFar += 1.0f;
            break;
        case GLFW_KEY_A:
            leftProj += 10.0f;
            break;
        case GLFW_KEY_D:
            leftProj -= 10.0f;
            break;
    }
}

GLFWwindow* InitWindow()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( 960, 540, "Tutorial 02 - Red triangle", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return nullptr;

    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return nullptr;
    }

    std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

int main( void )
{
    GLFWwindow* window = InitWindow();
    if (!window)
        return -1;

    float positions[] = {
      -50.0f, -50.0f, 0.0f, 0.0f, // 0
       50.0f, -50.0f, 1.0f, 0.0f, // 1
       50.0f,  50.0f, 1.0f, 1.0f, // 2
      -50.0f,  50.0f, 0.0f, 1.0f  // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall( glEnable(GL_BLEND) );
    GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    glfwSetKeyCallback(window, key_callback);
    {
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        IndexBuffer ib(indices, 6);

        // projection matrix

        // view matrix
        glm::mat4 ident = glm::mat4(1.0f);
        glm::vec3 trvec = glm::vec3(0, 0, 0);
        glm::mat4 view = glm::translate(ident, trvec);

        VertexBufferLayout layout;
        layout.AddFloat(2);
        layout.AddFloat(2);

        va.AddBuffer(vb, layout);

        Shader shader("/home/ali/Documents/thecherno_opengl/ep23-rendering-multiple-objects/res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.8f, 1.0f);

        Texture texture("/home/ali/Documents/thecherno_opengl/ep23-rendering-multiple-objects/res/textures/phone.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        float red = 0.0f;
        float step = 0.05f;

        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();


        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        do {
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();
            glm::mat4 proj = glm::ortho(leftProj, rightProj, bottomProj, topProj, zNear, zFar);
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

            // increment red
            if (red < 0.0f || red > 1.0f)
                step *= -1.0;
            red += step;

        } // Check if the ESC key was pressed or the window was closed
        while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                glfwWindowShouldClose(window) == 0 );
    }

    // Close OpenGL window and terminate GLFW
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}

