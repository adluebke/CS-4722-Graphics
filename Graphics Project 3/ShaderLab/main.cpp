#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "Shader.h"
#include "Model.h"
#include "MathHelper.h"

Model* sphereModel;		
Model* sphereModel2;
Model* sphereModel3;
Model* sphereModel4;

GLfloat* rotXMatrix;	// All of these are temporary matrices
GLfloat* rotYMatrix;
GLfloat* rotZMatrix;
GLfloat* transMatrix;
GLfloat* scaleMatrix;
GLfloat* tempMatrix;
GLfloat* tempMatrix2;

GLfloat* V;				// View Matrix
GLfloat* P;				// Perspective Matrix

GLfloat lightPos[] = { 0.0f, 10.0f, 10.0f, 0.0f };
GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };	// Not used in the shader, but you could
GLfloat lightDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };	// Not used in the shader, but you could
GLfloat lightSpecular[] = { 0.9f, 0.9f, 0.9f, 1.0f };	// Not used in the shader, but you could

GLfloat matAmbient[] = { 0.1f, 0.05f, 0.05f, 1.0f };
GLfloat matDiffuse[] = { 1.0f, 0.01f, 0.01f, 1.0f };
GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

bool one, two, three, four;
float bumpFactor = 0.01f;

#pragma region SETTING_VARIABLES
// Clearly these could be done with just one function, 
// but I wanted you to see the pattern.  If you pass
// a shader ID and the value you want to pass, it'll set it.

void setRInShader(GLuint shaderID, GLfloat* matrix)
{
	int ID = glGetUniformLocation(shaderID, "mR");
	glUniformMatrix4fv(ID, 1, GL_TRUE, matrix);
}

void setVInShader(GLuint shaderID, GLfloat* matrix)
{
	int ID = glGetUniformLocation(shaderID, "mV");
	glUniformMatrix4fv(ID, 1, GL_TRUE, matrix);
}

void setPInShader(GLuint shaderID, GLfloat* matrix)
{
	int ID = glGetUniformLocation(shaderID, "mP");
	glUniformMatrix4fv(ID, 1, GL_TRUE, matrix);
}

void setLightPositionInShader(GLuint shaderID, GLfloat* vec4)
{
	int ID = glGetUniformLocation(shaderID, "lightPosition");
	glUniform4fv(ID, 1, vec4);
}

void setAmbientInShader(GLuint shaderID, GLfloat* vec4)
{
	int ID = glGetUniformLocation(shaderID, "ambient");
	glUniform4fv(ID, 1, vec4);
}

void setDiffuseInShader(GLuint shaderID, GLfloat* vec4)
{
	int ID = glGetUniformLocation(shaderID, "diffuse");
	glUniform4fv(ID, 1, vec4);
}

void setSpecularInShader(GLuint shaderID, GLfloat* vec4)
{
	int ID = glGetUniformLocation(shaderID, "specular");
	glUniform4fv(ID, 1, vec4);
}

