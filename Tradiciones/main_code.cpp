/**
* @file main_code.cpp
* @brief Proyecto Computación Gráfica 2020-1
*
* Casa con dos cuartos
* Primer cuarto con temática celebración 2 de Noviembre en México
* Segundo cuarto con temética celebración de Navidad en México
*/

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>		//Entrada y salida de datos
#include <vector>
#include <fstream>
#include <ctime>

#include <GL/glew.h>	//Librería para resolver dependencias
#include <GLFW/glfw3.h>	//Librería para creación de ventana y contexto openGL

//operaciones con matrices
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//Modelos
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Material.h"
#include "Model.h"
#include "Texture.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"

//Interaccion
#include "Window.h"
#include "Camera.h"
#include "Skybox.h"

void inputKeyframes(bool* keys);

#define MAX_O_FRAMES 250
#define DIS 1.0f

const float toRadians = 3.14159265f / 180.0f;

//Crear objeto para contexto de la ventana
Window mainWindow;

//Camara
Camera camera;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Skybox
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//Lista de modelos
std::vector<Mesh*> meshList;
//Lista de shaders
std::vector<Shader> shaderList;

//Texturas a crear
Texture pisoTexture;

//Materiales
Material Material_brillante;
Material Material_opaco;

//Modelos
Model House_M;
Model Puerta_M;
Model Intrp_M;
Model tree;
Model mesa_M;
Model album_M;
Model ovni;
Model vela_M;

const std::string delimiter = ",";
std::string s = "";

void piso() {
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	20.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 20.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		20.0f, 20.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj = new Mesh();
	obj->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

float   rotOvni = 0.0f,
rotOvniY = 0.0f,
rotOvniZ = 0.0f,
ovniX = 0.0f,
ovniY = 0.0f,
ovniZ = 0.0f;

// Variables animacíón ovni

int i_max_steps_ovni = 30;
int i_curr_steps_ovni = 0;

typedef struct ovni_frame {
	float posX;
	float posY;
	float posZ;
	float incX;
	float incY;
	float incZ;
	float rotY;
	float rotYinc;
	float rotZ;
	float rotZinc;

}O_FRAME;

O_FRAME OvniFrame[MAX_O_FRAMES];
int FrameIndexO = 0;
bool playO = false;
int playIndexO = 0;

void loadAnimationOvni(void) {

	std::ifstream in;
	std::string str;
	std::string cad = "";
	std::string token;
	size_t pos = 0, i;
	float val;

	in.open("animation/ovni.txt");
	getline(in, str);
	do {
		cad = str;
		i = 0;

		while ((pos = cad.find(delimiter)) != std::string::npos) {
			token = cad.substr(0, pos);
			val = stof(token);
			switch (i) {
			case 0:
				FrameIndexO = val;
				break;
			case 1:
				OvniFrame[FrameIndexO].posX = val;
				break;
			case 2:
				OvniFrame[FrameIndexO].posY = val;
				break;
			case 3:
				OvniFrame[FrameIndexO].posZ = val;
				break;
			case 4:
				OvniFrame[FrameIndexO].rotY = val;
				break;
			case 5:
				OvniFrame[FrameIndexO].rotZ = val;
				break;
			default:
				break;
			}

			cad.erase(0, pos + delimiter.length());
			i++;
		}

		FrameIndexO++;
		printf("%s\n", str.c_str());
	} while (getline(in, str));

	in.close();

}

void resetElementsO(void) {

	ovniX = OvniFrame[0].posX;
	ovniY = OvniFrame[0].posY;
	ovniZ = OvniFrame[0].posZ;

	rotOvniY = OvniFrame[0].rotY;
	rotOvniZ = OvniFrame[0].rotZ;

}

void saveFrameO(void) {

	printf("frameindex %d\n", FrameIndexO);

	std::ofstream out;
	out.open("animacionOvni.txt", std::ofstream::out | std::ofstream::app);
	s = std::to_string(FrameIndexO++) + "," + std::to_string(ovniX) + "," + std::to_string(ovniY) + "," + std::to_string(ovniZ) + "," + std::to_string(rotOvniY) + "," + std::to_string(rotOvniZ) + "\n";
	out << s;
	out.close();

}

void interpolationO(void) {

	OvniFrame[playIndexO].incX = (OvniFrame[playIndexO + 1].posX - OvniFrame[playIndexO].posX) / i_max_steps_ovni;
	OvniFrame[playIndexO].incY = (OvniFrame[playIndexO + 1].posY - OvniFrame[playIndexO].posY) / i_max_steps_ovni;
	OvniFrame[playIndexO].incZ = (OvniFrame[playIndexO + 1].posZ - OvniFrame[playIndexO].posZ) / i_max_steps_ovni;

	OvniFrame[playIndexO].rotYinc = (OvniFrame[playIndexO + 1].rotY - OvniFrame[playIndexO].rotY) / i_max_steps_ovni;
	OvniFrame[playIndexO].rotZinc = (OvniFrame[playIndexO + 1].rotZ - OvniFrame[playIndexO].rotZ) / i_max_steps_ovni;

}

void animateO(void) {
	if (playO) {
		if (i_curr_steps_ovni >= i_max_steps_ovni) { //end of animation between frames?
			playIndexO++;
			if (playIndexO > FrameIndexO - 2) {	//end of total animation?
				printf("termina animacion Ovni\n");
				playIndexO = 0;
				playO = false;
			}
			else { //Next frame interpolations
				i_curr_steps_ovni = 0; //Reset counter
								  //Interpolation
				interpolationO();
			}
		}
		else {
			//Draw animation
			ovniX += OvniFrame[playIndexO].incX;
			ovniY += OvniFrame[playIndexO].incY;
			ovniZ += OvniFrame[playIndexO].incZ;

			rotOvniY += OvniFrame[playIndexO].rotYinc;
			rotOvniZ += OvniFrame[playIndexO].rotZinc;

			i_curr_steps_ovni++;
		}
	}

}

void animate(void) {

	rotOvniY = -DIS + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (DIS - (-DIS))));
	rotOvni += 2.5f;
}

