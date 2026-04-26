
/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture navel;
Texture carro;
Texture farol;
Model Kitt_M;
Texture dadoTexture;

Model Blackhawk_M;
Mesh* peceraCristal;
Mesh* peceraAgua;
Texture texturaCristal;
Texture texturaAgua;

//lo que hice yo
Model cofre;
Model LLanta2;
Model Llanta_M;
Model faro;
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

bool luzFaroOn = true;
bool luzBulboOn = true;
bool key1Pressed = false;
bool key2Pressed = false;

GLfloat pezPosX = 0.0f;
GLfloat pezPosY = -0.8f;

// Dirección inicial del spotlight del pez
GLfloat spotPezDirX = 0.0f;
GLfloat spotPezDirY = -1.0f;
GLfloat spotPezDirZ = 0.0f;
// -------------

//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}






void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};	GLfloat cubo_vertices[] = {
			// front
			//x		y		z		S		T			NX		NY		NZ
			-0.5f, -0.5f,  0.5f,	0.f,  0.0f,			1.0f,	0.0f,	-1.0f,	//0
			0.5f, -0.5f,  0.5f,		1.f,	0.0f,		1.0f,	0.0f,	-1.0f,	//1
			0.5f,  0.5f,  0.5f,		1.f,	1.f,		1.0f,	0.0f,	-1.0f,	//2
			-0.5f,  0.5f,  0.5f,	0.f,	1.f,		1.0f,	0.0f,	-1.0f,	//3
			// right
			//x		y		z		S		T
			0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
			0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
			// back
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
			0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

			// left
			//x		y		z		S		T
			-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

			// bottom
			//x		y		z		S		T
			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
			0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
			 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
			-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

			//UP
			 //x		y		z		S		T
			 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
			  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
			 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}


//..........................
void CrearPecera()
{
	unsigned int cristal_indices[] = {
		// front
		0, 1, 2,  2, 3, 0,
		// back
		4, 5, 6,  6, 7, 4,
		// left
		8, 9, 10,  10, 11, 8,
		// right
		12, 13, 14,  14, 15, 12,
		// bottom
		16, 17, 18,  18, 19, 16
	};

	GLfloat cristal_vertices[] = {
		// front
		//x      y      z       S      T         NX     NY     NZ (Invertidos)
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,     0.0f,  0.0f, -1.0f,  //0
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,     0.0f,  0.0f, -1.0f,  //1
		 0.5f,  0.5f,  0.5f,   1.0f,  1.0f,     0.0f,  0.0f, -1.0f,  //2
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,     0.0f,  0.0f, -1.0f,  //3
		// back
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f,     0.0f,  0.0f,  1.0f,  //4
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,     0.0f,  0.0f,  1.0f,  //5
		 0.5f,  0.5f, -0.5f,   1.0f,  1.0f,     0.0f,  0.0f,  1.0f,  //6
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,     0.0f,  0.0f,  1.0f,  //7
		// left
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f,     1.0f,  0.0f,  0.0f,  //8
		-0.5f, -0.5f,  0.5f,   1.0f,  0.0f,     1.0f,  0.0f,  0.0f,  //9
		-0.5f,  0.5f,  0.5f,   1.0f,  1.0f,     1.0f,  0.0f,  0.0f,  //10
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,     1.0f,  0.0f,  0.0f,  //11
		// right
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,    -1.0f,  0.0f,  0.0f,  //12
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,    -1.0f,  0.0f,  0.0f,  //13
		 0.5f,  0.5f, -0.5f,   1.0f,  1.0f,    -1.0f,  0.0f,  0.0f,  //14
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,    -1.0f,  0.0f,  0.0f,  //15
		 // bottom
		 -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,     0.0f,  1.0f,  0.0f,  //16
		  0.5f, -0.5f,  0.5f,   1.0f,  0.0f,     0.0f,  1.0f,  0.0f,  //17
		  0.5f, -0.5f, -0.5f,   1.0f,  1.0f,     0.0f,  1.0f,  0.0f,  //18
		 -0.5f, -0.5f, -0.5f,   0.0f,  1.0f,     0.0f,  1.0f,  0.0f,  //19
	};

	peceraCristal = new Mesh();
	peceraCristal->CreateMesh(cristal_vertices, cristal_indices, 160, 30); 

	unsigned int agua_indices[] = {
		0, 1, 2,  2, 3, 0
	};

	GLfloat agua_vertices[] = {
		// UP (Agua)
		//x      y      z       S      T         NX     NY     NZ (Invertido hacia abajo)
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,     0.0f, -1.0f,  0.0f,  //0
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,     0.0f, -1.0f,  0.0f,  //1
		 0.5f,  0.5f, -0.5f,   1.0f,  1.0f,     0.0f, -1.0f,  0.0f,  //2
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,     0.0f, -1.0f,  0.0f,  //3
	};

	peceraAgua = new Mesh();
	peceraAgua->CreateMesh(agua_vertices, agua_indices, 32, 6); 
}





