/**
* @file main_code.cpp
* @brief Proyecto Computación Gráfica 2020-1
*
* Casa con dos cuartos
* Primer cuarto con temática celebración 2 de Noviembre en México
* Segundo cuarto con temética celebración de Navidad en México
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										LIBRERIAS A UTILIZAR														///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
#include "Sphere.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													Estructuras														///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ufo_frame {
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

}U_FRAME;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												DEFINICIONES														///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define MAX_UFO_FRAMES 250
#define DIS 1.0f
#define MIN_DIS_SWITCH 2.0f
#define MIN_DIS_DOOR 5.0f
#define MAX_DOOR_ROT 120.0f
#define MAX_SWITCH_ROT 45.0f

const float toRadians = 3.14159265f / 180.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//Crear objeto para contexto de la ventana
Window mainWindow;

//Cámaras
Camera currentCamera;
Camera camera01;
Camera camera02;
Camera camera03;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Skybox
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


//Lista de modelos
std::vector<Mesh*> meshList;
//Lista de shaders
std::vector<Shader> shaderList;

// Objetos
Sphere sp = Sphere(0.5, 20, 20);

//Texturas a crear
Texture pisoTexture;
Texture yellowTexture;
Texture pinkTexture;
Texture blueTexture;
Texture redTexture;
Texture greenTexture;

//Materiales
Material Material_metalico;
Material Material_brillante;
Material Material_opaco;

//Modelos
Model House_M;
Model Puerta_M;
Model Intrp_M;
Model tree;
Model santa;
Model christmas00;
Model christmas01;
Model christmas02;
Model christmas03;
Model christmas04;
Model christmas05;
Model christmas06;
Model mesa_M;
Model album_M;
Model cup;
Model ufo;
Model vela_M;
Model dead00;
Model dead01;
Model dead02;
Model dead03;
Model dead04;
Model dead05;
Model dead06;

// Variables de objetos
glm::vec3 tablePos = glm::vec3(-10.0f, 1.5f, -8.0f);

glm::vec3 doorPos00 = glm::vec3(0.2f, 0.01f, 12.0f);
glm::vec3 doorPos01 = glm::vec3(-4.0f, 0.01f, 7.8f);
glm::vec3 doorPos02 = glm::vec3(-4.0f, 0.01f, -11.2f);

glm::vec3 switchPos00 = glm::vec3(7.31f, 4.738f, -15.75f);
glm::vec3 switchPos01 = glm::vec3(-5.69f, 4.738f, -15.75f);
glm::vec3 switchPos02 = glm::vec3(-4.242f, 4.738f, 9.0f);
glm::vec3 switchPos03 = glm::vec3(4.81f, 4.738f, 11.76f);

glm::vec3 spotPos00 = glm::vec3(6.0f, 12.0f, -1.75f);
glm::vec3 spotPos01 = glm::vec3(-10.0f, 12.0f, 8.5f);
glm::vec3 spotPos02 = glm::vec3(-10.0f, 12.0f, -5.0f);
glm::vec3 spotPos03 = glm::vec3(-10.0f, 12.0f, -13.0f);

const std::string delimiter = ",";
std::string s = "";

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

bool playDoor00 = false, playDoor01 = false, playDoor02 = false, playDoor03 = false;
bool switchState00 = false, switchState01 = false, switchState02 = false, switchState03 = false;
bool doorState00 = false, doorState01 = false, doorState02 = false;
bool spotOn00, spotOn01 = false, spotOn02 = false, spotOn03 = false;

float doorRot00 = 0.0f,
doorRot01 = 0.0f,
doorRot02 = 0.0f,
doorRot03 = 0.0f;

float switchRot00 = 0.0f,
switchRot01 = 0.0f,
switchRot02 = 0.0f,
switchRot03 = 0.0f;

float   ufoRot = 0.0f,
ufoRotY = 0.0f,
ufoRotZ = 0.0f,
ufoX = 0.0f,
ufoY = 0.0f,
ufoZ = 0.0f,
ufoPosX = 10.0f,
ufoPosY = 15.0f,
ufoPosZ = -12.0f;

// Variables animacíón ovni

int i_max_steps_ovni = 30;
int i_curr_steps_ovni = 0;

U_FRAME UFOFrame[MAX_UFO_FRAMES];
int FrameIndexO = 0;
bool playO = false;
int playIndexO = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												FIRMAS DE FUNCIONES													///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void inputKeyframes(bool* keys);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													FUNCIONES														///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief Distancia entre dos puntos
* @param a vector inicio
* @param b vector fin
*/
float distance(glm::vec3 a, glm::vec3 b) {
	return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2) + powf(b.z - a.z, 2));
}

