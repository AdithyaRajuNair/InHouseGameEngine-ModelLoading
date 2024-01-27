#ifndef GameObjClass
#define GameObjClass

#include <Shader_s.h>
#include <Camera.h>
#include <Model.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

class GameObject
{
public: 
	string path;
	unsigned int shaderID;
	glm::mat4 GOmodelmat = glm::mat4(1.f);
	Model ourModel;
	GameObject()
	{

	}
	GameObject(const std::string& modelpath) : path(modelpath), ourModel(modelpath)
	{
		/*path = modelpath;
		ourModel = Model(path);*/
	}

	void transform(glm::vec3 tvec, glm::vec3 svec, float angle, glm::vec3 rvec)
	{
		GOmodelmat = glm::translate(GOmodelmat, tvec);
		GOmodelmat = glm::scale(GOmodelmat, svec);
		GOmodelmat = glm::rotate(GOmodelmat, glm::radians(angle), rvec);
	}


	void draw(Shader ourShader) 
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it right
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);
	}
};

#endif