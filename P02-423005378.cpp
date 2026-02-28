//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList; // Descomentado para las letras
std::vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//shaders nuevos se crearían acá
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* vShaderVerdeOscuro = "shaders/shaderverdeoscuro.vert";
static const char* vShaderrojo = "shaders/shaderrojo.vert";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}


// BLOQUE COMENTADO ORIGINALMENTE POR TI:
//void CrearLetrasyFiguras()
//{
//	GLfloat vertices_letras[] = {	
//			//X			Y			Z			R		G		B
//			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
//			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
//			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
//			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
//			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
//			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
//			
//	};
//	MeshColor *letras = new MeshColor();
//	letras->CreateMeshColor(vertices_letras,18);
//	meshColorList.push_back(letras);
//
//	GLfloat vertices_triangulorojo[] = {
//		//X			Y			Z			R		G		B
//		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
//		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
//		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
//		
//	};
//
//	MeshColor* triangulorojo = new MeshColor();
//	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
//	meshColorList.push_back(triangulorojo);
//
//	GLfloat vertices_cuadradoverde[] = {
//		//X			Y			Z			R		G		B
//		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
//		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
//		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
//		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
//		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
//		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
//
//	};
//
//	MeshColor* cuadradoverde = new MeshColor();
//	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
//	meshColorList.push_back(cuadradoverde);
//
//	GLfloat vertices_cuadradorojo[] = {
//		//X			Y			Z			R		G		B
//		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
//		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
//		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
//		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
//		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
//		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
//
//	};
//
//	MeshColor* cuadradorojo = new MeshColor();
//	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
//	meshColorList.push_back(cuadradorojo);
//
//
//}

void CrearIniciales()
{
	// --- LETRA J (Azul) ---
	GLfloat vertices_J[] = {
		 0.1f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.3f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.3f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.1f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.3f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.1f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,

		-0.3f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.3f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.3f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.3f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,

		-0.3f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.1f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.1f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.1f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.3f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f
	};
	MeshColor* letraJ = new MeshColor();
	letraJ->CreateMeshColor(vertices_J, 108);
	meshColorList.push_back(letraJ); // Índice 0

	// --- LETRA T (Roja) ---
	GLfloat vertices_T[] = {
		-0.5f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.5f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,

		-0.1f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.1f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.1f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.1f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.1f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.1f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f
	};
	MeshColor* letraT = new MeshColor();
	letraT->CreateMeshColor(vertices_T, 72);
	meshColorList.push_back(letraT); // Índice 1

	// --- LETRA C (Verde) ---
	GLfloat vertices_C[] = {
		-0.4f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.4f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.4f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.4f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.4f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.4f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,

		-0.4f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.2f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.2f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.4f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.2f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.4f,  0.3f, 0.0f,   0.0f, 1.0f, 0.0f,

		-0.4f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.4f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.4f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.4f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.4f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.4f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f
	};
	MeshColor* letraC = new MeshColor();
	letraC->CreateMeshColor(vertices_C, 108);
	meshColorList.push_back(letraC); // Índice 2
}


void CreateShaders()
{

	// BLOQUE COMENTADO ORIGINALMENTE POR TI:
	//Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	//shader1->CreateFromFiles(vShader, fShader);
	//shaderList.push_back(*shader1);

	//Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	//shader2->CreateFromFiles(vShaderColor, fShaderColor);
	//shaderList.push_back(*shader2);

	//Shader* shader3 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	//shader3->CreateFromFiles(vShaderrojo, fShader);
	//shaderList.push_back(*shader3);

	//donde los cree
	Shader rojo;
	rojo.CreateFromFiles(vShaderrojo, fShader);
	shaderList.push_back(rojo); // Índice 0

	Shader verde;
	verde.CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(verde); // Índice 1

	Shader azul;
	azul.CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(azul); // Índice 2

	Shader cafe;
	cafe.CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(cafe); // Índice 3

	Shader verdeOscuro;
	verdeOscuro.CreateFromFiles(vShaderVerdeOscuro, fShader);
	shaderList.push_back(verdeOscuro); // Índice 4

	// SHADER PARA LAS LETRAS
	Shader shaderLetras;
	shaderLetras.CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(shaderLetras); // Índice 5

}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	glEnable(GL_DEPTH_TEST);
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList

	//CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CrearIniciales(); // Iniciales agregadas

	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	glm::mat4 projection = glm::perspective(
		glm::radians(75.0f),// solo se modifico el campo de vision 
		(float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(),
		0.1f,
		100.0f
	);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		// ==============================
		// CUERPO DE LA CASA (CUBO)
		// ==============================
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -6.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh(); // cubo


		// ==============================
		//  TECHO (PIRÁMIDE)
		// ==============================
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.7f, -6.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.8f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); // pirámide


		// ==============================
		//  VENTANAS (CUBOS)
		// ==============================
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Ventana izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, 0.2f, -5.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Ventana derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, 0.2f, -5.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Puerta
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.9f, -5.0f));
		model = glm::scale(model, glm::vec3(0.7f, 1.0f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		// ==============================
		//  ÁRBOL IZQUIERDO
		// ==============================

		// Tronco 
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, -1.2f, -6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Hojas 
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.3f, -6.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();


		// ==============================
		//  ÁRBOL DERECHO
		// ==============================

		// Tronco 
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, -1.2f, -6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Hojas 
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, 0.3f, -6.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// ==============================
		// LETRAS INICIALES (J T C) 
		// ==============================
		shaderList[5].useShader(); // Indice 5 es el shader para los colores por vertice
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// LETRA J 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 3.8f, -6.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor();

		// LETRA T
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 3.8f, -6.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		// LETRA C 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 3.8f, -6.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();


		// BLOQUE COMENTADO ORIGINALMENTE POR TI EN EL LOOP:
		////Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		//shaderList[0].useShader(); 
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();
		//angulo += 0.01;
		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[1]->RenderMesh();
		//
		////piramide
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-1.25f, 1.25f, -4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//meshList[0]->RenderMesh();

		//para usar el shader rojo
		//shaderList[2].useShader(); 
		//uniformModel = shaderList[2].getModelLocation();
		//uniformProjection = shaderList[2].getProjectLocation();


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}

// BLOQUE COMENTADO ORIGINALMENTE POR TI AL FINAL:
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/