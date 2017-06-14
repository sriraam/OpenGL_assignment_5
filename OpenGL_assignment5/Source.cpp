#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC
#include<gl/glew.h>

#include"shader.h"
#include"texture.h"
#include<GL/freeglut.h>
//#include<iostream>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
//#include<fstream>
#include<vector>
//#include<string>



GLuint VertexArrayID;

glm::mat4 View, Model, Projection;

GLint modelLoc, viewLoc, projLoc;

GLuint lightcolor_loc, materialcolor_loc;
GLuint lightposLoc;
GLuint MatrixID;

GLuint VertexBuffer;
GLuint VertexBuffer2;

//GLuint framebuffer;

GLuint VAO_2, vao_floor;// , vao_screen;
GLuint VBO_2, vbo_floor;// , vbo_screen;

GLuint texture1;
GLuint texture_floor;
//GLuint texture_framebuf;

GLuint renderbuf;


texture Wood_texture,floor_texture;

shader shader_main;
shader shader_floor;
//shader shader_screen;

//GLuint g_ShaderProgram = 0;
//glGenVertexArrays(1, &VertexArrayID);

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 40.0f, beta = 45.0f;
float r = 5.25f;



void display1()
{
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(camX, camY, camZ), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	Model = glm::mat4();
	Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
	

//	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glViewport(0, 0, 640, 480);

	glEnable(GL_DEPTH_TEST);

	shader_main.Use();
	
	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "view"), 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	
	//cube_vao:
	glBindVertexArray(VAO_2);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(0);
	glUniform1i(glGetUniformLocation(shader_main.program, "texture_1"), 0);
	Model = glm::translate(Model, glm::vec3(1.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, 0, 36); // Starting from vertex 0; 3 vertices total -> 1 triangle
	Model = glm::translate(Model, glm::vec3(-2.0, 0.0f, 2.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader_main.program,"model"),1,GL_FALSE,glm::value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	shader_floor.Use();

	glUniformMatrix4fv(glGetUniformLocation(shader_floor.program, "view"), 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(glGetUniformLocation(shader_floor.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));

	//floor_vao:
	glBindVertexArray(vao_floor);
	glBindTexture(GL_TEXTURE_2D, texture_floor);
	glUniform1i(glGetUniformLocation(shader_floor.program,"floor_texture"),0);
	//Model = glm::translate(Model, glm::vec3(-1.0f, -1.0f, -1.0f));
	Model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(shader_floor.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0);
	
/*
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 640, 480);
	glClearColor(0, 1, 1, 1);
	//glClearColor(1.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH);
	

	
	shader_screen.Use();

	glBindVertexArray(vao_screen);
	
	glBindTexture(GL_TEXTURE_2D, texture_framebuf);
	glUniform1f(glGetUniformLocation(shader_screen.program, "screentexture"), 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
	 //glFlush();
	glutSwapBuffers();
	glDeleteFramebuffers(1, &framebuffer);
	*/
}


void init() {
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
	// some GL settings
	glViewport(0, 0, 640, 480);

	//glEnable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	


	//So we need three 3D points in order to make a triangle
	static const GLfloat g_Vertex_Buffer_data[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};

	GLfloat cubeVertices[] = {
		// Positions          // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLfloat floor_vertices[] = {
		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
	};

	/*GLfloat Quad[] = {
		
		-1, 1, 0, 0,1,
		 1, 1, 0, 1,1, 
		 1,-1, 0, 1,0,
		 1,-1, 0, 1,0,
		-1,-1, 0, 0,0,
		-1, 1, 0, 0,1

		// positions   // texCoords
		- 1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,0.0f,  0.0f, 0.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,
		1.0f,  1.0f,0.0f,  1.0f, 1.0f
		
	};*/

	//wooden texture
	Wood_texture.loadtexture("wooden_texture.jpg");
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Wood_texture.Width, Wood_texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Wood_texture.Data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//floor texture
	floor_texture.loadtexture("floor.jpg");
	glGenTextures(1, &texture_floor);
	glBindTexture(GL_TEXTURE_2D, texture_floor); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, floor_texture.Width, floor_texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, floor_texture.Data);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D,0);


	
	/*
	glGenVertexArrays(1, &VertexArrayID);
	glGenBuffers(1, &VertexBuffer);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertex_Buffer_data), g_Vertex_Buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	*/

	
	//Cube VAO
	glGenVertexArrays(1, &VAO_2);
	glGenBuffers(1, &VBO_2);
	glBindVertexArray(VAO_2);
	glBindBuffer(GL_ARRAY_BUFFER,VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//floor VAO
	glGenVertexArrays(1, &vao_floor);
	glGenBuffers(1, &vbo_floor);
	glBindVertexArray(vao_floor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_floor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	/*
	//Quad VAO
	glGenVertexArrays(1, &vao_screen);
	glGenBuffers(1, &vbo_screen);
	glBindVertexArray(vao_screen);
	glBindBuffer(GL_ARRAY_BUFFER,vbo_screen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	*/
/*
	//Frame Buffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//texture attachment(it samples)
	glGenTextures(1, &texture_framebuf);
	glBindTexture(GL_TEXTURE_2D, texture_framebuf);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);


	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);			

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_framebuf, 0);

	//

	//render attachment(has depth values)

	glGenRenderbuffers(1, &renderbuf);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT , 640, 480);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuf);
	
	
	
	//checking whether the frame buffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glDeleteFramebuffers(1, &framebuffer);
	*/

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

}

void processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
		break;

	}

	//  uncomment this if not using an idle func
	glutPostRedisplay();
}
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}



	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	glutPostRedisplay();
}
void reshape(int width,int heigth) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)heigth);
	Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)heigth, 0.1f, 100.0f);
}

int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitContextVersion(3, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	/*
	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);*/

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutCreateWindow("simple rectangle");


	glewExperimental = true; // Needed in core profile
	

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}



	//shader shader_main;
	shader_main.loadshader("vertex_shader.vert", "fragment_shader.frag");
	shader_floor.loadshader("vertexshader_floor.vert", "fragmentshader_floor.frag");
	//shader_screen.loadshader("vertexshader_screen.vert", "fragmentshader_screen.frag");
	
	init();
	
	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	//	glutMouseWheelFunc(mouseWheel);
	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(display1);
	//glutReshapeFunc(reshape);
	glutIdleFunc(display1);
	//glutTimerFunc(1000 / 60, runMainLoop, 0);

	glutMainLoop();

}

