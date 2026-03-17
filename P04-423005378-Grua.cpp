
/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
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
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

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
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
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

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


void DibujarPata(glm::mat4 modeloBase, GLfloat anguloSuperior, GLfloat anguloInferior, GLuint uniformModel, GLuint uniformColor, vector<Mesh*> meshList) {
	glm::mat4 model = modeloBase;

	// ==========================================
	// 1. ARTICULACIÓN SUPERIOR (CADERA)
	// ==========================================
	model = glm::rotate(model, glm::radians(anguloSuperior), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 caderaAux = model; // Punto de pivote exacto de la cadera

	// ---> Dibujar la ESFERA de la Cadera
	glm::mat4 modelEsferaCadera = glm::scale(caderaAux, glm::vec3(0.7f, 0.7f, 0.7f)); // Tamaño de la articulación
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsferaCadera));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.8f, 0.2f, 0.2f))); // Color rojo/granate para distinguirla
	sp.render(); // Usamos tu esfera global

	// ---> Dibujar el Muslo (Cilindro)
	model = glm::translate(caderaAux, glm::vec3(0.0f, -1.0f, 0.0f)); // Bajamos la mitad de la altura del cilindro
	model = glm::scale(model, glm::vec3(0.5f, 2.0f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f))); // Color oscuro para las patas
	meshList[2]->RenderMeshGeometry();

	// ==========================================
	// 2. ARTICULACIÓN INFERIOR (RODILLA)
	// ==========================================
	model = caderaAux;
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // Bajamos hasta el final del muslo
	model = glm::rotate(model, glm::radians(anguloInferior), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rodillaAux = model; // Punto de pivote exacto de la rodilla

	// ---> Dibujar la ESFERA de la Rodilla
	glm::mat4 modelEsferaRodilla = glm::scale(rodillaAux, glm::vec3(0.55f, 0.55f, 0.55f)); // Un poco más pequeña que la cadera
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsferaRodilla));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.8f, 0.2f, 0.2f))); // Mismo color rojo/granate
	sp.render();

	// ---> Dibujar la Pantorrilla (Cilindro)
	model = glm::translate(rodillaAux, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 2.0f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	meshList[2]->RenderMeshGeometry();
}
void DibujarOreja(glm::mat4 modeloBase, GLfloat angulo, GLuint uniformModel, GLuint uniformColor, vector<Mesh*> meshList) {
	glm::mat4 model = modeloBase;

	// 1. Rotar la Oreja (Articulación)
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));

	// 2. Posicionar y escalar la geometría de la oreja
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f)); // Subir la oreja para pivotar desde la base
	model = glm::scale(model, glm::vec3(0.4f, 1.2f, 0.4f)); // Orejas un poco alargadas

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f))); // Color oscuro

	// Usamos el cubo (meshList[0]) o el cilindro (meshList[2])
	meshList[0]->RenderMesh();
}

