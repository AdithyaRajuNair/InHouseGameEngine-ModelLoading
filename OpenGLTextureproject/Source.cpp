#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <Shader_s.h>
#include <Camera.h>
#include <Model.h>
#include <GameObj.h>

#include <stb_image.h>

#include <iostream>

void framebuffersizecallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float rval = 0.0f;
float gval = 0.6f;
float bval = 0.75f;
float alphaval = 0.00;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* a = glfwGetPrimaryMonitor();

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "HelloWindow", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, framebuffersizecallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    stbi_set_flip_vertically_on_load(true); 

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    Shader ourShader("shader1.vert", "shader1.frag");
    /*
    stbi_set_flip_vertically_on_load(true);
    Model ourModel1("Models/bag_model/backpack.obj");
    stbi_set_flip_vertically_on_load(false);
    Model ourModel2("Models/cup/cup.obj");
    stbi_set_flip_vertically_on_load(false);
    Model ourModel3("Models/basecharacter/brideskull.obj");
    //stbi_set_flip_vertically_on_load(false);
    //Model ourModel4("Models/car/model.obj");  //eyeball
    stbi_set_flip_vertically_on_load(false);
    Model ourModel4("Models/cwire/golden-lion-knocker-edit.obj");
    stbi_set_flip_vertically_on_load(false);
    Model ourModel5("Models/cycle/raptor.obj");
    */

    string str = "Models/bag_model/backpack.obj";
    GameObject bag(str);
    //bag.modelpath = "Models/bag_model/backpack.obj";


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    while (!glfwWindowShouldClose(window))
    {

        float currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        processInput(window);

        // render
        glClearColor(rval, gval, bval, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // render container

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);


        bag.draw(ourShader);
        /*
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it right
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel1.Draw(ourShader);

        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f)); // translate it left
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));	// it's a bit too small for our scene, so scale it up
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("model", model);
        ourModel2.Draw(ourShader);

        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f)); // translate it left
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too small for our scene, so scale it up
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);
        ourModel3.Draw(ourShader);

        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f)); // translate it left
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too small for our scene, so scale it up
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("model", model);
        ourModel4.Draw(ourShader);

        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f)); // translate it left
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too small for our scene, so scale it up
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("model", model);
        ourModel5.Draw(ourShader);
        */

        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, 1); }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { alphaval = alphaval + 0.001f; }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { alphaval = alphaval - 0.001f; }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)  camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)  camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)  camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)  camera.ProcessKeyboard(DOWN, deltaTime);
}

void framebuffersizecallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xOffset = xpos - lastX;
    float yOffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}