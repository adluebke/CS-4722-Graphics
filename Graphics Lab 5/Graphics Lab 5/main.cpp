#include <GL/GLEW.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate/rotate/scale/etc
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

glm::mat4 M;
glm::mat4 V;
glm::mat4 P;
glm::mat4 trans;
glm::mat4 rot;
float rotAmount = 0.0f;
GLint umM;
GLint umV;
GLint umP;

void render() {

	rotAmount += 0.01f;

	trans = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1));
	rot = glm::rotate(glm::mat4(1.0f), rotAmount, glm::vec3(0, 1, 0));
	M = trans*rot;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(umM, 1, GL_FALSE, glm::value_ptr(M));
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();
	glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: printf("The UP key was pressed\n"); break;
	case GLUT_KEY_DOWN: printf("The DOWN key was pressed\n"); break;
	}
}

void mousePressed(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		//printf("Mouse clicked at %d %d\n", x, y);
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
	glutCreateWindow("Lab 5");          // Create the window and give it a title

	glewInit();                         // Ask the driver for all the OpenGL functions

										// Some callback functions
	glutDisplayFunc(render);            // Use the render function to draw
	glutSpecialFunc(specialKeys);       // Use the specialKeys function for Up/Down/Left/Right keys
	glutMouseFunc(mousePressed);        // Use for mouse clicks
	glutMotionFunc(mouseDragged);       // Use for mouse dragging
	glutPassiveMotionFunc(mouseMoved); // Use for mouse moving

#pragma region SHADER_STUFF
									   // =========  SHADER STUFF  ===============
	const GLchar* vertexShaderCode = "#version 150\n\
									in vec4 vPosition;\n\
									in vec4 vColor;\n\
									out vec4 color;\n\
									uniform mat4 mM;\n\
									uniform mat4 mV;\n\
									uniform mat4 mP;\n\
									void main () {\n\
										 color = vColor;\n\
										 \tgl_Position = mP*mV*mM*vPosition;\n\
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

	// Make the program
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
#pragma endregion SHADER_STUFF

	// Positions
	GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,  // 0
		-0.25f, 0.0f, 0.0f, // 1
		0.0f, 0.5f, 0.0f, // 2
		0.0f, -0.5f, 0.0f, // 3
		0.25f, 0.0f, 0.0f, // 4
		0.5f, -0.5f, 0.0f, // 5
	};

	// Color information
	GLfloat colors[] = { 1.0f, 0.73f, 0.0f, 1.0f, //0
		1.0f, 1.0f, 0.0f, 1.0f, // 1
		1.0f, 1.0f, 0.0f, 1.0f, // 2
		1.0f, 0.73f, 0.0f, 1.0f, // 3
		1.0f, 0.65f, 0.0f, 1.0f, // 4
		1.0f, 0.65f, 0.0f, 1.0f, // 5
	};

	// Connect the dots
	GLuint index_buffer[] = { 0, 3, 1, 2, 1, 4, 4, 3, 5 };

	int numVertices = 6;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);	// Use the Vertex Array Object we created above

	GLuint vbo;		// The Vertex Buffer Object ID
	glGenBuffers(1, &vbo);	// Ask the GPU driver for a buffer array. "vbo" now has the ID
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	// Make this buffer the active one for subsequent operations (below)

										// Specify how big the buffer is, but don't pass the data yet (see NULL).  We *could* have, but I wanted to show glBufferSubData
	glBufferData(GL_ARRAY_BUFFER, numVertices * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// NOW we copy the data in as a separate step. There is an "offset" of 0 - meaning the beginning of the buffer. 
	// We specify the size of the data we want to copy, followed by a pointer to the actual data
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 4 * sizeof(GLfloat), colors);

	// Figure out where vPosition is in our shader and get its ID
	GLuint loc = glGetAttribLocation(shaderProgram, "vPosition");
	GLuint loc2 = glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(loc);
	glEnableVertexAttribArray(loc2);

	printf("vPosition ID is %d\n", loc);
	printf("vColor ID is %d\n", loc2);

	// When it's time for vPosition to find information, we need to tell it where the data is (or how the data should be parsed)
	// Here, we're saying that "vPosition" (loc from above) should look for 3 GLfloats. The data isn't normalized or interlaced, 
	// and starts at index 0 (the beginning of the current buffer)
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices * 3 * sizeof(GLfloat)));

	GLuint index_vbo;
	// Ask the graphics card (driver) for another buffer – same as the old code
	glGenBuffers(1, &index_vbo);
	// We still want the VAO active to remember these settings
	glBindVertexArray(vao);
	// Here's where we tell the driver that it's an index buffer.  
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	// This time, we'll just go ahead and pass the data off to the buffer because
	// we're not packing multiple data sets into the buffer - only indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9 * sizeof(GLuint), index_buffer, GL_STATIC_DRAW);

	P = glm::perspective(-60.0f, 1.3f, 0.1f, 1000.0f);
	V = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


	umM = glGetUniformLocation(shaderProgram, "mM"); // Find the mM variable
	umV = glGetUniformLocation(shaderProgram, "mV"); // Find the mV variable
	umP = glGetUniformLocation(shaderProgram, "mP"); // Find the mP variable

	if (umP != -1)
	{
		glUniformMatrix4fv(umP, 1, GL_FALSE, glm::value_ptr(P));
	}
	if (umV != -1)
	{
		glUniformMatrix4fv(umV, 1, GL_FALSE, glm::value_ptr(V));
	}

	glutMainLoop();                     // Start listening for events
}