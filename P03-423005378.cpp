// práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//clases para dar orden y limpieza al còdigo
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z

using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0f;//grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20);//recibe radio, slices, stacks


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

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

void CrearPiramideCuadrangular() {
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4, 3,2,4, 2,1,4, 1,0,4, 0,1,2, 0,2,4
	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,  0.5f,-0.5f,-0.5f,  -0.5f,-0.5f,-0.5f,  -0.5f,-0.5f,0.5f,  0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

// RUBICK PIRAMIDE
// Crea una pirámide pequeña
void CrearPiramidePequena(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, glm::vec3 faceNormal, Mesh* mesh) {
	glm::vec3 centroid = (t1 + t2 + t3) / 3.0f;
	float S = 0.88f;
	glm::vec3 p1 = centroid + (t1 - centroid) * S;
	glm::vec3 p2 = centroid + (t2 - centroid) * S;
	glm::vec3 p3 = centroid + (t3 - centroid) * S;

	p1 += faceNormal * 0.02f;
	p2 += faceNormal * 0.02f;
	p3 += faceNormal * 0.02f;

	glm::vec3 tip = centroid - faceNormal * 0.45f;

	vector<GLfloat> vertices = {
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		p3.x, p3.y, p3.z,
		tip.x, tip.y, tip.z
	};

	vector<unsigned int> indices = {
		0, 1, 2,
		0, 3, 1,
		1, 3, 2,
		2, 3, 0
	};

	mesh->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
}


void CrearCaraEsquina(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, glm::vec3 centroidCorner, glm::vec3 faceNormal, Mesh* mesh) {

	float S = 0.88f;
	glm::vec3 p1 = centroidCorner + (t1 - centroidCorner) * S;
	glm::vec3 p2 = centroidCorner + (t2 - centroidCorner) * S;
	glm::vec3 p3 = centroidCorner + (t3 - centroidCorner) * S;

	
	p1 += faceNormal * 0.02f;
	p2 += faceNormal * 0.02f;
	p3 += faceNormal * 0.02f;

	// La punta es exactamente el centro del tetraedro de la esquina
	glm::vec3 tip = centroidCorner + faceNormal * 0.02f;

	vector<GLfloat> vertices = {
		p1.x, p1.y, p1.z,       
		p2.x, p2.y, p2.z,       
		p3.x, p3.y, p3.z,       
		tip.x, tip.y, tip.z		
	};

	vector<unsigned int> indices = {
		0, 1, 2,                
		0, 3, 1,              
		2, 3, 0                 // Pared interna
	};

	mesh->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
}

// Genera los 9 triángulos de una cara del RUBICK
void GenerarPiramidesCara(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 cA, glm::vec3 cB, glm::vec3 cC) {

	int N = 3;
	vector<glm::vec3> puntos;

	glm::vec3 normalCara = glm::normalize((A + B + C) / 3.0f);

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N - i; j++) {

			int k = N - i - j;

			glm::vec3 p = (float(i) * A + float(j) * B + float(k) * C) / float(N);
			puntos.push_back(p);
		}
	}

	auto indice = [](int i, int j) {

		int offset = 0;

		for (int r = 0; r < i; r++)
			offset += (4 - r);

		return offset + j;
		};

	for (int i = 0; i < N; i++) {

		for (int j = 0; j < N - i; j++) {

			Mesh* pieza1 = new Mesh();

			glm::vec3 p1 = puntos[indice(i, j)];
			glm::vec3 p2 = puntos[indice(i + 1, j)];
			glm::vec3 p3 = puntos[indice(i, j + 1)];

			if (i == 2 && j == 0) {

				CrearCaraEsquina(p1, p2, p3, cA, normalCara, pieza1);

			}
			else if (i == 0 && j == 2) {

				CrearCaraEsquina(p1, p2, p3, cB, normalCara, pieza1);

			}
			else if (i == 0 && j == 0) {

				CrearCaraEsquina(p1, p2, p3, cC, normalCara, pieza1);

			}
			else {

				CrearPiramidePequena(p1, p2, p3, normalCara, pieza1);

			}

			meshList.push_back(pieza1);

			if (i + j < N - 1) {

				Mesh* pieza2 = new Mesh();

				glm::vec3 p4 = puntos[indice(i + 1, j)];
				glm::vec3 p5 = puntos[indice(i + 1, j + 1)];
				glm::vec3 p6 = puntos[indice(i, j + 1)];

				CrearPiramidePequena(p4, p5, p6, normalCara, pieza2);

				meshList.push_back(pieza2);
			}
		}
	}
}