void setBumpinessInShader(GLuint shaderID, GLfloat x)
{
	int ID = glGetUniformLocation(shaderID, "bumpValue");
	glUniform1f(ID, x);
}
#pragma endregion SETTING_VARIABLES

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float scale = 1.0f;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	
	// Sphere one - the diffuse sphere
	if (one) {
		// Activate the first shader
		glUseProgram(sphereModel->shader->programID);
		// Set V and P in the shader
		setVInShader(sphereModel->shader->programID, V);
		setPInShader(sphereModel->shader->programID, P);
		// Set (R)otation in the shader called mR
		MathHelper::makeIdentity(tempMatrix);
		setRInShader(sphereModel2->shader->programID, tempMatrix);
		// Set the diffuse material in the fragment shader
		setDiffuseInShader(sphereModel->shader->programID, matDiffuse);
		// Set the light position in the vert shader
		setLightPositionInShader(sphereModel->shader->programID, lightPos);
		// Construct M
		MathHelper::makeScale(scaleMatrix, scale, scale, scale);
		MathHelper::makeRotateY(rotYMatrix, 0);
		MathHelper::matrixMult4x4(tempMatrix, scaleMatrix, rotYMatrix);
		MathHelper::makeTranslate(tempMatrix2, 0, 0, -2.0f);
		// Copy M data into the mM variable in the model.  This is copied to the shader during render()
		MathHelper::matrixMult4x4(sphereModel->mM, tempMatrix2, tempMatrix);
		sphereModel->render();
	}
	// Second sphere - using textures for some lighting tricks
	if (two) {
		bumpFactor += 0.001f;		// This variable is incremented each frame and passed to the shader
		// Activate the second shader
		glUseProgram(sphereModel2->shader->programID);
		// Sets the V and P matrices in the shader 
		setVInShader(sphereModel2->shader->programID, V);
		setPInShader(sphereModel2->shader->programID, P);

		GLfloat newlightPos[] = { 0.0f, 3.0f, 10.0f, 0.0f };
		// Set the light position (called "lightPosition") in the shader
		setLightPositionInShader(sphereModel2->shader->programID, newlightPos);
		setBumpinessInShader(sphereModel2->shader->programID, bumpFactor);
		// Set (R)otation
		MathHelper::makeIdentity(tempMatrix);
		setRInShader(sphereModel2->shader->programID, tempMatrix);

		GLfloat newSpec[] = { -1.0f, -0.01f, -0.01f, 1.0f };
		// Set Diffuse, Specular, Ambient
		setAmbientInShader(sphereModel2->shader->programID, matAmbient);
		setDiffuseInShader(sphereModel2->shader->programID, matDiffuse);
		setSpecularInShader(sphereModel2->shader->programID, newSpec);

		// Begin constructing M
		MathHelper::makeScale(scaleMatrix, scale, scale, scale);
		MathHelper::makeRotateX(rotYMatrix, 0);
		MathHelper::matrixMult4x4(tempMatrix, scaleMatrix, rotYMatrix);
		MathHelper::makeTranslate(tempMatrix2, 0, 0, -2.0f);
		// Put M into the model's mM variable.  This is copied to the shader during render()
		MathHelper::matrixMult4x4(sphereModel2->mM, tempMatrix2, tempMatrix);
		sphereModel2->render();
	}

	// Standard Blinn/Phong shader
	if (three) {
		// Activate the third shader
		glUseProgram(sphereModel3->shader->programID);
		// Set V and P in the shader
		setVInShader(sphereModel3->shader->programID, V);
		setPInShader(sphereModel3->shader->programID, P);
		// Set the light position (called "lightPosition") in the shader 
		setLightPositionInShader(sphereModel3->shader->programID, lightPos);
		// Set (R)otation
		MathHelper::makeIdentity(tempMatrix);
		setRInShader(sphereModel3->shader->programID, tempMatrix);

		// Set Diffuse, Specular, Ambient
		setAmbientInShader(sphereModel3->shader->programID, matAmbient);
		setDiffuseInShader(sphereModel3->shader->programID, matDiffuse);
		setSpecularInShader(sphereModel3->shader->programID, matSpecular);
		// Construct M
		MathHelper::makeScale(scaleMatrix, scale, scale, scale);
		MathHelper::makeRotateY(rotYMatrix, 0);
		MathHelper::matrixMult4x4(tempMatrix, scaleMatrix, rotYMatrix);
		MathHelper::makeTranslate(tempMatrix2, 0, 0, -2.0f);
		// Copy M data into the mM variable in the model.  This is copied to the shader during render()
		MathHelper::matrixMult4x4(sphereModel3->mM, tempMatrix2, tempMatrix);
		sphereModel3->render();
	}

	if (four) {
		// Activate the third shader
		glUseProgram(sphereModel4->shader->programID);
		// Set V and P in the shader
		setVInShader(sphereModel4->shader->programID, V);
		setPInShader(sphereModel4->shader->programID, P);
		// Set the light position (called "lightPosition") in the shader 
		setLightPositionInShader(sphereModel4->shader->programID, lightPos);
		// Set (R)otation
		MathHelper::makeIdentity(tempMatrix);
		setRInShader(sphereModel4->shader->programID, tempMatrix);

		GLfloat newDif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		// Set Diffuse, Specular, Ambient
		setAmbientInShader(sphereModel4->shader->programID, matAmbient);
		setDiffuseInShader(sphereModel4->shader->programID, newDif);
		setSpecularInShader(sphereModel4->shader->programID, matSpecular);
		// Construct M
		MathHelper::makeScale(scaleMatrix, scale, scale, scale);
		MathHelper::makeRotateY(rotYMatrix, 0);
		MathHelper::matrixMult4x4(tempMatrix, scaleMatrix, rotYMatrix);
		MathHelper::makeTranslate(tempMatrix2, 0, 0, -2.0f);
		// Copy M data into the mM variable in the model.  This is copied to the shader during render()
		MathHelper::matrixMult4x4(sphereModel4->mM, tempMatrix2, tempMatrix);
		sphereModel4->render();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

// Any time the window is resized, this function gets called.  It's setup to the 
// "glutReshapeFunc" in main.
void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}

