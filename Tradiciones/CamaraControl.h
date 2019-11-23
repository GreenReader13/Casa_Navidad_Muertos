#pragma once
//Cámaras

#include "Camera.h"

Camera* currentCamera;
Camera camera00;
Camera camera01;
Camera camera02;
Camera camera03;
Camera camera04;

void createCams() {

	camera00 = Camera(glm::vec3(2.642012f, 8.250121f, 35.622520f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	camera01 = Camera(glm::vec3(13.0f, 4.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 0.0f, 5.0f, 0.5f);
	camera02 = Camera(glm::vec3(-2.124664f, 11.593169f, 8.981001f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	camera03 = Camera(glm::vec3(-13.385676f, 11.375727f, 13.942100f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	camera04 = Camera(glm::vec3(-9.740357f, 3.500000f, -0.022698f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	currentCamera = &camera00;
}