void createObjets() {
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
				UFOFrame[FrameIndexO].posX = val;
				break;
			case 2:
				UFOFrame[FrameIndexO].posY = val;
				break;
			case 3:
				UFOFrame[FrameIndexO].posZ = val;
				break;
			case 4:
				UFOFrame[FrameIndexO].rotY = val;
				break;
			case 5:
				UFOFrame[FrameIndexO].rotZ = val;
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

	ufoX = UFOFrame[0].posX;
	ufoY = UFOFrame[0].posY;
	ufoZ = UFOFrame[0].posZ;

	ufoRotY = UFOFrame[0].rotY;
	ufoRotZ = UFOFrame[0].rotZ;

}

void saveFrameO(void) {

	printf("frameindex %d\n", FrameIndexO);

	std::ofstream out;
	out.open("animation/ovni.txt", std::ofstream::out | std::ofstream::app);
	s = std::to_string(FrameIndexO++) + "," + std::to_string(ufoX) + "," + std::to_string(ufoY) + "," + std::to_string(ufoZ) + "," + std::to_string(ufoRotY) + "," + std::to_string(ufoRotZ) + "\n";
	out << s;
	out.close();

}

void interpolationO(void) {

	UFOFrame[playIndexO].incX = (UFOFrame[playIndexO + 1].posX - UFOFrame[playIndexO].posX) / i_max_steps_ovni;
	UFOFrame[playIndexO].incY = (UFOFrame[playIndexO + 1].posY - UFOFrame[playIndexO].posY) / i_max_steps_ovni;
	UFOFrame[playIndexO].incZ = (UFOFrame[playIndexO + 1].posZ - UFOFrame[playIndexO].posZ) / i_max_steps_ovni;

	UFOFrame[playIndexO].rotYinc = (UFOFrame[playIndexO + 1].rotY - UFOFrame[playIndexO].rotY) / i_max_steps_ovni;
	UFOFrame[playIndexO].rotZinc = (UFOFrame[playIndexO + 1].rotZ - UFOFrame[playIndexO].rotZ) / i_max_steps_ovni;

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
			ufoX += UFOFrame[playIndexO].incX;
			ufoY += UFOFrame[playIndexO].incY;
			ufoZ += UFOFrame[playIndexO].incZ;

			ufoRotY += UFOFrame[playIndexO].rotYinc;
			ufoRotZ += UFOFrame[playIndexO].rotZinc;

			i_curr_steps_ovni++;
		}
	}

}

void validate(void) {

	float ds,dd;

	if (mainWindow.getMouseLeftClick()) {
		ds = distance(currentCamera.getCameraPosition(), switchPos00);
		if(ds < MIN_DIS_SWITCH)
			switchState00 = !switchState00;
		else 
		{
			ds = distance(currentCamera.getCameraPosition(), switchPos01);
			if (ds < MIN_DIS_SWITCH)
				switchState01 = !switchState01;
			else
			{
				ds = distance(currentCamera.getCameraPosition(), switchPos02);
				if (ds < MIN_DIS_SWITCH)
					switchState02 = !switchState02;
				else
				{

					ds = distance(currentCamera.getCameraPosition(), switchPos03);
					if (ds < MIN_DIS_SWITCH)
						switchState03 = !switchState03;
				}

			}
		}
	}

	if (mainWindow.getMouseRightClick()) {
		dd = distance(currentCamera.getCameraPosition(), doorPos00);
		if (dd < MIN_DIS_DOOR) {
			doorState00 = !doorState00;
			playDoor00 = true;
		}
		else
		{
			dd = distance(currentCamera.getCameraPosition(), doorPos01);
			if (dd < MIN_DIS_DOOR) {
				doorState01 = !doorState01;
				playDoor01 = true;
			}
			else
			{
				dd = distance(currentCamera.getCameraPosition(), doorPos02);
				if (dd < MIN_DIS_DOOR) {
					doorState02 = !doorState02;
					playDoor02 = true;
				}

			}
		}
	}
}