void DibujarCola(glm::mat4 modeloBase, GLfloat anguloBase, GLfloat anguloPunta, GLuint uniformModel, GLuint uniformColor, vector<Mesh*> meshList) {
	glm::mat4 model = modeloBase;

	// ==========================================
	// 1. BASE DE LA COLA (Primera articulación)
	// ==========================================
	// Rotar un poco hacia atrás por defecto (-45 grados) más lo que sumes con el teclado
	model = glm::rotate(model, glm::radians(-45.0f + anguloBase), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 baseAux = model;

	// Esfera de la base
	glm::mat4 modelEsfera1 = glm::scale(baseAux, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera1));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.8f, 0.2f, 0.2f))); // Color rojo articulación
	sp.render();

	// Primer segmento (Cilindro)
	model = glm::translate(baseAux, glm::vec3(0.0f, 0.8f, 0.0f)); // Subir la mitad del cilindro
	model = glm::scale(model, glm::vec3(0.3f, 1.6f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	meshList[2]->RenderMeshGeometry(); // Cilindro

	// ==========================================
	// 2. PUNTA DE LA COLA (Segunda articulación)
	// ==========================================
	model = baseAux;
	model = glm::translate(model, glm::vec3(0.0f, 1.6f, 0.0f)); // Ir a la punta del primer segmento
	model = glm::rotate(model, glm::radians(anguloPunta), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 puntaAux = model;

	// Esfera del medio
	glm::mat4 modelEsfera2 = glm::scale(puntaAux, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera2));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.8f, 0.2f, 0.2f))); // Color rojo
	sp.render();

	// Segundo segmento (Cono)
	model = glm::translate(puntaAux, glm::vec3(0.0f, 0.8f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 1.6f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	meshList[3]->RenderMeshGeometry(); // Usamos tu Cono (índice 3)
}

void DibujarCartelMili(glm::mat4 modeloBase, GLuint uniformModel, GLuint uniformColor, vector<Mesh*> meshList) {
	// 1. Dibujar el panel de fondo (Blanco)
	glm::mat4 panel = glm::translate(modeloBase, glm::vec3(0.0f, 0.0f, -0.2f));
	panel = glm::scale(panel, glm::vec3(8.0f, 1.8f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(panel));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f))); // Fondo blanco
	meshList[0]->RenderMesh();

	// Cambiamos el color para las letras (Ej. Rosa/Morado)
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.8f, 0.2f, 0.6f)));

	// Función auxiliar interna (Lambda) para dibujar los bloques de las letras rápido
	auto Bloque = [&](float x, float y, float sx, float sy) {
		glm::mat4 m = glm::translate(modeloBase, glm::vec3(x, y, 0.0f));
		m = glm::scale(m, glm::vec3(sx, sy, 0.2f)); // 0.2f de grosor para que resalte
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m));
		meshList[0]->RenderMesh();
		};

	// --- PALABRA: PARA ---
	// Letra P
	Bloque(-3.2f, 0.0f, 0.1f, 1.0f);   // Poste izq
	Bloque(-3.0f, 0.45f, 0.3f, 0.1f);  // Techo
	Bloque(-3.0f, 0.05f, 0.3f, 0.1f);  // Medio
	Bloque(-2.8f, 0.25f, 0.1f, 0.5f);  // Poste der (arriba)

	// Letra A
	Bloque(-2.3f, 0.0f, 0.1f, 1.0f);   // Poste izq
	Bloque(-1.7f, 0.0f, 0.1f, 1.0f);   // Poste der
	Bloque(-2.0f, 0.45f, 0.5f, 0.1f);  // Techo
	Bloque(-2.0f, 0.0f, 0.5f, 0.1f);  // Medio

	// Letra R
	Bloque(-1.2f, 0.0f, 0.1f, 1.0f);   // Poste izq
	Bloque(-1.0f, 0.45f, 0.3f, 0.1f);  // Techo
	Bloque(-1.0f, 0.05f, 0.3f, 0.1f);  // Medio
	Bloque(-0.8f, 0.25f, 0.1f, 0.5f);  // Poste der (arriba)
	Bloque(-0.9f, -0.3f, 0.1f, 0.4f);  // Pata diagonal (simplificada)

	// Letra A
	Bloque(-0.3f, 0.0f, 0.1f, 1.0f);   // Poste izq
	Bloque(0.3f, 0.0f, 0.1f, 1.0f);    // Poste der
	Bloque(0.0f, 0.45f, 0.5f, 0.1f);   // Techo
	Bloque(0.0f, 0.0f, 0.5f, 0.1f);   // Medio

	// --- PALABRA: MILI ---
	// Letra M
	Bloque(1.2f, 0.0f, 0.1f, 1.0f);    // Poste izq
	Bloque(2.0f, 0.0f, 0.1f, 1.0f);    // Poste der
	Bloque(1.4f, 0.25f, 0.1f, 0.5f);   // Diagonal izq
	Bloque(1.8f, 0.25f, 0.1f, 0.5f);   // Diagonal der
	Bloque(1.6f, 0.0f, 0.1f, 0.4f);   // Centro

	// Letra I
	Bloque(2.4f, 0.0f, 0.1f, 1.0f);    // Poste

	// Letra L
	Bloque(2.8f, 0.0f, 0.1f, 1.0f);    // Poste
	Bloque(3.0f, -0.45f, 0.4f, 0.1f);  // Base

	// Letra I
	Bloque(3.4f, 0.0f, 0.1f, 1.0f);    // Poste
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(20, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
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
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		// 4. MANDAR LA CÁMARA Y PROYECCIÓN A LA TARJETA GRÁFICA 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


		//CREANDO LA CABINA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 7.0f, -4.0f));
		//aqui está la jerarquía
		modelaux = model;
		model = glm::scale(model, glm::vec3(7.0f, 4.0f, 5.0f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		// =======================================================================
		// INICIO DE LA JERARQUÍA INFERIOR (Hereda DIRECTAMENTE de la Cabina)


		model = modelaux;
		// Trasladamos la base triangular hacia abajo de la cabina. 
		// Usamos -3.5f para ponerla donde solía empezar el cuerpo.
		model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
		// Guardamos esta matriz sin escalar para dársela a las llantas
		glm::mat4 modelaux_base = model;
		// Escalamos la base (ajusta estos valores a consideracion )
		model = glm::scale(model, glm::vec3(8.0f, 4.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); // Color Naranja/Óxido
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Se usa RenderMesh() porque la pirámide triangular se creó con arreglos estáticos
		meshList[1]->RenderMesh();





		// 2. LLANTAS (4 Cilindros - meshList[2]) - SIGUEN HEREDANDO DE LA BASE
		// No cambiamos nada aquí, solo se asegura de heredar de la matriz base correcta.
		// Llanta 1 (Frente - Derecha)
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(3.5f, -0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta1()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.1f, 0.1f); // Casi negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		// Llanta 2 (Frente - Izquierda)
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta2()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();

		// Llanta 3 (Atrás - Derecha)
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(3.5f, -0.5f, -3.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta3()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();

		// Llanta 4 (Atrás - Izquierda)
		model = modelaux_base;
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, -3.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getLlanta4()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();

		// =======================================================================



		//crear la articulación 1
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Creando el brazo de una grúa
			//articulacion1 hasta articulación5 sólo son puntos de rotación o articulación, en este caso no dibujaremos esferas que los representen

		//primer brazo que conecta con la cabina

		//para reiniciar la matriz de modelo con valor de la matriz identidad
		//model = glm::mat4(1.0); en jerarquía rompe la conexión
		//rotación alrededor de la articulación que une con la cabina
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera

		//articulacion 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		//segundo brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		model = modelaux;

		//articulación 3 extremo derecho del segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		// 1. La rotación de giro (controlada por tus teclas)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));

		// 2. EL POSICIONAMIENTO EN GRADOS (Ángulo inicial fijo)
		// Si quieres que inicie con la misma inclinación que el brazo 1, ponle 135.0f. 
		// Puedes cambiar este número al ángulo exacto que necesites (ej. 90.0f, 45.0f, etc.)
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model;

		//dibujar una pequeña esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		
		//Brazo 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		
		//articulacion 4
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glm::mat4 esfera = model;
		esfera = glm::scale(esfera, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(esfera));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		
		//  CANASTA
		model = modelaux; 
		// Esto empuja la canasta hacia afuera para que su base quede pegada a la articulación
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		// Opcional:que inicie enderezada (vertical u horizontal), puedes agregar una rotación aquí:
		// model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		// Escala de la canasta (Altura en Y = 4.0f)
		model = glm::scale(model, glm::vec3(7.0f, 4.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
			

//// CUERPO DEL PERRO ROBOT
//		glm::mat4 modelPerro = glm::mat4(1.0);
//		modelPerro = glm::translate(modelPerro, glm::vec3(8.0f, 2.0f, 10.0f)); // Posición general
//		glm::mat4 cuerpoAux = modelPerro; // <--- AQUÍ se define cuerpoAux
//
//		// Dibujar el torso
//		modelPerro = glm::scale(modelPerro, glm::vec3(4.0f, 2.0f, 6.0f));
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPerro));
//		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
//		meshList[0]->RenderMesh();
//
//		// DIBUJAR LAS 4 PATAS USANDO LA FUNCIÓN
//		// Pata Delantera Derecha (Frente Derecha)
//		DibujarPata(glm::translate(cuerpoAux, glm::vec3(2.0f, -1.0f, 2.5f)),
//			mainWindow.getPatasUpperFrendeD(), mainWindow.getPatasLowerFrendeD(),
//			uniformModel, uniformColor, meshList);
//
//		// Pata Delantera Izquierda (Frente Izquierda)
//		DibujarPata(glm::translate(cuerpoAux, glm::vec3(-2.0f, -1.0f, 2.5f)),
//			mainWindow.getPatasUpperFrendeI(), mainWindow.getPatasLowerFrendeI(),
//			uniformModel, uniformColor, meshList);
//
//		// Pata Trasera Derecha (Atras Derecha)
//		DibujarPata(glm::translate(cuerpoAux, glm::vec3(2.0f, -1.0f, -2.5f)),
//			mainWindow.getPatasUpperAtrasD(), mainWindow.getPatasLowerAtrasD(),
//			uniformModel, uniformColor, meshList);
//
//		// Pata Trasera Izquierda (Atras Izquierda)
//		DibujarPata(glm::translate(cuerpoAux, glm::vec3(-2.0f, -1.0f, -2.5f)),
//			mainWindow.getPatasUpperAtrasI(), mainWindow.getPatasLowerAtrasI(),
//			uniformModel, uniformColor, meshList);
//
//
//
//
//// CABEZA DEL PERRO ROBOT (Hereda de cuerpoAux)
//
//		model = cuerpoAux;
//		// Posicionar la cabeza con una traslación
//		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 3.5f)); // Frente arriba
//		// Aquí podrías agregar rotación para que el perro "mire" a los lados
//		// model = glm::rotate(model, glm::radians(mainWindow.getArticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f)); 
//		glm::mat4 cabezaAux = model;
//		// DIBUJAR EL CUBO PRINCIPAL DE LA CABEZA
//		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		color = glm::vec3(0.7f, 0.7f, 0.7f); // Color gris de la cabeza
//		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//		meshList[0]->RenderMesh(); // Cubo para la cabeza
//
//
//		// --- AGREGANDO OJOS (Hijos de cabezaAux) ---
//		// Usaremos cubos pequeños de color negro.
//
//		// Cambiar color a Negro para ojos y nariz
//		color = glm::vec3(0.0f, 0.0f, 0.0f);
//		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//
//		// Ojo Derecho
//		model = cabezaAux; // Empezamos desde la matriz base de la cabeza
//		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 1.0f)); // X=der, Y=arr, Z=frente (ligeramente afuera del cubo principal)
//		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Escalar a un cubo pequeño
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshList[0]->RenderMesh(); // Dibujar cubo del ojo
//
//		// Ojo Izquierdo
//		model = cabezaAux; // Empezamos desde la matriz base de la cabeza
//		model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 1.0f)); // X=izq (negativo), Y=arr, Z=frente
//		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); // Escalar a un cubo pequeño
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshList[0]->RenderMesh(); // Dibujar cubo del ojo
//
//
//		// --- AGREGANDO NARIZ (Hijo de cabezaAux) ---
//
//		// Nariz
//		model = cabezaAux; // Empezamos desde la matriz base de la cabeza
//		// Trasladar a la posición de la nariz (frente, centro, abajo)
//		model = glm::translate(model, glm::vec3(0.0f, -0.2f, 1.1f)); // X=centro, Y=abajo, Z=frente (más afuera que los ojos)
//		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); // Escalar a un cubo pequeño (un poco más grande que los ojos)
//		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//		meshList[0]->RenderMesh();
//		// DIBUJAR LAS 2 OREJAS
//
//		// Oreja Derecha
//		DibujarOreja(glm::translate(cabezaAux, glm::vec3(1.3f, 1.2f, -0.5f)),
//			mainWindow.getOrejaDerecha(), uniformModel, uniformColor, meshList);
//
//		// Oreja Izquierda
//		DibujarOreja(glm::translate(cabezaAux, glm::vec3(-1.3f, 1.2f, -0.5f)),
//			mainWindow.getOrejaIzquierda(), uniformModel, uniformColor, meshList);
//
//
//// =======================================================================
//// COLA DEL PERRO ROBOT
//
//// Trasladamos la base a la parte trasera alta del cuerpo (cuerpoAux)
//// Usamos "getCola()" para la base y "getarticulacion6()" para la punta 
//// (puedes usar las variables que tú prefieras).
//
//		DibujarCola(glm::translate(cuerpoAux, glm::vec3(0.0f, 1.0f, -3.0f)),
//			mainWindow.getCola(), mainWindow.getarticulacion6(),
//			uniformModel, uniformColor, meshList);
//
//
//// CARTEL "PARA MILI"
//// Lo trasladamos 4 unidades hacia arriba del centro del perro
//		glm::mat4 posicionCartel = glm::translate(cuerpoAux, glm::vec3(0.0f, 4.0f, 0.0f));
//
//		// Llamamos a la función
//		DibujarCartelMili(posicionCartel, uniformModel, uniformColor, meshList);

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
	
		