void CrearPyraminx() {

	float radio = 2.0f;

	glm::vec3 V0(0.0f, radio, 0.0f);
	glm::vec3 V1(radio * 0.9428f, -radio * 0.3333f, 0.0f);
	glm::vec3 V2(-radio * 0.4714f, -radio * 0.3333f, radio * 0.8165f);
	glm::vec3 V3(-radio * 0.4714f, -radio * 0.3333f, -radio * 0.8165f);

	auto centroEsquina = [](glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {

		float t = 1.0f / 3.0f;

		glm::vec3 p1 = v0 + (v1 - v0) * t;
		glm::vec3 p2 = v0 + (v2 - v0) * t;
		glm::vec3 p3 = v0 + (v3 - v0) * t;

		return (v0 + p1 + p2 + p3) / 4.0f;
		};

	glm::vec3 C0 = centroEsquina(V0, V1, V2, V3);
	glm::vec3 C1 = centroEsquina(V1, V0, V2, V3);
	glm::vec3 C2 = centroEsquina(V2, V0, V1, V3);
	glm::vec3 C3 = centroEsquina(V3, V0, V1, V2);

	Mesh* nucleo = new Mesh();

	glm::vec3 n0 = V0 * 0.90f;
	glm::vec3 n1 = V1 * 0.90f;
	glm::vec3 n2 = V2 * 0.90f;
	glm::vec3 n3 = V3 * 0.90f;

	vector<GLfloat> vertices = {

		n0.x,n0.y,n0.z,
		n1.x,n1.y,n1.z,
		n2.x,n2.y,n2.z,
		n3.x,n3.y,n3.z
	};

	vector<unsigned int> indN = {
	0, 1, 2,
	0, 3, 1,
	0, 2, 3,
	2,3,1  // Cara base (inferior)
	};
	nucleo->CreateMeshGeometry(vertices, indN, vertices.size(), indN.size());
	meshList.push_back(nucleo);
	
	GenerarPiramidesCara(V0, V2, V1, C0, C2, C1);
	GenerarPiramidesCara(V0, V3, V2, C0, C3, C2);
	GenerarPiramidesCara(V0, V1, V3, C0, C1, C3);
	GenerarPiramidesCara(V1, V3, V2, C1, C3, C2);
}


void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

int main() {
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CrearCubo();
	CrearPiramideTriangular();
	CrearCilindro(5, 1.0f);
	CrearCono(25, 2.0f);
	CrearPiramideCuadrangular();

	CrearPyraminx(); // Crea los índices 5 al 41 en meshList

	CreateShaders();

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
En la clase Camera se reciben 5 datos:
glm::vec3 vector de posición,
glm::vec3 vector up,
GlFloat yaw rotación para girar hacia la derecha e izquierda
GlFloat pitch rotación para inclinar hacia arriba y abajo
GlFloat velocidad de desplazamiento,
GlFloat velocidad de vuelta o de giro
Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
*/


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	sp.init();
	sp.load();

	glm::mat4 model(1.0);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);


	while (!mainWindow.getShouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -6.0f));

		// Animación de rotación
		model = glm::rotate(model, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));

		// Controles
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// PYRAMINX 
		float tamaño = 5.8f; // Tamaño de la pirámide negra
		float size = 1.6f;   
		float space = 0.2f; 
		float L = size + space;
		float H = L * 0.866025f; // Altura geométrica


		/*
		//ejercicio: Instanciar primitivas geométricas para recrear el dibujo de la práctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.
		model = glm::mat4(1.0f);
		color=glm::vec3(0.0f,1.0f,0.0f);
		//Opcional duplicar esta traslación inicial para posicionar en -Z a los objetos en el mismo punto
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();
		*/

		//// -------- 1. PIRÁMIDE NEGRA (FONDO) -------
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.5f)); 
		//model = glm::scale(model, glm::vec3(tamaño));
		//color = glm::vec3(0.0f, 0.0f, 0.0f);
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[1]->RenderMeshGeometry();



		//// -------- 2. CARA ROJA (FRENTE) --------

		//glm::mat4 caraRojaBase = glm::mat4(1.0f);

		//caraRojaBase = glm::translate(caraRojaBase, glm::vec3(0.0f, -1.8f, -2.5f));
		//caraRojaBase = glm::rotate(caraRojaBase, glm::radians(-19.5f), glm::vec3(1.0f, 0.0f, 0.0f));

		//for (int fila = 0; fila < 3; fila++)
		//{
		//	// Pirámides normales (6 piezas)
		//	for (int col = 0; col < 3 - fila; col++)
		//	{
		//		model = caraRojaBase;
		//		// Posición X centrada dinámicamente según la fila
		//		float xPos = (col - (2 - fila) * 0.5f) * L;
		//		float yPos = fila * H;

		//		model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
		//		model = glm::scale(model, glm::vec3(size));

		//		color = glm::vec3(1.0f, 0.0f, 0.0f);
		//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//		meshList[1]->RenderMeshGeometry();
		//	}

		//	// Pirámides invertidas (3 piezas) - AQUÍ ESTABA EL ERROR
		//	if (fila < 2)
		//	{
		//		for (int col = 0; col < 2 - fila; col++)
		//		{
		//			model = caraRojaBase;
		//			// Corregido: El offset de X ahora usa (1 - fila) y eliminamos el * 0.0f
		//			float xPos = (col - (1 - fila) * 0.5f) * L;
		//			float yPos = fila * H + (H * -0.065f); // Ajuste fino de altura para encajar
	//			model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f))
		//			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));

		//			model = glm::scale(model, glm::vec3(size))
		//			color = glm::vec3(1.0f, 0.0f, 0.0f);
		//			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//			meshList[1]->RenderMeshGeometry();
		//		}
		//	}
		
		//  Piramide negra
		color = glm::vec3(0.05f, 0.05f, 0.05f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[5]->RenderMeshGeometry();
		
		glm::vec3 faceColors[4] = {

			glm::vec3(0.1f, 0.8f, 0.2f),
			glm::vec3(0.1f, 0.3f, 0.9f),
			glm::vec3(0.9f, 0.1f, 0.1f),
			glm::vec3(0.9f, 0.9f, 0.1f)
		};

		int indexMesh = 6;
		for (int cara = 0; cara < 4; cara++) {

			glUniform3fv(uniformColor, 1, glm::value_ptr(faceColors[cara]));
			for (int pieza = 0; pieza < 9; pieza++) {
				meshList[indexMesh]->RenderMeshGeometry();
			indexMesh++;
			}
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


