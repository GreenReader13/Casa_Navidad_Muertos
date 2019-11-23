#pragma once


#include "Texture.h"
#include "Skybox.h"

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

Texture TV_T[50];


//Skybox
Skybox* currentSkybox;
Skybox skybox, skybox0;


//Variables animacion
int ima = 0;
const int frames = 62;

void load_tv() {

	std::string ruta;

	for (int i = 0; i < frames; i++) {

		ruta = "Textures/v" + std::to_string(i + 1) + ".tga";
		const char* r = ruta.c_str();
		TV_T[i] = Texture(r);
		TV_T[i].LoadTextureA();
	}
}

void LoadTexture() {
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

	load_tv();
}

void LoadSkybox() {
	// Skybox
	std::vector<std::string> skyboxFaces;
	// Cargar texturas en orden
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_ft.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_bk.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_dn.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_up.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_rt.tga");
	skyboxFaces.push_back("Textures/ame_fade/fadeaway_lf.tga");

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
}