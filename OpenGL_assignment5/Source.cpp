#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC
#include<gl/glew.h>

#include"shader.h"
#include<GL/freeglut.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<vector>



shader shader_main;
//GLuint g_ShaderProgram = 0;
//glGenVertexArrays(1, &VertexArrayID);

GLuint vao_square;
GLuint vbo_square;

glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 40.0f, beta = 45.0f;
float r = 5.25f;

// Light attributes
glm::vec3 lightPos(1.5f, 1.5f, 2.0f);





void display1()
{

	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Projection = glm::perspective(glm::radians(90.0f), (float)640 / (float)480, 0.1f, 100.0f);
	View = glm::lookAt(glm::vec3(0,0,-5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	shader_main.Use();

	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "view"), 1, GL_FALSE, glm::value_ptr(View));

	//head
	Model = glm::mat4();
	Model = glm::translate(glm::vec3(0, 3, 0));

	glUniform3f(glGetUniformLocation(shader_main.program, "colors"),1,0,0);
	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glBindVertexArray(vao_square);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	//body
	Model = glm::mat4();
	Model = glm::translate(glm::vec3(0, 2, 0));
	Model += glm::scale(glm::vec3(4, 5, 0));

	glUniform3f(glGetUniformLocation(shader_main.program, "colors"), .2, .2, .2);

	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glBindVertexArray(vao_square);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	//Leg_R
	Model = glm::mat4();
	Model = glm::translate(glm::vec3(-1, -3, 0));
	Model += glm::scale(glm::vec3(.7, 4, 0));

	glUniform3f(glGetUniformLocation(shader_main.program, "colors"), 0, 0, 1);

	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glBindVertexArray(vao_square);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//Leg_L
	Model = glm::mat4();
	Model = glm::translate(glm::vec3(1, -3, 0));
	Model += glm::scale(glm::vec3(.7, 4, 0));

	glUniform3f(glGetUniformLocation(shader_main.program, "colors"), 0, 0, 1);

	glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glBindVertexArray(vao_square);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);




	glutSwapBuffers();

}


void init() {


	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);



	// some GL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	glViewport(0, 0, 640, 480);

	//Not neccessary to be written inside init(),usually written in main()

	GLfloat square[] = {
     -0.5f,0.5f,
	  0.5f,0.5f,
	  0.5f,-0.5f,

	  0.5f,-0.5f,
	 -0.5f,-0.5f,
	 -0.5f,0.5f		
	};
	/*
	GLfloat square[] = {
		-.5f, .5f, -2, 
		.5, .5f, -2,
		.5f,-.5f, -2,
		1,-1, -1,
		-1,-1, -1,	
		-1, 1, -1,
	};*/


	//vao square
	glGenVertexArrays(1,&vao_square);
	glGenBuffers(1, &vbo_square);
	glBindVertexArray(vao_square);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_square);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 *sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);




}
//void runMainLoop(int val) {
//	display1();
//	glutTimerFunc(1000 / 60, runMainLoop, 0);
//}

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

int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("simple rectangle");


	glewExperimental = true; // Needed in core profile


	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	//shader shader_main;
	shader_main.loadshader("vertex_shader.vert", "fragment_shader.frag");

	init();

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	//	glutMouseWheelFunc(mouseWheel);
	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(display1);

	//glutTimerFunc(1000 / 60, runMainLoop, 0);

	glutMainLoop();

}

