/**
* @file main_code.cpp
* @brief Proyecto Computación Gráfica 2020-1
*
* Casa con dos cuartos
* Primer cuarto con temática celebración 2 de Noviembre en México
* Segundo cuarto con temética celebración de Navidad en México
*/

#pragma comment(lib, "winmm.lib")

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										LIBRERIAS A UTILIZAR														///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>		//Entrada y salida de datos
#include <vector>
#include <fstream>
#include <ctime>

#include<Windows.h>
#include<mmsystem.h>

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
#define DIS_UFO 1.0f
#define MIN_DIS_SWITCH 2.0f
#define MIN_DIS_DOOR 5.0f
#define MAX_DOOR_ROT 90.0f
#define MAX_SWITCH_ROT 45.0f
#define MAX_PINATA_ROT 45.0f

const float toRadians = 3.14159265f / 180.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//Crear objeto para contexto de la ventana
Window mainWindow;

//Cámaras
Camera *currentCamera;
Camera camera00;
Camera camera01;
Camera camera02;
Camera camera03;
Camera camera04;

//luz direccional
DirectionalLight mainLight, mainLight0;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS], pointLights0[1];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Skybox
Skybox* currentSkybox;
Skybox skybox, skybox0;

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
Texture flame1Texture;
Texture flame2Texture;
Texture pp1Texture;
Texture pp2Texture;
Texture pp3Texture;
Texture pp4Texture;
Texture pp5Texture;
Texture pp6Texture;
Texture rugrats;
Texture f1;
Texture f2;

//Materiales
Material Material_metalico;
Material Material_brillante;
Material Material_opaco;

//Modelos
Model House_M;
Model Puerta_M;
Model Intrp_M;
Model basket;
Model toilet;
Model sign;

Model TV_M;
Model sofa;

Model tree;
Model santa;
Model gift00;
Model gift01;
Model gift02;
Model star;
Model ufo;
Model noche_M;
Model cor_M;

Model christmas04;
Model christmas05;
Model christmas06;

Model luz_arm;
Model mesa_M;
Model album_M;
Model cup;
Model cup2;
Model coca_M;
Model cazuela_M;
Model pizza_M;
Model pan_M;
Model pan_muerto_M;
Model choc_M;
Model cuernito_M;
Model chupe_M;
Model semp_M;
Model sugar_sk_M;
Model sugar_sk1_M;
Model vela_M;
Model pin_M;
Model nac_M;
Model agua_M;
Model piolin_M;

// Variables de objetos
int flama = 0;
bool cambF = false;
bool stc = true;
float aIntensity = 0.2f, dIntensity = 0.5f;

glm::vec3 tablePos = glm::vec3(-10.0f, 0.01f, -8.0f);
glm::vec3 basketPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 chupePos = glm::vec3(2.25f, 2.375, -0.4f);
glm::vec3 gift00Pos = glm::vec3(11.0f, 0.05f, -10.5f);

glm::vec3 doorPos00 = glm::vec3(0.2f, 0.01f, 12.0f);
glm::vec3 doorPos01 = glm::vec3(-4.0f, 0.01f, 7.8f);
glm::vec3 doorPos02 = glm::vec3(-4.0f, 0.01f, -11.2f);

glm::vec3 switchPos00 = glm::vec3(7.31f, 4.738f, -15.75f);
glm::vec3 switchPos01 = glm::vec3(-5.69f, 4.738f, -15.75f);
glm::vec3 switchPos02 = glm::vec3(-4.242f, 4.738f, 9.0f);
glm::vec3 switchPos03 = glm::vec3(4.81f, 4.738f, 11.76f);
glm::vec3 switchPos04 = glm::vec3(-9.6f, 4.375f, -6.0f);
glm::vec3 switchPos05 = glm::vec3(10.367175f, 2.500000f, 3.511425f);

glm::vec3 spotPos00 = glm::vec3(6.0f, 12.0f, -1.75f);
glm::vec3 spotPos01 = glm::vec3(-10.0f, 12.0f, 8.5f);
glm::vec3 spotPos02 = glm::vec3(-10.0f, 12.0f, -5.0f);
glm::vec3 spotPos03 = glm::vec3(-10.0f, 12.0f, -13.0f);
glm::vec3 spotPos04 = glm::vec3(-10.0f, 4.375f, -6.0f);
glm::vec3 spotPos05 = glm::vec3(10.367175f, 2.500000f, 3.211425f);