void generateSphere(double resolution, int* numVerts, GLfloat** verts, GLfloat** normals, GLfloat** texture_coords) {
	float steps = (float)resolution;
	double c = 3.1415926 / 180.0;
	//double x, y, z;

	*numVerts = 360 / resolution * 360 / resolution * 6;
	// Allocate space
	*verts = new GLfloat[*numVerts * 3];
	*normals = new GLfloat[*numVerts * 3];
	*texture_coords = new GLfloat[*numVerts * 3];

	for (int i = 0; i < *numVerts; i++) {
		(*verts)[i] = 0.0f;
		(*normals)[i] = 0.0f;
		(*texture_coords)[i] = 0.0f;
	}

	int vert_count = 0;
	int uv_count = 0;
	float x, y;
	float PI = 3.1515926;
	// back slice to front slice (starting z deep)
	for (double phi = -90.0; phi <= 90.0; phi += steps) { // Rotating around the y axis

		// circle around z axis
		for (GLfloat theta = 0.0; theta <= 360.0; theta += steps) { // Rotating around the z axis - starting on the right
			// 1
			(*verts)[vert_count] = sin(c*theta)*cos(c*(phi + steps));
			(*verts)[vert_count + 1] = cos(c*theta)*cos(c*(phi + steps));
			(*verts)[vert_count + 2] = sin(c*(phi + steps));
			(*normals)[vert_count] = (*verts)[vert_count];
			(*normals)[vert_count + 1] = (*verts)[vert_count + 1];
			(*normals)[vert_count + 2] = (*verts)[vert_count + 2];
			// calculate the angle of it's xy position relative to x axis
			x = (*verts)[vert_count];
			y = (*verts)[vert_count + 1];
			(*texture_coords)[uv_count] = (atan2(y, x) + PI) / (2 * PI); // range between 0 and 1;
			(*texture_coords)[uv_count + 1] = ((*verts)[vert_count + 2] + 1.0f) / 2.0f;

			// 2
			(*verts)[vert_count + 3] = sin(c*theta)*cos(c*phi); // x
			(*verts)[vert_count + 4] = cos(c*theta)*cos(c*phi); // y
			(*verts)[vert_count + 5] = sin(c*phi);		// z
			(*normals)[vert_count + 3] = (*verts)[vert_count + 3];
			(*normals)[vert_count + 4] = (*verts)[vert_count + 4];
			(*normals)[vert_count + 5] = (*verts)[vert_count + 5];
			x = (*verts)[vert_count + 3];
			y = (*verts)[vert_count + 4];
			(*texture_coords)[uv_count + 2] = (atan2(y, x) + PI) / (2 * PI); // range between 0 and 1;
			(*texture_coords)[uv_count + 3] = ((*verts)[vert_count + 5] + 1.0f) / 2.0f;

			// 3
			theta -= steps;
			(*verts)[vert_count + 6] = sin(c*theta)*cos(c*(phi));
			(*verts)[vert_count + 7] = cos(c*theta)*cos(c*(phi));
			(*verts)[vert_count + 8] = sin(c*(phi));
			(*normals)[vert_count + 6] = (*verts)[vert_count + 6];
			(*normals)[vert_count + 7] = (*verts)[vert_count + 7];
			(*normals)[vert_count + 8] = (*verts)[vert_count + 8];
			x = (*verts)[vert_count + 6];
			y = (*verts)[vert_count + 7];
			(*texture_coords)[uv_count + 4] = (atan2(y, x) + PI) / (2 * PI); // range between 0 and 1;
			(*texture_coords)[uv_count + 5] = ((*verts)[vert_count + 8] + 1.0f) / 2.0f;
			theta += steps;

			// 4
			theta -= steps;
			phi += steps;
			(*verts)[vert_count + 9] = sin(c*theta)*cos(c*(phi));
			(*verts)[vert_count + 10] = cos(c*theta)*cos(c*(phi));
			(*verts)[vert_count + 11] = sin(c*(phi));
			(*normals)[vert_count + 9] = (*verts)[vert_count + 9];
			(*normals)[vert_count + 10] = (*verts)[vert_count + 10];
			(*normals)[vert_count + 11] = (*verts)[vert_count + 11];
			x = (*verts)[vert_count + 9];
			y = (*verts)[vert_count + 10];
			(*texture_coords)[uv_count + 6] = (atan2(y, x) + PI) / (2 * PI); // range between 0 and 1;
			(*texture_coords)[uv_count + 7] = ((*verts)[vert_count + 11] + 1.0f) / 2.0f;
			theta += steps;
			phi -= steps;

			// 5
			(*verts)[vert_count + 12] = (*verts)[vert_count];
			(*verts)[vert_count + 13] = (*verts)[vert_count + 1];
			(*verts)[vert_count + 14] = (*verts)[vert_count + 2];
			(*normals)[vert_count + 12] = (*verts)[vert_count + 12];
			(*normals)[vert_count + 13] = (*verts)[vert_count + 13];
			(*normals)[vert_count + 14] = (*verts)[vert_count + 14];
			x = (*verts)[vert_count + 12];
			y = (*verts)[vert_count + 13];
			(*texture_coords)[uv_count + 8] = (atan2(y, x) + PI) / (2 * PI); // range between 0 and 1;
			(*texture_coords)[uv_count + 9] = ((*verts)[vert_count + 14] + 1.0f) / 2.0f;

			// 6 
			theta -= steps;
			(*verts)[vert_count + 15] = (*verts)[vert_count + 6];
			(*verts)[vert_count + 16] = (*verts)[vert_count + 7];
			(*verts)[vert_count + 17] = (*verts)[vert_count + 8];
			(*normals)[vert_count + 15] = (*verts)[vert_count + 15];
			(*normals)[vert_count + 16] = (*verts)[vert_count + 16];
			(*normals)[vert_count + 17] = (*verts)[vert_count + 17];
			x = (*verts)[vert_count + 15];
			y = (*verts)[vert_count + 16];
			(*texture_coords)[uv_count + 10] = (atan2(y, x) + PI) / (2 * PI); // range between 0 and 1;
			(*texture_coords)[uv_count + 11] = ((*verts)[vert_count + 17] + 1.0f) / 2.0f;
			theta += steps;

			vert_count += 18;
			uv_count += 12;
		}
	}

}

