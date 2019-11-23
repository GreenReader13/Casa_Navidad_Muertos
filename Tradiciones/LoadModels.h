#pragma once

//Modelos

#include "Model.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Material.h"
#include "Sphere.h"

//Lista de modelos
std::vector<Mesh*> meshList;


// Objetos
Sphere sp = Sphere(0.5, 20, 20);	//Esfera

//Modelos
Model House_M;
Model Puerta_M;
Model Intrp_M;
Model basket;
Model toilet;
Model sign;

Model TV_M;
Model sofa;
Model go_M;

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

//piolin
Model cabeza_M;
Model torso_M;
Model brazoL_M;
Model brazoR_M;
Model piernaL_M;
Model piernaR_M;

void LoadModel() {
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

	go_M = Model();
	go_M.LoadModel("Models/go/go.obj");
	//piolin
	cabeza_M = Model();
	cabeza_M.LoadModel("Models/pio/cabeza.obj");
	torso_M = Model();
	torso_M.LoadModel("Models/pio/cuerpo.obj");
	brazoL_M = Model();
	brazoL_M.LoadModel("Models/pio/brazo.obj");
	brazoR_M = Model();
	brazoR_M.LoadModel("Models/pio/brazo_R.obj");
	piernaL_M = Model();
	piernaL_M.LoadModel("Models/pio/pierna.obj");
	piernaR_M = Model();
	piernaR_M.LoadModel("Models/pio/pierna_R.obj");
}