int main() {

	//Creación de contexto
	mainWindow = Window();
	mainWindow.Initialise();

	//Objetos
	piso();

	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	//Cargado de texturas
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	//------------------ Materiales ----------------------------
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//------------------ Modelos ----------------------------

	House_M = Model();
	House_M.LoadModel("Models/casa/House.obj");
	tree = Model();
	tree.LoadModel("Models/tree/12150_Christmas_Tree_V2_L2.obj");
	mesa_M = Model();
	mesa_M.LoadModel("Models/mesa/table.obj");
	album_M = Model();
	album_M.LoadModel("Models/album/album.obj");
	vela_M = Model();
	vela_M.LoadModel("Models/candleWhite_obj/candleWhite_obj.obj");
	ovni = Model();
	ovni.LoadModel("Models/ovni/ovni-obj.obj");
	Puerta_M = Model();
	Puerta_M.LoadModel("Models/casa/Puerta.obj");
	Intrp_M = Model();
	Intrp_M.LoadModel("Models/casa/int.obj");

	srand(static_cast <unsigned> (time(0)));

	//------------------ Luces ----------------------------
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//Luces puntuales
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		3.0f, 5.53f, -5.0f,
		0.9f, 0.2f, 0.1f);
	pointLightCount++;

	//Luces tipo sptolight
	//Luz interior 1
	spotLights[0] = SpotLight(0.0f, 1.0f, 0.0f,
		1.5f, 3.0f,
		6.0f, 12.0f, -1.75f,
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 2
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
		1.5f, 3.0f,
		-10.0f, 12.0f, 8.5f,
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 3
	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		1.5f, 3.0f,
		-10.0f, 12.0f, -5.0f,
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 4
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f,
		1.5f, 3.0f,
		-10.0f, 12.0f, -13.0f,
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;

	//Skybox
	std::vector<std::string> skyboxFaces;
	//Cargar texturas en orden
	skyboxFaces.push_back("Textures/ely_hills/hills_rt.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_lf.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_dn.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_up.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_bk.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_ft.tga");

	//Inicializar Skybox
	skybox = Skybox(skyboxFaces);

	//Uniforms
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	//Manejo del contenido
	while (!mainWindow.getShouldClose()) {
		GLfloat now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		inputKeyframes(mainWindow.getsKeys());
		animateO();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//Habilitar deteccion de profundidad redibujado
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		//Iniciar parámetros de Shader
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		//Luces

		//Luces shader
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//Transformaciones
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//Matriz de transformaciones
		glm::mat4 model(1.0);

		//Carga de modelos y transformaciones
		model = glm::mat4(1.0);

		// ---------------------------- PISO ----------------------------
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshList[0]->RenderMesh();


		// ---------------------------- CASA ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		House_M.RenderModel();

		// ---------------------------- Puertas ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.2f, 0.01f, 12.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 0.01f, 7.8f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 0.01f, -11.2f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_M.RenderModel();

		// ---------------------------- Interruptores ----------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.31f, 4.738f, -15.75f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.69f, 4.738f, -15.75f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.242f, 4.738f, 9.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.81f, 4.738f, 11.76f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

		// ---------------------------- OVNI ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
		model = glm::translate(model, glm::vec3(ovniX, ovniY, ovniZ));
		model = glm::rotate(model, glm::radians(rotOvniY), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotOvni), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f)*0.03f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ovni.RenderModel();

// ---------------------------------- Navidad ----------------------------

				// ---------------------------- ÁRBOL ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.0f, 0.0f, -13.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.05f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tree.RenderModel();

		// ---------------------------------- All Hollows Day ----------------------------

				// ---------------------------- MESA ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.5f, 1.5f, 8.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.03f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();

		// ---------------------------- CUADRO ----------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.4f, 3.08f, 8.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.01f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		album_M.RenderModel();

		// ---------------------------- VELAS ----------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 3.08f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 3.08f, 6.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();


		animate();

		glUseProgram(0);
		//SwapBuffer
		mainWindow.swapBuffers();
	}
	// Terminate GLFW
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (playO == false && (FrameIndexO > 1))
			{
				resetElementsO();
				//First Interpolation				
				interpolationO();
				playO = true;
				playIndexO = 0;
				i_curr_steps_ovni = 0;
				reproduciranimacion++;
				printf("\nPresiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				playO = false;
			}
		}
	}
	if (keys[GLFW_KEY_1]){
		if (habilitaranimacion < 1)
			reproduciranimacion = 0;
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrameO();
			printf("\nPresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P]){
		if (reinicioFrame < 1)
			guardoFrame = 0;
	}
	if (keys[GLFW_KEY_0]) {
		loadAnimationOvni();
	}

}