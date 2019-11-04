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
Texture plainTexture;

//materiales
Material Material_brillante;
Material Material_opaco;

//Modelos
Model House_M;

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
int main() {

	//Creación de contexto
	mainWindow = Window();
	mainWindow.Initialise();

	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	//Cargado de texturas
	plainTexture = Texture("Textures/plain.png");

	//Materiales
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//Modelos
	House_M = Model();
	House_M.LoadModel("Models/House.obj");

//Luces
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//Luces puntuales
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Luces tipo sptolight
	//linterna
	spotLights[0] = SpotLight(0.8f, 0.8f, 0.8f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de faro
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-4.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	//Skybox
	std::vector<std::string> skyboxFaces;
	//Cargar texturas en orden

	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

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
		//Linterna
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

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

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		House_M.RenderModel();

		glUseProgram(0);
		//SwapBuffer
		mainWindow.swapBuffers();
	}
	// Terminate GLFW
}