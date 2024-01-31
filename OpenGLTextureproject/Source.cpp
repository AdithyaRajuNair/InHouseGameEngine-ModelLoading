#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/ext.hpp>

#include <Shader_s.h>
#include <Camera.h>
#include <Model.h>
#include <GameObj.h>

#include <stb_image.h>
#include <vector>
#include <iostream>

char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)				\
	(glfwGetKey(WINDOW, KEY) ?				\
	 (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :	\
	 (keyOnce[KEY] = false))

void framebuffersizecallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, int GOAsize, GameObject gameobjarray[], int SelObj);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float rval = 0.0f;
float gval = 0.0f;
float bval = 0.0f;

int selectedGameObj = 0;

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

    GameObject bag("Models/bag_model/backpack.obj", true);
    GameObject cup("Models/cup/cup.obj", false);
    GameObject skull("Models/basecharacter/brideskull.obj", false);

    GameObject GameObjArray[] = {bag,cup,skull};

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    

    while (!glfwWindowShouldClose(window))
    {

        float currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        processInput(window, sizeof(GameObjArray) / sizeof(GameObject), GameObjArray, selectedGameObj);

        // render
        glClearColor(rval, gval, bval, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render container

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //cout << sizeof(GameObjArray) << " " << sizeof(GameObject) << endl;
        for (int i = 0; i<sizeof(GameObjArray)/sizeof(GameObject); i++)
        {
            GameObjArray[i].transform(ourShader);
            GameObjArray[i].draw(ourShader);
        }

        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, int GOAsize, GameObject gameobjarray[], int SelObj)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, 1); }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)  camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)  camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)  camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)  camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { gameobjarray[SelObj].tvecm[1] = gameobjarray[SelObj].tvecm[1] + 0.03f; }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { gameobjarray[SelObj].tvecm[1] = gameobjarray[SelObj].tvecm[1] - 0.03f; }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { gameobjarray[SelObj].tvecm[0] = gameobjarray[SelObj].tvecm[0] + 0.03f; }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { gameobjarray[SelObj].tvecm[0] = gameobjarray[SelObj].tvecm[0] - 0.03f; }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) { gameobjarray[SelObj].tvecm[2] = gameobjarray[SelObj].tvecm[2] + 0.03f; }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { gameobjarray[SelObj].tvecm[2] = gameobjarray[SelObj].tvecm[2] - 0.03f; }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { gameobjarray[SelObj].xaxisanglem = gameobjarray[SelObj].xaxisanglem + 1.0f; }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { gameobjarray[SelObj].xaxisanglem = gameobjarray[SelObj].xaxisanglem - 1.0f; }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { gameobjarray[SelObj].yaxisanglem = gameobjarray[SelObj].yaxisanglem + 1.0f; }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { gameobjarray[SelObj].yaxisanglem = gameobjarray[SelObj].yaxisanglem - 1.0f; }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { gameobjarray[SelObj].zaxisanglem = gameobjarray[SelObj].zaxisanglem + 1.0f; }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { gameobjarray[SelObj].zaxisanglem = gameobjarray[SelObj].zaxisanglem - 1.0f; }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { gameobjarray[SelObj].scalem = gameobjarray[SelObj].scalem + 0.01f; }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS and glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { gameobjarray[SelObj].scalem = gameobjarray[SelObj].scalem - 0.01f; }
    if (glfwGetKeyOnce(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        selectedGameObj++;
        if (selectedGameObj >= GOAsize) selectedGameObj = 0;
    }
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