const std::string delimiter = ",";
std::string s = "";

bool dayState = true;
bool playSound = true;
bool playDoor00 = false, playDoor01 = false, playDoor02 = false, playDoor03 = false;
bool switchState00 = false, switchState01 = false, switchState02 = false, switchState03 = false, switchState04 = false, switchState05 = false;
bool doorState00 = false, doorState01 = false, doorState02 = false;
bool chupeState = false;
bool pinataState = false;

float doorRot00 = 0.0f,
doorRot01 = 0.0f,
doorRot02 = 0.0f,
doorRot03 = 0.0f;

float switchRot00 = 0.0f,
switchRot01 = 0.0f,
switchRot02 = 0.0f,
switchRot03 = 0.0f;

float pinataRot;

float   ufoRot = 0.0f,
ufoRotY = 0.0f,
ufoRotZ = 0.0f,
ufoX = 11.0f,
ufoY = 0.1f,
ufoZ = -10.5f,
ufoPosX = 10.0f,
ufoPosY = 15.0f,
ufoPosZ = -12.0f;

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

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
float distance(glm::vec3 a, glm::vec3 b);
void createObjets(void);
void CreateShaders(void);
void loadAnimationOvni(void);
void resetElementsO(void);
void saveFrameO(void);
void interpolationO(void);
void animateO(void);
void validate(void);
void animate(void);
void flame(void);
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

void createObjets(void) {
	unsigned int indices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	20.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 20.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		20.0f, 20.0f,	0.0f, -1.0f, 0.0f
	};

	GLfloat flameVertices[] = {
		0.0f, -0.5f, -1.0f,		 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		0.0f,  0.5f, -1.0f,		 1.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		0.0f, -0.5f,  1.0f,		 0.0f,  1.0f,	0.0f, -1.0f, 0.0f,
		0.0f,  0.5f,  1.0f,		 1.0f,  1.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj = new Mesh();
	obj->CreateMesh(floorVertices, indices, 32, 6);
	meshList.push_back(obj);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(flameVertices, indices, 32, 6);
	meshList.push_back(obj1);
}

void CreateShaders(void)
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
		//printf("%s\n", str.c_str());
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

	ds = distance(currentCamera->getCameraPosition(), switchPos00);
	(mainWindow.getMouseLeftClick() && ds < MIN_DIS_SWITCH) ? switchState00 = true : switchState00 = false;

	ds = distance(currentCamera->getCameraPosition(), switchPos01);
	(mainWindow.getMouseLeftClick() && ds < MIN_DIS_SWITCH) ? switchState01 = true : switchState01 = false;

	ds = distance(currentCamera->getCameraPosition(), switchPos02);
	(mainWindow.getMouseLeftClick() && ds < MIN_DIS_SWITCH) ? switchState02 = true : switchState02 = false;

	ds = distance(currentCamera->getCameraPosition(), switchPos03);
	(mainWindow.getMouseLeftClick() && ds < MIN_DIS_SWITCH) ? switchState03 = true : switchState03 = false;

	ds = distance(currentCamera->getCameraPosition(), switchPos04);
	(mainWindow.getMouseLeftClick() && ds < MIN_DIS_SWITCH) ? switchState04 = true : switchState04 = false;

	ds = distance(currentCamera->getCameraPosition(), switchPos05);
	(mainWindow.getMouseLeftClick() && ds < MIN_DIS_SWITCH) ? switchState05 = true : switchState05 = false;
	
			
	ds = distance(currentCamera->getCameraPosition(), chupePos);
	if (mainWindow.getMouseLeftClick() && ds < MIN_DIS_SWITCH) 
		chupeState = !chupeState;


	if (mainWindow.getMouseRightClick()) {
		dd = distance(currentCamera->getCameraPosition(), doorPos00);
		if (dd < MIN_DIS_DOOR) {
			doorState00 = !doorState00;
			playDoor00 = true;
		}
		else
		{
			dd = distance(currentCamera->getCameraPosition(), doorPos01);
			if (dd < MIN_DIS_DOOR) {
				doorState01 = !doorState01;
				playDoor01 = true;
			}
			else
			{
				dd = distance(currentCamera->getCameraPosition(), doorPos02);
				if (dd < MIN_DIS_DOOR) {
					doorState02 = !doorState02;
					playDoor02 = true;
				}

			}
		}
	}
	//if (playSound)
	//	PlaySound("sound/Feliz\ Navidad.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	//else
	//	PlaySound(NULL, 0, 0);


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

	if (chupeState) {
		chupePos = basketPos;
		chupePos.y += 0.1;
	}
	else {
		chupePos = glm::vec3(2.25f, 2.375, -0.4f) + tablePos;
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
	if (switchState04) {
		spotPos04.y = 4.375f;
	}
	else {
		spotPos04.y = -50.0f;
	}

	if (switchState05) {
		spotPos05.y = 2.5f;
	}
	else {
		spotPos05.y = -50.0f;
	}

	if (pinataState) {
		if (pinataRot <= MAX_PINATA_ROT)
			pinataRot += 0.5f;
		else {
			pinataRot = MAX_PINATA_ROT;
			pinataState = false;
		}
	}
	else {
		if (pinataRot >= -MAX_PINATA_ROT)
			pinataRot -= 0.5f;
		else {
			pinataRot = -MAX_PINATA_ROT;
			pinataState = true;
		}
	}

	ufoRot += 2.5f;
	ufoRotY += -DIS_UFO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (DIS_UFO - (-DIS_UFO))));

}

