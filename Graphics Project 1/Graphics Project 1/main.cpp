#include <GL/GLEW.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "main.h"

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const GLfloat zVal = 0;

const size_t VERTICES_ARRAY_SIZE = 9;

int NUM_VERTICES = 3;
int windowHeight = 600;
int windowWidth = 800;

bool beenRendered;
bool drawTriangles = true;
bool triangleStrips;
bool triFans;
bool backfaceCulling;
bool wireFrame;

vector<GLfloat> ndcPoints;
GLfloat* ndcPoints1;
vector<GLfloat> colors;
GLfloat* colors1;
//GLfloat colors[] =
//{
//	0.0f, 0.0f, 1.0f, 1.0f,
//	0.0f, 0.0f, 1.0f, 1.0f,
//	0.0f, 0.0f, 1.0f, 1.0f,
//};


void render()
{
	if (drawTriangles)
	{
		//printf("Render was called\n");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers
		size_t vertexVecSize = ndcPoints.size();
		if (backfaceCulling)
			glEnable(GL_CULL_FACE);

		if (wireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if(!wireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (vertexVecSize == VERTICES_ARRAY_SIZE)
		{
			//glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
			beenRendered = true;
			glVertexPointer(3, GL_FLOAT, 0, ndcPoints1);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glutSwapBuffers(); // Since we’re using double-buffering
			glutPostRedisplay(); // Optional, but calls the render method again
		}
	}

	else if (triangleStrips)
	{
		//printf("Render was called\n");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers
		size_t vertexVecSize = ndcPoints.size();

			//glDrawElements(GL_TRIANGLE_STRIP, vertexVecSize, GL_UNSIGNED_INT, NULL);
			//beenRendered = true;
			glVertexPointer(3, GL_FLOAT, 0, ndcPoints1);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, NUM_VERTICES);
			glutSwapBuffers(); // Since we’re using double-buffering
			glutPostRedisplay(); // Optional, but calls the render method again
	}

	else if (triFans)
	{
		//printf("Render was called\n");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers
		size_t vertexVecSize = ndcPoints.size();

			//glDrawElements(GL_TRIANGLE_FAN, vertexVecSize, GL_UNSIGNED_INT, NULL);
			//beenRendered = true;
			glVertexPointer(3, GL_FLOAT, 0, ndcPoints1);
			glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_VERTICES);
			glutSwapBuffers(); // Since we’re using double-buffering
			glutPostRedisplay(); // Optional, but calls the render method again

	}
}

void specialKeys(int key, int x, int y) 
{
	switch (key) 
	{
	//case GLUT_KEY_UP: printf("The UP key was pressed\n"); break;
	//case GLUT_KEY_DOWN: printf("The DOWN key was pressed\n"); break;
	case GLUT_KEY_LEFT:
		printf("The LEFT key was pressed, switching to draw triangles mode.\n");
		drawTriangles = true;
		triangleStrips = false;
		triFans = false;
			ndcPoints.clear();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glutSwapBuffers();
			glLoadIdentity();
		break;

	case GLUT_KEY_DOWN:
		printf("The DOWN key was pressed, switching to triangle strip mode.\n");
		drawTriangles = false;
		triangleStrips = true;
		triFans = false;
			ndcPoints.clear();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glutSwapBuffers();
			glLoadIdentity();
		break;

	case GLUT_KEY_RIGHT:
		printf("The RIGHT key was pressed, switching to triangle fan mode.\n");
		drawTriangles = false;
		triangleStrips = false;
		triFans = true;
			ndcPoints.clear();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glutSwapBuffers();
			glLoadIdentity();
		break;

	case GLUT_KEY_END:
		printf("The END key was pressed, switching to backface culling mode.\n");
		backfaceCulling = true;
			ndcPoints.clear();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glutSwapBuffers();
			glLoadIdentity();
		break;
	case GLUT_KEY_HOME:
		printf("The HOME key was pressed, switching between wireframe and filled modes.\n");
		if (wireFrame)
			wireFrame = false;
		else if (!wireFrame)
			wireFrame = true;
				ndcPoints.clear();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glutSwapBuffers();
				glLoadIdentity();
		break;
	}
}

