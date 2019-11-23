#pragma once

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"

//luz direccional
DirectionalLight mainLight, mainLight0;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS], pointLights0[1];
SpotLight spotLights[MAX_SPOT_LIGHTS];


float aIntensity = 0.2f, dIntensity = 0.5f;


void LoadLight(unsigned int & pointLightCount, unsigned int& pointLightCount0, unsigned int& spotLightCount) {

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
		aIntensity, dIntensity,
		-13.0f, 3.75f, -8.7242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[3] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-13.0f, 3.75f, -7.0242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[4] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-13.0f, 3.05f, -6.6242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[5] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-13.0f, 3.05f, -4.9242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[6] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-13.0f, 2.35f, -4.5242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[7] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-13.0f, 2.35f, -2.8242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[8] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-7.0f, 3.75f, -8.7242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[9] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-7.0f, 3.75f, -7.0242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[10] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-7.0f, 3.05f, -6.6242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[11] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-7.0f, 3.05f, -4.9242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[12] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
		-7.0f, 2.35f, -4.5242f,
		0.5f, 0.8f, 0.1f);
	pointLightCount++;
	pointLights[13] = PointLight(0.8984f, 0.6875f, 0.4023f,
		aIntensity, dIntensity,
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
}