void animate(void) {

	if (playDoor00) {
		if (doorState00) {
			if (doorRot00 <= MAX_DOOR_ROT)
				doorRot00 += 1.0f;
			else {
				doorRot00 = MAX_DOOR_ROT;
				playDoor00 = false;
			}
		}
		else {
			if (doorRot00 >= 0.0f)
				doorRot00 -= 1.0f;
			else {
				doorRot00 = 0.0f;
				playDoor00 = false;
			}
		}
	}

	if (playDoor01) {
		if (doorState01) {
			if (doorRot01 <= MAX_DOOR_ROT)
				doorRot01 += 1.0f;
			else {
				doorRot01 = MAX_DOOR_ROT;
				playDoor01 = false;
			}
		}
		else {
			if (doorRot01 >= 0.0f)
				doorRot01 -= 1.0f;
			else {
				doorRot01 = 0.0f;
				playDoor01 = false;
			}
		}
	}

	if (playDoor02) {
		if (doorState02) {
			if (doorRot02 <= MAX_DOOR_ROT)
				doorRot02 += 1.0f;
			else {
				doorRot02 = MAX_DOOR_ROT;
				playDoor02 = false;
			}
		}
		else {
			if (doorRot02 >= 0.0f)
				doorRot02 -= 1.0f;
			else {
				doorRot02 = 0.0f;
				playDoor02 = false;
			}
		}
	}

	if (switchState00) {
		switchRot00 = MAX_SWITCH_ROT;
	}
	else {
		switchRot00 = 0.0f;
	}

	if (switchState03) {
		switchRot03 = MAX_SWITCH_ROT;
	}
	else {
		switchRot03 = 0.0f;
	}
	if (switchState01) {
		switchRot01 = MAX_SWITCH_ROT;
		spotPos03.y = 12.0f;
	}
	else{
		switchRot01 = 0.0f;
		spotPos03.y = -50.0f;
	}
	if (switchState02) {
		switchRot02 = MAX_SWITCH_ROT;
		spotPos01.y = 12.0f;
		spotPos02.y = 12.0f;
	}
	else {
		switchRot02 = 0.0f;
		spotPos01.y = -50.0f;
		spotPos02.y = -50.0f;
	}

	if ((switchState03 && switchState00)|| (!switchState03 && !switchState00)) {
		spotPos00.y = -50.0f;
	}
	else{
		spotPos00.y = 12.0f;
	}

	ufoRot += 2.5f;
	ufoRotY += -DIS + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (DIS - (-DIS))));
}