int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearPecera();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	navel = Texture("Textures/Image_1.png");
	navel.LoadTextureA();
	dadoTexture = Texture("Textures/agua2.png");
	dadoTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/nave1.obj");
	//mis cosas
	farol = Texture("Textures/farol.png");
	farol.LoadTextureA();
	carro = Texture("Textures/batwheels.png");
	carro.LoadTextureA();
	LLanta2 = Model();
	LLanta2.LoadModel("Models/rueda4.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/antenapez.obj");
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/cuerpopez1.obj");
	cofre = Model();
	cofre.LoadModel("Models/focopez.obj");
	faro = Model();
	faro.LoadModel("Models/faro1.obj");

	texturaCristal = Texture("Textures/crista1.png");
	texturaCristal.LoadTextureA();

	texturaAgua = Texture("Textures/agua2.png");
	texturaAgua.LoadTextureA();




	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija

	//creado por mi 
	//Faros del Coche
	spotLights[1] = SpotLight(
		0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.05f, 0.01f,
		2.5f
	);
	spotLightCount++;

	// Faro coh 2
	spotLights[2] = SpotLight(
		0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.05f, 0.01f,
		2.5f
	);
	spotLightCount++;


	// Configuración de la luz amarilla del helicóptero
	spotLights[3] = SpotLight(
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.005f,
		10.0f
	);
	spotLightCount++;
	spotLights[4] = SpotLight(
		0.0f, 1.0f, 0.0f, // Color Verde
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, // Posición 
		0.0f, -1.0f, 0.0f,// Dirección inicial
		1.0f, 0.02f, 0.005f,
		15.0f
	);
	spotLightCount++;

	pointLights[1] = PointLight(
		1.0f, 1.0f, 1.0f,    // Color Blanco puro
		0.1f, 1.0f,          // Intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,    // Posición inicial
		0.3f, 0.2f, 0.1f     // Atenuación
	);
	pointLightCount++;


	//Luz puntual AZUL para el bulbo del pez (Indice 2)
	pointLights[2] = PointLight(0.0f, 0.0f, 1.0f,
	0.1f, 1.0f, 0.0f, 
	0.0f, 0.0f, 0.3f, 
	0.2f, 0.1f);
	pointLightCount++;


	//Correccion del ejercicio pdeclarando fuera del while 
	glm::mat4 modelauxFaro;
	glm::vec3 posLuzFaroMundo;
	glm::vec4 localLuzFaro = glm::vec4(0.0f, 4.5f, 0.0f, 1.0f);
	GLfloat inclinacionHeli;
	glm::mat4 modelauxHeli;
	glm::vec3 direccionMundoHeli;
	glm::vec4 localLuzHeli = glm::vec4(0.0f, -0.5f, 0.0f, 1.0f);
	glm::vec3 posLuzHeliMundo;
	glm::vec4 localFaroIzq = glm::vec4(0.5f, 1.72f, 0.4f, 1.0f);
	glm::vec4 localFaroDer = glm::vec4(0.58f, 1.72f, -2.3f, 1.0f);

	glm::vec3 direccionFaros = glm::vec3(-1.0f, -0.12f, 0.0f);

	glm::vec3 posFaroIzqMundo;
	glm::vec3 posFaroDerMundo;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		// --- LÓGICA DE TECLADO PARA MOVIMIENTO DEL PEZ ---
				// ZigZag: Ambas teclas avanzan en X, pero alternan en Y (arriba/abajo)
		if (mainWindow.getsKeys()[GLFW_KEY_Q]) { pezPosX -= 0.02f; pezPosY += 0.02f; } // Sube y avanza
		if (mainWindow.getsKeys()[GLFW_KEY_E]) { pezPosX -= 0.02f; pezPosY -= 0.02f; } // Baja y avanza

		if (mainWindow.getsKeys()[GLFW_KEY_Z]) { pezPosX += 0.02f; pezPosY += 0.02f; } // Sube y avanza
		if (mainWindow.getsKeys()[GLFW_KEY_X]) { pezPosX += 0.02f; pezPosY -= 0.02f; } // Baja y avanza

		// Dirección del Spotlight verde (Teclas: Flechas + U y J)
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT])  spotPezDirX -= 0.02f;
		if (mainWindow.getsKeys()[GLFW_KEY_RIGHT]) spotPezDirX += 0.02f;
		if (mainWindow.getsKeys()[GLFW_KEY_U])     spotPezDirY += 0.02f;
		if (mainWindow.getsKeys()[GLFW_KEY_J])     spotPezDirY -= 0.02f;
		if (mainWindow.getsKeys()[GLFW_KEY_UP])    spotPezDirZ -= 0.02f;
		if (mainWindow.getsKeys()[GLFW_KEY_DOWN])  spotPezDirZ += 0.02f;

		//tecla para apagar las luces puntuales del pez y la lampara
		if (mainWindow.getsKeys()[GLFW_KEY_1]) {
			if (!key1Pressed) { luzFaroOn = !luzFaroOn; key1Pressed = true; }
		}
		else { key1Pressed = false; }
		if (mainWindow.getsKeys()[GLFW_KEY_2]) {
			if (!key2Pressed) { luzBulboOn = !luzBulboOn; key2Pressed = true; }
		}
		else { key2Pressed = false; }
		// -------------------------------------------------------------



		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();

		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//las luces de los dos faors
		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		//spotLights[1].SetPos(posActualCoche + offsetFaros); NOTAAAAA: esto lo usare para posicionar en el helicoptero 


		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));



		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		inclinacionHeli = 0.0f;
		if (mainWindow.getsKeys()[GLFW_KEY_I]) {
			inclinacionHeli = -15.0f;  // Se inclina hacia atras
		}
		else if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			inclinacionHeli = 15.0f; // Se inclina hacia adelante
		}


		//l Pez Abisal (Cuerpo)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(pezPosX, pezPosY, -3.0f));
		model = glm::scale(model, glm::vec3(.1f, .1f, .1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;


		//faros
		//posFaroIzqMundo = glm::vec3(modelaux * localFaroIzq);
		//posFaroDerMundo = glm::vec3(modelaux * localFaroDer);
		//spotLights[1].SetFlash(posFaroIzqMundo, direccionFaros);
		//spotLights[2].SetFlash(posFaroDerMundo, direccionFaros);

		model = glm::scale(model, glm::vec3(2.0f, 2.f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		// Antena del Pez
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.6f, 13.3f, -4.6f));
		model = glm::scale(model, glm::vec3(1.8f, 1.8f, 2.3f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Bulbo del pez 
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.12f, 29.5f, 10.42f));
		model = glm::rotate(model, glm::radians(7.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 modelauxBulbo = model;
		glm::vec3 posLuzBulboMundo = glm::vec3(modelauxBulbo * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		if (luzBulboOn) {
			pointLights[2] = PointLight(0.0f, 0.0f, 1.0f, 0.1f, 1.0f, posLuzBulboMundo.x, posLuzBulboMundo.y, posLuzBulboMundo.z, 0.3f, 0.2f, 0.1f);
		}
		else {
			pointLights[2] = PointLight(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, posLuzBulboMundo.x, posLuzBulboMundo.y, posLuzBulboMundo.z, 0.3f, 0.2f, 0.1f);
		}
		glm::vec3 direccionSpotPez = glm::normalize(glm::vec3(spotPezDirX, spotPezDirY, spotPezDirZ));
		spotLights[4].SetFlash(posLuzBulboMundo, direccionSpotPez);
		model = glm::scale(model, glm::vec3(1.7f, 2.f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre.RenderModel();

		// -------------------------------------------------------------


		//helicoptero
		inclinacionHeli = 0.0f;
		if (mainWindow.getsKeys()[GLFW_KEY_I]) { inclinacionHeli = -25.0f; }
		else if (mainWindow.getsKeys()[GLFW_KEY_O]) { inclinacionHeli = 25.0f; }
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getmueveHeli(), 5.0f, 6.0f));
		modelauxHeli = model;
		posLuzHeliMundo = glm::vec3(modelauxHeli * localLuzHeli);
		model = glm::rotate(model, inclinacionHeli * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// Si la luz se inclina hacia el lado contrario, simplemente poner un signo menos al seno:  dirX = -sin(...)
		GLfloat dirX = -sin(inclinacionHeli * toRadians);
		GLfloat dirY = -cos(inclinacionHeli * toRadians);
		GLfloat dirZ = 0.0f;

		direccionMundoHeli = glm::normalize(glm::vec3(dirX, dirY, dirZ));
		spotLights[3].SetFlash(posLuzHeliMundo, direccionMundoHeli);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		navel.UseTexture();
		Blackhawk_M.RenderModel();

		// IMPORTANTE
		// shaderList[0].SetSpotLights(spotLights, spotLightCount);


		// Lámpara Texturizada (Faro)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -0.9f, -30.0f));
		modelauxFaro = model;
		posLuzFaroMundo = glm::vec3(modelauxFaro * localLuzFaro);
		// Revisamos el estado de la tecla 1
		if (luzFaroOn) {
			pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, 0.1f, 1.0f, posLuzFaroMundo.x, posLuzFaroMundo.y, posLuzFaroMundo.z, 0.3f, 0.2f, 0.1f);
		}
		else {
			pointLights[1] = PointLight(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, posLuzFaroMundo.x, posLuzFaroMundo.y, posLuzFaroMundo.z, 0.3f, 0.2f, 0.1f);
		}
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		farol.UseTexture();
		faro.RenderModel();

		// ¡IMPORTANTE PARA LA ARQUITECTURA DEL SHADER!
		// Como estamos calculando posiciones dinámicas por jerarquía DENTRO del while, debe enviar la información al shader DESPUÉS de haber actualizado todas las posiciones.
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//cubo
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.0f, 0.0f, 0.0));
		//model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dadoTexture.UseTexture();
		//meshList[4]->RenderMesh();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);


		// NOTA: INSTANCIAMOS LA PECERA (SIEMPRE AL FINAL DEL WHILE) PERO NO ANTES DE glUseProgram(0);
		// Activamos Blending para la transparencia
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Evitamos que las paredes frontales tapen a las traseras por el buffer de profundidad
		glDepthMask(GL_FALSE);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, -5.0f));
		model = glm::scale(model, glm::vec3(40.0f, 20.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		texturaAgua.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		peceraAgua->RenderMesh();

		// Renderiza el Cristal (Las 5 paredes restantes)
		texturaCristal.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		peceraCristal->RenderMesh();
		//Apagamos configuraciones especiales
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}