void flame(void) {

	flama += 1;
	if (cambF) {
		flame1Texture.UseTexture();
		flama >= 5 ? cambF = false : cambF = true;
	}
	else
	{
		flame2Texture.UseTexture();
		flama >= 5 ? cambF = true : cambF = false;
	}
	flama >= 5 ? flama = 0 : flama = flama;
}

void pant(void) {

	if (stc) {
		f1.UseTexture();
		stc = false;
	}
	else
	{
		f2.UseTexture();
		stc = true;
	}
	flama >= 5 ? flama = 0 : flama = flama;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   													MAIN													    ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

	bool active = PlaySound("sound/Feliz\ Navidad.wav",NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

	// Casteo de rand
	srand(static_cast <unsigned> (time(0)));

	//Creación de contexto
	mainWindow = Window();
	mainWindow.Initialise();

	//Objetos
	createObjets();
	CreateShaders();
	sp.init();
	sp.load();

	camera00 = Camera(glm::vec3(2.642012f, 8.250121f, 35.622520f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	camera01 = Camera(glm::vec3(7.339100f, 3.500000f, 5.515017f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	camera02 = Camera(glm::vec3(-2.124664f, 11.593169f, 8.981001f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	camera03 = Camera(glm::vec3(-13.385676f, 11.375727f, 13.942100f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	camera04 = Camera(glm::vec3(-9.740357f, 3.500000f, -0.022698f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	currentCamera = &camera00;

	//Cargado de texturas
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	yellowTexture = Texture("Textures/yellow.tga");
	yellowTexture.LoadTextureA();
	pinkTexture = Texture("Textures/pink.tga");
	pinkTexture.LoadTextureA();
	blueTexture = Texture("Textures/blue.tga");
	blueTexture.LoadTextureA();
	redTexture = Texture("Textures/red.tga");
	redTexture.LoadTextureA();
	greenTexture = Texture("Textures/green.tga");
	greenTexture.LoadTextureA();
	flame1Texture = Texture("Textures/flama1.tga");
	flame1Texture.LoadTextureA();
	flame2Texture = Texture("Textures/flama2.tga");
	flame2Texture.LoadTextureA();
	pp1Texture = Texture("Textures/1.tga");
	pp1Texture.LoadTextureA();
	pp2Texture = Texture("Textures/2.tga");
	pp2Texture.LoadTextureA();
	pp3Texture = Texture("Textures/3.tga");
	pp3Texture.LoadTextureA();
	pp4Texture = Texture("Textures/4.tga");
	pp4Texture.LoadTextureA();
	pp5Texture = Texture("Textures/5.tga");
	pp5Texture.LoadTextureA();
	pp6Texture = Texture("Textures/6.tga");
	pp6Texture.LoadTextureA();
	rugrats = Texture("Textures/Rugrats.png");
	rugrats.LoadTextureA();
	f1 = Texture("Textures/f1.tga");
	f1.LoadTextureA();
	f2 = Texture("Textures/f2.tga");
	f2.LoadTextureA();
	//------------------ Materiales ----------------------------

	Material_metalico = Material(8.0f, 512);
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//------------------ Modelos ----------------------------

	House_M = Model();
	House_M.LoadModel("Models/casa/House.obj");
	Puerta_M = Model();
	Puerta_M.LoadModel("Models/casa/Puerta.obj");
	Intrp_M = Model();
	Intrp_M.LoadModel("Models/casa/int.obj");
	basket = Model();
	basket.LoadModel("Models/basket/basket.obj");
	toilet = Model();
	toilet.LoadModel("Models/hsdc00/hsdc00.obj");
	toilet = Model();
	toilet.LoadModel("Models/hsdc00/hsdc00.obj");
	TV_M = Model();
	TV_M.LoadModel("Models/tv/Zoll.obj");
	//sign = Model();
	//sign.LoadModel("Models/objSign/objSign.obj"); // Su textura lanza excepción

	tree = Model();
	tree.LoadModel("Models/fir/fir.obj");
	santa = Model();
	santa.LoadModel("Models/Santa/Santa.obj");
	gift00 = Model();
	gift00.LoadModel("Models/box/box.obj");
	gift01 = Model();
	gift01.LoadModel("Models/11563_gift_box_V3/11563_gift_box_V3.obj");
	gift02 = Model();
	gift02.LoadModel("Models/13495_Stack_of_Gifts_v2_L2/13495_Stack_of_Gifts_v2_L2.obj");
	star = Model();
	star.LoadModel("Models/Gold_Star/Gold_Star.obj");
	ufo = Model();
	ufo.LoadModel("Models/ovni/ovni-obj.obj");
	pin_M = Model();
	pin_M.LoadModel("Models/pin/pin.obj");
	nac_M = Model();
	nac_M.LoadModel("Models/nac/nac.obj");
	agua_M = Model();
	agua_M.LoadModel("Models/nac/water.obj");

	sofa = Model();
	sofa.LoadModel("Models/sofa/sofa.obj");

	noche_M = Model();
	noche_M.LoadModel("Models/nb/nb.obj");
	cor_M = Model();
	cor_M.LoadModel("Models/cor/cor.obj");
	
	mesa_M = Model();
	mesa_M.LoadModel("Models/mesa/mesa.obj");
	album_M = Model();
	album_M.LoadModel("Models/album/album.obj");
	vela_M = Model();
	vela_M.LoadModel("Models/candleWhite_obj/candleWhite_obj.obj");
	cup = Model();
	cup.LoadModel("Models/12187_Cupcake_v1_L3/12187_Cupcake_v1_L3.obj");
	cup2 = Model();
	cup2.LoadModel("Models/12188_Cupcake_v1_L3/12188_Cupcake_v1_L3.obj");
	coca_M = Model();
	coca_M.LoadModel("Models/Cup/cupOBJ.obj");
	cazuela_M = Model();
	cazuela_M.LoadModel("Models/13561_Shrimp_Sausage_Jambalaya_v1_L1/13561_Shrimp_Sausage_Jambalaya_v1_L1.obj");
	pizza_M = Model();
	pizza_M.LoadModel("Models/13917_Pepperoni_v2_l2/13917_Pepperoni_v2_l2.obj");
	pan_M = Model();
	pan_M.LoadModel("Models/Bread/Bread.obj");
	choc_M = Model();
	choc_M.LoadModel("Models/chocolate/choco.obj");
	cuernito_M = Model();
	cuernito_M.LoadModel("Models/Croissant/Croissant.obj");

	chupe_M = Model();
	chupe_M.LoadModel("Models/BottleN210418/BottleN210418.obj");
	semp_M = Model();
	semp_M.LoadModel("Models/semp/semp.obj");
	pan_muerto_M = Model();
	pan_muerto_M.LoadModel("Models/pan/pan_muerto.obj");

	sugar_sk_M = Model();
	sugar_sk_M.LoadModel("Models/skull/sugar_skulls_v1.obj");
	sugar_sk1_M = Model();
	sugar_sk1_M.LoadModel("Models/skull/sugar_skulls_v2.obj");

	luz_arm = Model();
	luz_arm.LoadModel("Models/luz_arm/luz_arm.obj");

	piolin_M = Model();
	piolin_M.LoadModel("Models/pio/pio.obj");

	loadAnimationOvni();

	//------------------ Luces ----------------------------
	unsigned int pointLightCount = 0;
	unsigned int pointLightCount0 = 0;
	unsigned int spotLightCount = 0;
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.2f, 0.2f,
		0.0f, 0.0f, 1.0f);
	mainLight0 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.0001f, 0.0001f,
		0.0f, 0.0f, 1.0f);
	//Luces puntuales
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		4.0f, 4.0f,
		15.0f, 3.53f, 24.0f,
		0.9f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		4.0f, 4.0f,
		-15.0f, 3.53f, 24.0f,
		0.9f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[2] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-13.0f, 3.75f, -8.7242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[3] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-13.0f, 3.75f, -7.0242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[4] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-13.0f, 3.05f, -6.6242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[5] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-13.0f, 3.05f, -4.9242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[6] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-13.0f, 2.35f, -4.5242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[7] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-13.0f, 2.35f, -2.8242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[8] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-7.0f, 3.75f, -8.7242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[9] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-7.0f, 3.75f, -7.0242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[10] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-7.0f, 3.05f, -6.6242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[11] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-7.0f, 3.05f, -4.9242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[12] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-7.0f, 2.35f, -4.5242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[13] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity,dIntensity,
		-7.0f, 2.35f, -2.8242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[14] = PointLight(1.0f, 1.0f, 1.0f,
		4.0f, 4.0f,
		2.0f, 6.0f, -35.0f,
		0.9f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[15] = PointLight(1.0f, 1.0f, 0.0f,
		0.6f, 0.6f,
		12.0f, 9.5f, -12.0f,
		0.9f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights0[1] = PointLight(1.0f, 1.0f, 0.0f,
		0.01f, 0.01f,
		12.0f, 9.5f, -50.0f,
		0.9f, 0.2f, 0.1f);
	pointLightCount0++;

	// glm::vec3(12.0f, 0.05f, -12.0f)

	//Luces tipo spotlight
	//Luz interior 1
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		1.5f, 5.0f,
		6.0f, -50.0f, -1.75f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 2
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.5f, 5.0f,
		-10.0f, -50.0f, 8.5f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 3
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.5f, 5.0f,
		-10.0f, -50.0f, -5.0f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz interior 4
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		1.5f, 5.0f,
		-10.0f, -50.0f, -13.0f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	//Luz OVNI
	spotLights[4] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		-10.0f, -50.0f, -13.0f, // Pos
		0.0f, -1.0f, 0.0f,
		0.9f, 0.2f, 0.1f,
		200.0f);
	spotLightCount++;
	spotLights[5] = SpotLight(1.0f, 0.0f, 1.0f,
		4.0f, 4.0f,
		-10.0f, 4.375f, -6.0f, // Pos
		0.0f, -1.0f, -1.0f,
		1.0f, 0.7f, 0.3f,
		200.0f);
	spotLightCount++;
	spotLights[6] = SpotLight(1.0f, 1.0f, 1.0f,
		4.0f, 4.0f,
		10.367175f, 2.500000f, 3.011425f, // Pos
		0.0f, -1.0f, -1.0f,
		1.0f, 0.7f, 0.3f,
		200.0f);
	spotLightCount++;

	// Skybox
	std::vector<std::string> skyboxFaces;
	// Cargar texturas en orden
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_rt.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_lf.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_dn.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_up.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_bk.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_ft.tga");

	// Skybox
	std::vector<std::string> skyboxFaces0;
	// Cargar texturas en orden
	skyboxFaces0.push_back("Textures/ame_flatrock/flatrock_rt.tga");
	skyboxFaces0.push_back("Textures/ame_flatrock/flatrock_lf.tga");
	skyboxFaces0.push_back("Textures/ame_flatrock/flatrock_dn.tga");
	skyboxFaces0.push_back("Textures/ame_flatrock/flatrock_up.tga");
	skyboxFaces0.push_back("Textures/ame_flatrock/flatrock_bk.tga");
	skyboxFaces0.push_back("Textures/ame_flatrock/flatrock_ft.tga");

	// Inicializar Skybox
	skybox = Skybox(skyboxFaces);
	skybox0 = Skybox(skyboxFaces0);

	currentSkybox = &skybox;

	// Uniforms
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	// Matriz de transformaciones
	glm::mat4 model(1.0f);
	// Matrices auxiliares
	glm::mat4 mat00(1.0f);
	glm::mat4 mat01(1.0f);
	glm::mat4 modelTemp(1.0f);
	// Vectores auxiliares
	glm::vec3 vec00 = glm::vec3(0.0f);
	glm::vec3 vec01 = glm::vec3(0.0f);

	// Manejo del contenido
	while (!mainWindow.getShouldClose()) {
		GLfloat now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		currentCamera->keyControl(mainWindow.getsKeys(), deltaTime);
		currentCamera->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		inputKeyframes(mainWindow.getsKeys());
		animateO();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//Habilitar deteccion de profundidad redibujado
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentSkybox->DrawSkybox(currentCamera->calculateViewMatrix(), projection);

		//Iniciar parámetros de Shader
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		//Luces
		spotLights[0].SetPos(spotPos00);
		spotLights[1].SetPos(spotPos01);
		spotLights[2].SetPos(spotPos02);
		spotLights[3].SetPos(spotPos03);
		spotLights[4].SetPos(glm::vec3(ufoX, ufoY, ufoZ));
		spotLights[5].SetPos(spotPos04);
		spotLights[6].SetPos(spotPos05);

		//Luces shader
		if (dayState) {
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights0, pointLightCount0);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			shaderList[0].SetDirectionalLight(&mainLight0);
		
		}

		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//Transformaciones
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(currentCamera->calculateViewMatrix()));
		glUniform3f(uniformEyePosition, currentCamera->getCameraPosition().x, currentCamera->getCameraPosition().y, currentCamera->getCameraPosition().z);

		//Carga de modelos y transformaciones
		model = glm::mat4(1.0f);
		modelTemp = glm::mat4(1.0f);

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

		// Canasta
		basketPos = currentCamera->getCameraPosition();
		basketPos.x--;
		basketPos.y--;
		basketPos.z--;
		model = glm::mat4(1.0);
		model = glm::translate(model, basketPos);
		model = glm::scale(model, glm::vec3(1.0f) * 0.1f);
		//model = glm::rotate(model, glm::radians(switchRot03), glm::vec3(-1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		basket.RenderModel();

		// Excusado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.457438f, 0.01f, -13.129789));
		model = glm::scale(model, glm::vec3(1.0f) * 0.1f);
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		toilet.RenderModel();

// -------------------------------------------------------------- NAVIDAD --------------------------------------------------------

		// Regalo 1
		model = glm::mat4(1.0);
		model = glm::translate(model, gift00Pos);
		model = glm::scale(model, glm::vec3(1.0f) * 0.05f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gift00.RenderModel();

		// Regalo 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(11.0f, 0.05f, -12.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.05f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gift01.RenderModel();

		// Regalo 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.5f, 0.05f, -11.5f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.01f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gift02.RenderModel();


		// Estrella
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 9.6f, -12.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.05f);
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		star.RenderModel();

		// ---------------------------- ÁRBOL ----------------------------

		model = glm::mat4(1.0);
		modelTemp = model = glm::translate(model, glm::vec3(12.0f, 0.05f, -12.0f));

		// Esferas
		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 8.6f, 0.5f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		yellowTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 7.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pinkTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(-1.0f, 7.0f, 0.5f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		redTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 5.0f, 1.5f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		greenTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(-1.5f, 5.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		blueTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(1.0f, 5.0f, 1.3f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		yellowTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(1.5f, 3.0f, 1.5f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pinkTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(-1.5f, 3.0f, 1.5f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		redTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(1.5f, 3.0f, -1.5f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		greenTexture.UseTexture();
		sp.render();
		// Esferas
		modelTemp = model;
		modelTemp = glm::translate(modelTemp, glm::vec3(-1.5f, 3.0f, -1.5f));
		modelTemp = glm::scale(modelTemp, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		blueTexture.UseTexture();
		sp.render();
		// Enable blending
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::scale(model, glm::vec3(1.0f) * 2.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tree.RenderModel();


		// ---------------------------- SANTA ----------------------------
		model = glm::mat4(1.0);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, -35.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.045f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		santa.RenderModel();

		// ---------------------------- PINATA ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 12.0f, 6.0f));
		model = glm::rotate(model, glm::radians(pinataRot),glm::vec3(12.0f, 10.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.8f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pin_M.RenderModel();

		// ---------------------------- NACIMIENTO ----------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 0.2f, 6.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.3f);
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		nac_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 0.2f, 6.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.3f);
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		agua_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 3.2f, 6.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luz_arm.RenderModel();

		// ---------------------------- OVNI ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(ufoX, ufoY, ufoZ));
		model = glm::rotate(model, glm::radians(ufoRotY), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(ufoRot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f) * 0.01f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ufo.RenderModel();


		// ---------------------------- Pantalla ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.3f, 2.05f, -2.7));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.7f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TV_M.RenderModel();

		// ---------------------------- Fondo ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.295f, 4.1f, -2.7f));
		model = glm::scale(model, glm::vec3(1.0f, 1.78f, 6.34f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pant();
		meshList[1]->RenderMesh();

		// ---------------------------- Rugrats ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.29f, 4.1f, -2.7f));
		model = glm::scale(model, glm::vec3(1.0f, 1.78f, 6.34f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rugrats.UseTexture();
		meshList[1]->RenderMesh();

		// ---------------------------- NOCHE BUENA ----------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 0.01f, 2.5f));
		model = glm::scale(model, glm::vec3(1.0f)*0.33f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		noche_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 0.01f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.33f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		noche_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 0.01f, -8.5f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.33f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		noche_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f, 0.01f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.33f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		noche_M.RenderModel();

		// ---------------------------- Corona ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.6f, 8.7f, -2.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cor_M.RenderModel();

		// ---------------------------- Sofa ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f, 0.01f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 0.5f);
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sofa.RenderModel();

// ----------------------------- MUERTOS COMO DAVID --------------------------------------

				
		// ---------------------------- PAPEL PICADO ----------------------------

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-12.0f, 8.5f, -9.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.5f, 1.5f, 1.0f)*1.5f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pp2Texture.UseTexture();
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-8.0f, 8.5f, -9.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.5f, 1.5f, 1.0f) * 1.5f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pp2Texture.UseTexture();
		meshList[1]->RenderMesh();


		// ---------------------------- MESA ARRIBA ----------------------------
		model = glm::mat4(1.0);
		mat01 = mat00 = model = glm::translate(model, tablePos);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();

		// ---------------------------- Luminaria ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(0.0f, 4.7f, 0.14));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		luz_arm.RenderModel();

		// ---------------------------- CUADRO ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(0.5f, 2.375f, 0.0f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.01f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		album_M.RenderModel();

		// ---------------------------- VELAS ----------------------------
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(3.0f, 2.375f, 0.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(-3.0f, 2.375f, 0.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(3.0f, 2.375f, -0.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(-3.0f, 2.375f, -0.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(3.0f, 1.67f, 1.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(-3.0f, 1.67f, 1.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//

		mat00 = model;
		modelTemp = mat00 =  glm::translate(mat00, glm::vec3(3.0f, 1.67f, 2.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(-3.0f, 1.67f, 2.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(3.0f, 0.97f, 3.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(-3.0f, 0.97f, 3.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(3.0f, 0.97f, 4.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//
		mat00 = model;
		modelTemp = mat00 = glm::translate(mat00, glm::vec3(-3.0f, 0.97f, 4.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f, 4.0f, 1.0f) * 0.015f);
		mat00 = glm::rotate(mat00, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		vela_M.RenderModel();

		modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.65f, 0.0f));
		modelTemp = glm::rotate(modelTemp, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = glm::scale(modelTemp, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTemp));
		flame();
		meshList[1]->RenderMesh();
		//

		// ---------------------------- COCAS ----------------------------

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(0.95f, 2.5f, 0.3f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		cup.RenderModel();

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-0.8f, 2.5f, 0.3f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		cup.RenderModel();



		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(1.45f, 2.377f, 0.7f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		cup2.RenderModel();

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(2.3f, 2.377f, 0.5f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		cup2.RenderModel();

		// ---------------------------- COCA ----------------------------

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(1.6f, 2.81f, 0.0f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		coca_M.RenderModel();

		// ---------------------------- CAZUELA ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-2.0f, 2.375f, 0.2f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.15f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		cazuela_M.RenderModel();


		// ---------------------------- PIZZA ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(1.85f, 1.67, 2.25f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.1f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		mat00 = glm::rotate(mat00, 135.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		pizza_M.RenderModel();

		// ---------------------------- PAN ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-2.16f, 1.66, 2.15f));
		mat00 = glm::rotate(mat00, 335.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		pan_M.RenderModel();

		// ---------------------------- Chocolate ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-1.2f, 1.675, 2.4f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.15f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		choc_M.RenderModel();

		// ---------------------------- Cuernito ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-0.28f, 1.675, 1.7f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		cuernito_M.RenderModel();

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(0.37f, 1.675, 2.4f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		mat00 = glm::rotate(mat00, -30.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		cuernito_M.RenderModel();

		// ---------------------------- Licor ----------------------------
		mat00 = model;
		mat00 = glm::translate(glm::mat4(1.0f), chupePos);
		mat00 = glm::scale(mat00, glm::vec3(1.0f)*0.2f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		chupe_M.RenderModel();

		// ---------------------------- FLOR ----------------------------
		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(2.3f, 0.94f, 3.75f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.125f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		semp_M.RenderModel();

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(2.3f, 0.94f, 4.6f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.125f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		semp_M.RenderModel();

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-2.3f, 0.94f, 3.75f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.125f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		semp_M.RenderModel();

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-2.3f, 0.94f, 4.6f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.125f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		semp_M.RenderModel();

		// ---------------------------- CALAVERAS ----------------------------

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(1.3f, 0.94f, 4.0f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		sugar_sk_M.RenderModel();

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(-1.3f, 0.94f, 4.0f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.05f);
		mat00 = glm::rotate(mat00, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		sugar_sk1_M.RenderModel();

		// ---------------------------- PAN MUERTO ----------------------------

		mat00 = model;
		mat00 = glm::translate(mat00, glm::vec3(0.0f, 0.98f, 4.0f));
		mat00 = glm::scale(mat00, glm::vec3(1.0f) * 0.7f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat00));
		pan_muerto_M.RenderModel();

		// ---------------------------- Pollo ----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.01f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f) * 3.0f);
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piolin_M.RenderModel();


		validate();
		animate();

		//printf("%f % f %f\n",currentCamera->getCameraPosition().x, currentCamera->getCameraPosition().y, currentCamera->getCameraPosition().z);

		glUseProgram(0);
		//SwapBuffer
		mainWindow.swapBuffers();
	}
	// Terminate GLFW
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	if (keys[GLFW_KEY_Z]){
		if (habilitaranimacion < 1)
			reproduciranimacion = 0;
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrameO();
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

	if (keys[GLFW_KEY_1]) {
		dayState = true;
		currentSkybox = &skybox;
	}
	if (keys[GLFW_KEY_2]) {
		dayState = false;
		currentSkybox = &skybox0;
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

	if (keys[GLFW_KEY_F1])
		currentCamera = &camera00;
	if (keys[GLFW_KEY_F2])
		currentCamera = &camera01;
	if (keys[GLFW_KEY_F3])
		currentCamera = &camera02;
	if (keys[GLFW_KEY_F4])
		currentCamera = &camera03;
	if (keys[GLFW_KEY_F5])
		currentCamera = &camera04;

}