void mousePressed(int button, int state, int x, int y) 
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) 
	{
		if (drawTriangles)
		{
			if (beenRendered)
			{
				beenRendered = false;
				ndcPoints.clear();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glutSwapBuffers();
				glLoadIdentity();
			}
			else
			{
				NUM_VERTICES = 3;

				GLfloat ndcX = ((GLfloat)x / ((GLfloat)windowWidth / 2.0f)) - 1.0f;
				GLfloat ndcY = (((GLfloat)windowHeight - (GLfloat)y) / ((GLfloat)windowHeight / 2.0f)) - 1.0f;
				printf("Created vertex at Width: %f Height: %f\n", ndcX, ndcY);

				ndcPoints.push_back(ndcX);
				ndcPoints.push_back(ndcY);
				ndcPoints.push_back(zVal);

				colors.push_back(0.0);
				colors.push_back(0.0);
				colors.push_back(1.0);
				colors.push_back(1.0);

				ndcPoints1 = ndcPoints.data();
				colors1 = colors.data();

				glBufferData(GL_ARRAY_BUFFER,
					NUM_VERTICES * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

				glBufferSubData(GL_ARRAY_BUFFER,
					0, NUM_VERTICES * 3 * sizeof(GLfloat), ndcPoints1);

				glBufferSubData(GL_ARRAY_BUFFER,
					NUM_VERTICES * 3 * sizeof(GLfloat), NUM_VERTICES * 4 * sizeof(GLfloat), colors1);
			}
		}

		else if (triangleStrips)
		{
				GLfloat ndcX = ((GLfloat)x / ((GLfloat)windowWidth / 2.0f)) - 1.0f;
				GLfloat ndcY = (((GLfloat)windowHeight - (GLfloat)y) / ((GLfloat)windowHeight / 2.0f)) - 1.0f;
				printf("Created vertex at Width: %f Height: %f\n", ndcX, ndcY);

				ndcPoints.push_back(ndcX);
				ndcPoints.push_back(ndcY);
				ndcPoints.push_back(zVal);

				NUM_VERTICES = ndcPoints.size() / 3;

				colors.push_back(0.0);
				colors.push_back(0.0);
				colors.push_back(1.0);
				colors.push_back(1.0);

				ndcPoints1 = ndcPoints.data();
				colors1 = colors.data();

				glBufferData(GL_ARRAY_BUFFER,
					NUM_VERTICES * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

				glBufferSubData(GL_ARRAY_BUFFER,
					0, NUM_VERTICES * 3 * sizeof(GLfloat), ndcPoints1);

				glBufferSubData(GL_ARRAY_BUFFER,
					NUM_VERTICES * 3 * sizeof(GLfloat), NUM_VERTICES * 4 * sizeof(GLfloat), colors1);
		}

		else if (triFans)
		{

				GLfloat ndcX = ((GLfloat)x / ((GLfloat)windowWidth / 2.0f)) - 1.0f;
				GLfloat ndcY = (((GLfloat)windowHeight - (GLfloat)y) / ((GLfloat)windowHeight / 2.0f)) - 1.0f;
				printf("Created vertex at Width: %f Height: %f\n", ndcX, ndcY);

				ndcPoints.push_back(ndcX);
				ndcPoints.push_back(ndcY);
				ndcPoints.push_back(zVal);

				colors.push_back(0.0);
				colors.push_back(0.0);
				colors.push_back(1.0);
				colors.push_back(1.0);

				ndcPoints1 = ndcPoints.data();
				colors1 = colors.data();

				NUM_VERTICES = ndcPoints.size() / 3;

				glBufferData(GL_ARRAY_BUFFER,
					NUM_VERTICES * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

				glBufferSubData(GL_ARRAY_BUFFER,
					0, NUM_VERTICES * 3 * sizeof(GLfloat), ndcPoints1);

				glBufferSubData(GL_ARRAY_BUFFER,
					NUM_VERTICES * 3 * sizeof(GLfloat), NUM_VERTICES * 4 * sizeof(GLfloat), colors1);

				glVertexPointer(3, GL_FLOAT, 0, ndcPoints1);

		}
	}
}

void mouseMoved(int x, int y) {
	//printf("Mouse moved at %d %d\n", x, y);
}

void mouseDragged(int x, int y) {
	//printf("Mouse dragged at %d %d\n", x, y);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);              // Initialize GLUT
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Set up buffers
	glutInitWindowPosition(200, 200);   // Optional: position the upper-left of the window
	glutInitWindowSize(windowWidth, windowHeight);       // Set the window size
	glutCreateWindow("Project 1");          // Create the window and give it a title

	glewInit();                         // Ask the driver for all the OpenGL functions

										// Some callback functions
	glutDisplayFunc(render);            // Use the render function to draw
	glutSpecialFunc(specialKeys);       // Use the specialKeys function for Up/Down/Left/Right keys
	glutMouseFunc(mousePressed);        // Use for mouse clicks
	glutMotionFunc(mouseDragged);       // Use for mouse dragging
	glutPassiveMotionFunc(mouseMoved); // Use for mouse moving

#pragma region SHADER_STUFF
	const GLchar* vertexShaderCode = "#version 150\n\
									in vec4 vPosition;\n\
									in vec4 vColor;\n\
									out vec4 color;\n\
									void main () {\n\
										color = vColor;\n\
										\tgl_Position = vPosition;\n\
									 }\n";

	const GLchar* fragmentShaderCode = "#version 150\n\n\
										out vec4 fColor;\n\
										in vec4 color;\n\
										void main () {\
										   fColor = color;\n\
							  			}";
	// Vertex Shader 
	GLint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShaderID);
	GLint compiled = -10;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compiled);
	printf("Vertex compile status %d!\n", compiled);
	printf("Vertex shader ID is %d\n", vertexShaderID);

	// Fragment Shader 
	GLint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShaderID);
	GLint compiled2 = -19;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compiled2);

	printf("Fragment compile status %d!\n", compiled2);
	printf("Fragment shader ID is %d\n", fragmentShaderID);

	// Make the program and link it all together
	GLint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);
	glLinkProgram(shaderProgram);
	GLint linkedStatus = 14;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkedStatus);
	printf("Link status is %d\n", linkedStatus);
	printf("Shader program ID is %d\n", shaderProgram);

	glUseProgram(shaderProgram);

	//GLuint index_buffer[] = { 0, 3, 1, 2, 1, 4, 4, 3, 5 };

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;               // The Vertex Buffer Object ID
	glGenBuffers(1, &vbo);     // Ask the GPU driver for a buffer array.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);   // Make this buffer active

	/*glBufferData(GL_ARRAY_BUFFER,
		NUM_VERTICES * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER,
		0, NUM_VERTICES * 3 * sizeof(GLfloat), vertices);

	glBufferSubData(GL_ARRAY_BUFFER,
		NUM_VERTICES * 3 * sizeof(GLfloat), NUM_VERTICES * 4 * sizeof(GLfloat), colors);*/

	GLint attribLocID1 = glGetAttribLocation(shaderProgram, "vPosition");
	glEnableVertexAttribArray(attribLocID1);
	glVertexAttribPointer(attribLocID1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint attribLocID2 = glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(attribLocID2);
	glVertexAttribPointer(attribLocID2, 4,
		GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(NUM_VERTICES * 3 * sizeof(GLfloat)));

	//GLuint index_vbo;
	/*glGenBuffers(1, &index_vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), index_buffer, GL_STATIC_DRAW);*/

	glutMainLoop();                      // Start listening for events

}