int main() {

	//Creación de contexto
	mainWindow = Window();
	mainWindow.Initialise();

	//Objetos
	createObjets();
	CreateShaders();
	sp.init();
	sp.load();

	currentCamera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	//Cargado de texturas
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	yellowTexture = Texture("Textures/plain.png");
	yellowTexture.LoadTextureA();
	pinkTexture = Texture("Textures/plain.png");
	pinkTexture.LoadTextureA();
	blueTexture = Texture("Textures/plain.png");
	blueTexture.LoadTextureA();
	redTexture = Texture("Textures/plain.png");
	redTexture.LoadTextureA();
	greenTexture = Texture("Textures/plain.png");
	greenTexture.LoadTextureA();

	//------------------ Materiales ----------------------------
	Material_metalico = Material(8.0f, 512);
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//------------------ Modelos ----------------------------

	House_M = Model();
	House_M.LoadModel("Models/casa/House.obj");
	tree = Model();
	tree.LoadModel("Models/fir/fir.obj");
	santa = Model();
	santa.LoadModel("Models/Santa/Santa.obj");
	mesa_M = Model();
	mesa_M.LoadModel("Models/mesa/table.obj");
	album_M = Model();
	album_M.LoadModel("Models/album/album.obj");
	vela_M = Model();
	vela_M.LoadModel("Models/candleWhite_obj/candleWhite_obj.obj");
	cup = Model();
	cup.LoadModel("Models/Cup/cup\ OBJ.obj");
	ufo = Model();
	ufo.LoadModel("Models/ovni/ovni-obj.obj");
	Puerta_M = Model();
	Puerta_M.LoadModel("Models/casa/Puerta.obj");
	Intrp_M = Model();
	Intrp_M.LoadModel("Models/casa/int.obj");

	loadAnimationOvni();

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

	//Luces tipo spotlight
	//Luz interior 1
	spotLights[0] = SpotLight(0.0f, 1.0f, 0.0f,
		1.5f, 3.0f,
		6.0f, -50.0f, -1.75f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 2
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
		1.5f, 3.0f,
		-10.0f, -50.0f, 8.5f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 3
	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		1.5f, 3.0f,
		-10.0f, -50.0f, -5.0f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 4
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f,
		1.5f, 3.0f,
		-10.0f, -50.0f, -13.0f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;

	// Skybox
	std::vector<std::string> skyboxFaces;
	// Cargar texturas en orden
	skyboxFaces.push_back("Textures/ely_hills/hills_rt.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_lf.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_dn.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_up.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_bk.tga");
	skyboxFaces.push_back("Textures/ely_hills/hills_ft.tga");

	// Inicializar Skybox
	skybox = Skybox(skyboxFaces);

	// Uniforms
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	// Matriz de transformaciones
	glm::mat4 model(1.0);
	// Vectores auxiliares
	glm::mat4 mat00(1.0);
	glm::mat4 mat01(1.0);
	//
	glm::vec3 vec00 = glm::vec3(0.0f);
	glm::vec3 vec01 = glm::vec3(0.0f);

	// Manejo del contenido
	while (!mainWindow.getShouldClose()) {
		GLfloat now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		currentCamera.keyControl(mainWindow.getsKeys(), deltaTime);
		currentCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		inputKeyframes(mainWindow.getsKeys());
		animateO();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//Habilitar deteccion de profundidad redibujado
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(currentCamera.calculateViewMatrix(), projection);

		//Iniciar parámetros de Shader
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		//Luces
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		spotLights[0].SetPos(spotPos00);
		spotLights[1].SetPos(spotPos01);
		spotLights[2].SetPos(spotPos02);
		spotLights[3].SetPos(spotPos03);

		//Luces shader
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//Transformaciones
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(currentCamera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, currentCamera.getCameraPosition().x, currentCamera.getCameraPosition().y, currentCamera.getCameraPosition().z);

		//Carga de modelos y transformaciones
		model = glm::mat4(1.0);

		// ---------------------------- PISO ----------------------------
		model = glm::mat4(1.0);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		model = glm::rotate(model, glm::radians(doorRot00), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 0.01f, 7.8f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(doorRot01), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 0.01f, -11.2f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(doorRot02), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_M.RenderModel();

		// ---------------------------- Interruptores ----------------------------

		// Principal 1
		model = glm::mat4(1.0);
		model = glm::translate(model, switchPos00);
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(switchRot00), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

		// Baño
		model = glm::mat4(1.0);
		model = glm::translate(model, switchPos01);
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(switchRot01), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

		// Secundario
		model = glm::mat4(1.0);
		model = glm::translate(model, switchPos02);
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(switchRot02), glm::vec3(0.0f, 0.0f, 1.0));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

		// Principal 2
		model = glm::mat4(1.0);
		model = glm::translate(model, switchPos03);
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		model = glm::rotate(model, glm::radians(switchRot03), glm::vec3(-1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Intrp_M.RenderModel();

// -------------------------------------------------------------- NAVIDAD --------------------------------------------------------

		// ---------------------------- ÁRBOL ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 0.05f, -12.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		tree.RenderModel();

		// Esferas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		yellowTexture.UseTexture();
		sp.render();
		// ---------------------------- SANTA ----------------------------
		model = glm::mat4(1.0);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, -35.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.045f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		santa.RenderModel();

		// ---------------------------- OVNI ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(ufoX, ufoY, ufoZ));
		model = glm::rotate(model, glm::radians(ufoRotY), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(ufoRot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f) * 0.01f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ufo.RenderModel();

// -------------------------------------------------------------- MUERTOS COMO DAVID --------------------------------------------------------

		// ---------------------------- MESA ARRIBA ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, tablePos);
		mat01 = mat00 = model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f,1.0f,1.0f) * 0.03f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();
		// ---------------------------- MESA ENMEDIO ----------------------------
		mat00 = model = glm::translate(mat00, glm::vec3(2.2f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f) * 0.03f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();
		// ---------------------------- MESA ABAJO ----------------------------
		model = glm::translate(mat00, glm::vec3(2.2f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f) * 0.03f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();

		// ---------------------------- CUADRO ----------------------------
		mat00 = model = glm::translate(mat00, glm::vec3(0.0f, 1.57f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.01f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		album_M.RenderModel();

		// ---------------------------- VELAS ----------------------------
		mat00 = model = glm::translate(mat00, glm::vec3(3.0f, -0.7f, 3.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(-1.7f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(-0.4f, 0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(-1.7f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(-0.4f, 0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(-1.7f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();

		mat00 = model = glm::translate(mat00, glm::vec3(0.0f, 0.0f, -6.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(1.7f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(0.4f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(1.7f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(0.4f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();
		mat00 = model = glm::translate(mat00, glm::vec3(1.6f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela_M.RenderModel();

		// ---------------------------- CACAS ----------------------------
		model = glm::translate(mat01, glm::vec3(0.0f, 2.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.05f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cup.RenderModel();

		validate();
		animate();

		printf("%f % f %f\n",ufoX, ufoY, ufoZ);

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
				//printf("\nPresiona 0 para habilitar reproducir de nuevo la animación'\n");
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
			//printf("\nPresiona P para habilitar guardar otro frame'\n");
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

	if (keys[GLFW_KEY_T])
		ufoY = ufoY + 1;
	if (keys[GLFW_KEY_U])
		ufoY = ufoY - 1;
	if (keys[GLFW_KEY_Y])
		ufoZ = ufoZ + 1;
	if (keys[GLFW_KEY_H])
		ufoZ = ufoZ - 1;
	if (keys[GLFW_KEY_G])
		ufoX = ufoX - 1;
	if (keys[GLFW_KEY_J])
		ufoX = ufoX + 1;

}