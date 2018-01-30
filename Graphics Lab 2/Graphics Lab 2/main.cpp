#include <GL/GLEW.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void render() {
	//printf("Render was called\n");
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


int main(int argc, char** argv)
{
	glutInit(&argc, argv);               // Initialize GLUT
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Set up buffers
	glutInitWindowPosition(200, 200);   // Optional: position the upper-left of the window
	glutInitWindowSize(800, 600);        // Set the window size
	glutCreateWindow("Lab 2");          // Create the window and give it a title

	glewInit();                          // Ask the driver for all the OpenGL functions

										 // Some callback functions
	glutDisplayFunc(render);             // Use the render function to draw
	glutSpecialFunc(specialKeys);        // Use the specialKeys function for Up/Down/Left/Right keys
	glutMouseFunc(mousePressed);        // Use for mouse clicks
	glutMotionFunc(mouseDragged);        // Use for mouse dragging
	glutPassiveMotionFunc(mouseMoved); // Use for mouse moving


	const GLchar* vertexShaderCode = "#version 130\nin vec4 vPosition; \n\n\
										void main() {\n\
										\tgl_Position = vPosition;\n\
													   }\n";
	const GLchar* fragmentShaderCode = "#version 130\n\nout vec4 fColor; \
									   void main () {\
									   fColor = vec4(1.0, 0.0, 0.0, 1.0);\
									   }";

	GLint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShaderID);
	GLint compiled = -3;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compiled);
	cout << "Vertex compile status: " << compiled << endl;
	cout << "Vertex shader ID: " << vertexShaderID << endl;

	GLint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShaderID);
	GLint compiled2 = -5;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compiled2);
	cout << "Fragment compile status: " << compiled2 << endl;
	cout << "Fragment shader ID: " << fragmentShaderID << endl;

	GLint programID = glCreateProgram();
	glAttachShader (programID, vertexShaderID);
	glAttachShader (programID, fragmentShaderID);
	glLinkProgram (programID);
	GLint linkedStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkedStatus);
	cout << "Link status: " << compiled << endl;
	cout << "Shader Program ID: " << vertexShaderID << endl;

	glutMainLoop();                      // Start listening for events

}
