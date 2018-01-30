#include <GL/GLEW.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
int NUM_VERTICES = 6;
int numIndices = 9;

void render() {
	//printf("Render was called\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL); // Draw three vertices
	glutSwapBuffers(); // Since we’re using double-buffering
	glutPostRedisplay(); // Optional, but calls the render method again

}

void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: printf("The UP key was pressed\n"); break;
	case GLUT_KEY_DOWN: printf("The DOWN key was pressed\n"); break;
	}
}

void mousePressed(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		printf("Mouse clicked at %d %d\n", x, y);
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
	glutInitWindowSize(800, 600);       // Set the window size
	glutCreateWindow("Lab 4");          // Create the window and give it a title

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

	GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,
						   -0.25f, 0.0f, 0.0f,
							0.0f, 0.5f, 0.0f,
							0.0f, -0.5f, 0.0f,
							0.25f, 0.0f, 0.0f,
							0.5f, -0.5f, 0.0f,
	};

	GLfloat colors[] = {    1.0f, 1.0f, 0.0f, 1.0f,
							0.0f, 1.0f, 1.0f, 1.0f,
							0.0f, 0.0f, 1.0f, 1.0f,
							1.0f, 1.0f, 1.0f, 1.0f,
							1.0f, 0.0f, 0.0f, 1.0f,
							0.0f, 0.25f, 0.25f, 1.0f
	};

	GLuint index_buffer[] = { 0, 3, 1, 2, 1, 4, 4, 3, 5 };

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;               // The Vertex Buffer Object ID
	glGenBuffers(1, &vbo);     // Ask the GPU driver for a buffer array.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);   // Make this buffer active

	glBufferData(GL_ARRAY_BUFFER,
		NUM_VERTICES * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER,
		0, NUM_VERTICES * 3 * sizeof(GLfloat), vertices);

	glBufferSubData(GL_ARRAY_BUFFER,
		NUM_VERTICES * 3 * sizeof(GLfloat), NUM_VERTICES * 4 * sizeof(GLfloat), colors);

	GLint attribLocID1 = glGetAttribLocation(shaderProgram, "vPosition");
	glEnableVertexAttribArray(attribLocID1);
	glVertexAttribPointer(attribLocID1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint attribLocID2 = glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(attribLocID2);
	glVertexAttribPointer(attribLocID2, 4,
		GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(NUM_VERTICES * 3 * sizeof(GLfloat)));

	GLuint index_vbo;
	glGenBuffers(1, &index_vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), index_buffer, GL_STATIC_DRAW);

	glutMainLoop();                      // Start listening for events

}