// Change the mode to be either the first, second, third or fourth? sphere
void keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		one = true;
		two = three = four = false;
	}
	else if (key == '2') {
		bumpFactor = 0.0f;	// Reset the variable
		two = true;
		one = three = four = false;
	}
	else if (key == '3') {
		three = true;
		one = two = four = false;
	}
	else if (key == '4') {
		four = true;
		one = two = three = false;
	}
}

int main (int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);

	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(600, 600);
	// Create the window with the title "Triangle"
	glutCreateWindow("Project 3");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	
	// Remember, we always init GLEW after we create a window...
	// This asks the GPU driver for all of those gl functions
	if (GLEW_OK != glewInit()) {
		exit(1);
	}
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	// These two commands aren't necessary, but I thought I'd throw them in
	// so you could see the defaults.
	glCullFace(GL_BACK);
	glFrontFace (GL_CCW);

	// Compile our shaders
	const char* vertexProgram = "../ShaderLab/TextureShader3.vsh";
	const char* fragmentProgram = "../ShaderLab/TextureShader3.fsh";
	Shader* myShader = new Shader(vertexProgram, fragmentProgram);

	vertexProgram = "../ShaderLab/DiffuseShader.vsh";
	fragmentProgram = "../ShaderLab/DiffuseShader.fsh";
	Shader* diffuseShader = new Shader(vertexProgram, fragmentProgram);
	
	vertexProgram = "../ShaderLab/BlinnShader.vsh";
	fragmentProgram = "../ShaderLab/BlinnShader.fsh";
	Shader* blinnShader = new Shader(vertexProgram, fragmentProgram);

	vertexProgram = "../ShaderLab/TextureShader.vsh";
	fragmentProgram = "../ShaderLab/TextureShader.fsh";
	Shader* andrewShader = new Shader(vertexProgram, fragmentProgram);

	
	GLfloat* sphereVertices;
	GLfloat* sphereNormals;
	GLfloat* sphereUVs;
	GLint numSphereVerts;
	generateSphere(10, &numSphereVerts, &sphereVertices, &sphereNormals, &sphereUVs);
	
	// Clearly you could optimize by only loading the geometry of one sphere...
	sphereModel = new Model(diffuseShader);
	sphereModel->DEBUG = false;
	sphereModel->setGeometry(sphereVertices, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel->setNormals(sphereNormals, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel->loadVBO();

	// This sphere has texture coordinates and loads a bitmap
	sphereModel2 = new Model(myShader);
	sphereModel2->DEBUG = false;
	sphereModel2->setGeometry(sphereVertices, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel2->setNormals(sphereNormals, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel2->setTextureCoordinates(sphereUVs, numSphereVerts * 2 * sizeof(GLfloat));
	sphereModel2->loadBitmap("../ShaderLab/perlin.bmp"); // Maybe try perlin2?
	sphereModel2->loadVBO();

	sphereModel3 = new Model(blinnShader);
	sphereModel3->DEBUG = false;
	sphereModel3->setGeometry(sphereVertices, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel3->setNormals(sphereNormals, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel3->loadVBO();

	// Clearly you could optimize by only loading the geometry of one sphere...
	sphereModel4 = new Model(andrewShader);
	sphereModel4->DEBUG = false;
	sphereModel4->setGeometry(sphereVertices, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel4->setNormals(sphereNormals, numSphereVerts * 3 * sizeof(GLfloat));
	sphereModel4->loadVBO();

	// Allocate memory for the matrices
	rotXMatrix = new GLfloat[16];
	rotYMatrix = new GLfloat[16];
	rotZMatrix = new GLfloat[16];
	transMatrix = new GLfloat[16];
	scaleMatrix = new GLfloat[16];
	tempMatrix = new GLfloat[16];
	tempMatrix2 = new GLfloat[16];
	V = new GLfloat[16];
	P = new GLfloat[16];

	// Set up the View matrix
	MathHelper::makeIdentity(V);

	// Set up the Perspective matrix
	MathHelper::makePerspective(P, 60, 1.0f, 1.0f, 1000.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	one = two = three = four = false;
	one = true;

	return 0;
}