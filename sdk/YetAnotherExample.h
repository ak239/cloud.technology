#pragma once

#include "gl.h"
#include "ContextCallbacks.h"
#include "Texture.h"
#include "ControlledCamera.h"
#include "FSShaderProgramLoader.h"
#include "TransformMatrix.h"
#include "UniformWrapper.h"

#include <glm\ext.hpp>

static const double PI = 3.14159265358979323846;

class ArcBall {
public:
	ArcBall() : mWidth(0), mHeight(0) {}

	void Begin(int x, int y);
	void End();
	void Drag(int x, int y);
	void SetWindowSize(int width, int height);
	const glm::mat3 Rotation() const { return glm::mat3_cast(mQuatNow); }

private:
	glm::vec3 ScreenToVector(float screenX, float screenY);

	int mWidth, mHeight;
	bool mDragging;
	glm::quat mQuatNow, mQuatDown;
	glm::vec3 mVecNow, mVecDown;
};

typedef struct _GLshape
{
    GLfloat* vertices;
    GLfloat* normals;
    GLfloat* tangents;
    GLfloat* texCoords;
    GLuint* indices;
    GLuint numberVertices;
    GLuint numberIndices;
    GLenum mode;
} GLshape;

GLvoid glDestroyShapef(GLshape* shape)
{
	if (!shape)
	{
		return;
	}

	if (shape->vertices)
	{
		free(shape->vertices);

		shape->vertices = 0;
	}

	if (shape->normals)
	{
		free(shape->normals);

		shape->normals = 0;
	}

	if (shape->tangents)
	{
		free(shape->tangents);

		shape->tangents = 0;
	}

	if (shape->texCoords)
	{
		free(shape->texCoords);

		shape->texCoords = 0;
	}

	if (shape->indices)
	{
		free(shape->indices);

		shape->indices = 0;
	}

	shape->numberVertices = 0;
	shape->numberIndices = 0;
	shape->mode = 0;
}

GLboolean glCreateSpheref(GLshape* shape, const GLfloat radius, const GLuint numberSlices)
{
	GLuint i, j;

	GLuint numberParallels = numberSlices / 2;
	GLuint numberVertices = (numberParallels + 1) * (numberSlices + 1);
	GLuint numberIndices = numberParallels * numberSlices * 6;

	GLfloat angleStep = (2.0f * (float)PI) / ((GLfloat) numberSlices);

	GLuint indexIndices;

	// used later to help us calculating tangents vectors	

	if (!shape)
	{
		return GL_FALSE;
	}
	
	memset(shape, 0, sizeof(GLshape));

	shape->mode = GL_TRIANGLES;

	shape->numberVertices = numberVertices;
	shape->numberIndices = numberIndices;

	shape->vertices = (GLfloat*) malloc(4 * numberVertices * sizeof(GLfloat));
	shape->normals = (GLfloat*) malloc(3 * numberVertices * sizeof(GLfloat));
	shape->tangents = (GLfloat*) malloc(3 * numberVertices * sizeof(GLfloat));
	shape->texCoords = (GLfloat*) malloc(2 * numberVertices * sizeof(GLfloat));
	shape->indices = (GLuint*) malloc(numberIndices * sizeof(GLuint));

	for (i = 0; i < numberParallels + 1; i++)
	{
		for (j = 0; j < numberSlices + 1; j++)
		{
			GLuint vertexIndex = (i * (numberSlices + 1) + j) * 4;
			GLuint normalIndex = (i * (numberSlices + 1) + j) * 3;
			GLuint tangentIndex = (i * (numberSlices + 1) + j) * 3;
			GLuint texCoordsIndex = (i * (numberSlices + 1) + j) * 2;

			shape->vertices[vertexIndex + 0] = radius * sinf(angleStep * (GLfloat) i) * sinf(angleStep * (GLfloat) j);
			shape->vertices[vertexIndex + 1] = radius * cosf(angleStep * (GLfloat) i);
			shape->vertices[vertexIndex + 2] = radius * sinf(angleStep * (GLfloat) i) * cosf(angleStep * (GLfloat) j);
			shape->vertices[vertexIndex + 3] = 1.0f;

			shape->normals[normalIndex + 0] = shape->vertices[vertexIndex + 0] / radius;
			shape->normals[normalIndex + 1] = shape->vertices[vertexIndex + 1] / radius;
			shape->normals[normalIndex + 2] = shape->vertices[vertexIndex + 2] / radius;

			shape->texCoords[texCoordsIndex + 0] = (GLfloat) j / (GLfloat) numberSlices;
			shape->texCoords[texCoordsIndex + 1] = 1.0f - (GLfloat) i / (GLfloat) numberParallels;

			// use quaternion to get the tangent vector
			glm::quat helpQuaternion = glm::rotate(glm::quat(), 360.0f * shape->texCoords[texCoordsIndex + 0], glm::vec3(0, 1, 0));
			glm::mat3 helpMatrix = glm::mat3( glm::mat4_cast(helpQuaternion));
			glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f) * helpMatrix;
			
			for (int k = 0; k < 3; k++) {
				(&shape->tangents[tangentIndex])[k] = tangent[k];
			}
		}
	}

	indexIndices = 0;
	for (i = 0; i < numberParallels; i++)
	{
		for (j = 0; j < numberSlices; j++)
		{
			shape->indices[indexIndices++] = i * (numberSlices + 1) + j;
			shape->indices[indexIndices++] = (i + 1) * (numberSlices + 1) + j;
			shape->indices[indexIndices++] = (i + 1) * (numberSlices + 1) + (j + 1);

			shape->indices[indexIndices++] = i * (numberSlices + 1) + j;
			shape->indices[indexIndices++] = (i + 1) * (numberSlices + 1) + (j + 1);
			shape->indices[indexIndices++] = i * (numberSlices + 1) + (j + 1);
		}
	}

	return GL_TRUE;
}

struct MaterialProperties {
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	GLfloat Shininess;
};

struct LightProperties {
	glm::vec4 position;
	glm::vec3 la;
	glm::vec3 ld;
	glm::vec3 ls;
};

struct MaterialLocations {
	GLint Ka;
	GLint Kd;
	GLint Ks;
	GLint Shininess;
};

struct LightLocations
{
	GLint Position;
	GLint La;
	GLint Ld;
	GLint Ls;
};

class YetAnotherExample : public ContextCallbacks {
public:
	YetAnotherExample() : camera(nullptr), var(0.0f) {
	}

	~YetAnotherExample(){
		delete camera;
	}

	bool hasIdleFunc() { return false; }

private:

	void initImpl(){
		int width  = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		aspect = static_cast<GLfloat>(width) / height;
		// create camera
	
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

		camera = new ControlledCamera(width, height);
		camera->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		camera->setUp(glm::vec3(0.0f, 1.0f, 0.0f));

		fprintf(
			stdout,
			"INFO: OpenGL Version: %s\n",
			glGetString(GL_VERSION)
			);

		// setup gl
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		ProjectionMatrix = glm::mat4();
		ModelMatrix = glm::mat4();
		ViewMatrix = glm::translate(glm::mat4(), glm::vec3(0,0, -2));


		// load shaders
		loader = new FSShaderProgramLoader("./yetanotherexample");
		if (!loader->loadProgram(getContext())){
			std::cerr << loader->getLastError() << std::endl;
			ExitOnGLError("Something happened during loading shaders");
		}

		program = &loader->getProgram();

		ModelMatrixUniformLocation = program->getUniformLoc("ModelMatrix");
		ViewMatrixUniformLocation = program->getUniformLoc("ViewMatrix");
		ProjectionMatrixUniformLocation = program->getUniformLoc("ProjectionMatrix");
		NormalMatrixLocation = program->getUniformLoc("NormalMatrix");


		LightLocation.Position = program->getUniformLoc("Light.Position");
		LightLocation.La = program->getUniformLoc("Light.La");
		LightLocation.Ld = program->getUniformLoc("Light.Ld");
		LightLocation.Ls = program->getUniformLoc("Light.Ls");

		MaterialLocation.Ka = program->getUniformLoc("Material.Ka");
		MaterialLocation.Kd = program->getUniformLoc("Material.Kd");
		MaterialLocation.Ks = program->getUniformLoc("Material.Ks");
		MaterialLocation.Shininess = program->getUniformLoc("Material.Shininess");

		ExitOnGLError("ERROR: Could not get shader uniform locations");


		createBuffers();
	}

	void mouseImpl(int button, int state, int x, int y) {
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				arcBall.Begin(x, y);
			}
			else if (state == GLUT_DOWN)
			{
				arcBall.End();
			}

		}
	}
	
	void motionImpl(int x, int y) {
		arcBall.Drag(x, y);
	}
	

	void displayImpl() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ModelMatrix = glm::mat4(arcBall.Rotation());
		

		program->use();
		ExitOnGLError("ERROR: Could not use the shader program");

		glm::mat3 NormalMatrix = glm::mat3(ViewMatrix*ModelMatrix);

		glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix3fv(NormalMatrixLocation, 1, GL_FALSE, &NormalMatrix[0][0]);
		ExitOnGLError("ERROR: Could not set the shader uniforms. Matrices.");

		glBindVertexArray(BufferIds[0]);
		ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

		glDrawElements(GL_TRIANGLES, numberIndices, GL_UNSIGNED_INT, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not draw the cube");

		glBindVertexArray(0);

		glUseProgram(0);
	}


	void updateLight() {
		LightProperties Light = {glm::vec4(2.0f), glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(1.0f)};


		// Set up light ...
		glUniform4fv(LightLocation.Position, 1, &Light.position[0]);
		glUniform3fv(LightLocation.La, 1, &Light.la[0]);
		glUniform3fv(LightLocation.Ld, 1, &Light.ld[0]);
		glUniform3fv(LightLocation.Ls, 1, &Light.ls[0]);
		ExitOnGLError("ERROR: Could not set update uniform locations for light");
	}

	void updateMaterial() {
		MaterialProperties Material;
		Material.Ka = glm::vec3(0.4f);
		Material.Kd = glm::vec3(0.0f, 0.2f, 0.4f);
		Material.Ks = glm::vec3(1.0f, 1.0f, 1.0f);
		Material.Shininess = 20.0f;


		// ... and material values.
		glUniform3fv(MaterialLocation.Ka, 1, &Material.Ka[0]);
		glUniform3fv(MaterialLocation.Kd, 1, &Material.Kd[0]);
		glUniform3fv(MaterialLocation.Ks, 1, &Material.Ks[0]);
		glUniform1f(MaterialLocation.Shininess, Material.Shininess);

		ExitOnGLError("ERROR: Could not set update uniform locations for material");
	}


	void createBuffers() {
		GLshape sphere;
		glCreateSpheref(&sphere, 1.0f, 32);

		glGenVertexArrays(1, &BufferIds[0]);
		ExitOnGLError("ERROR: Could not generate the VAO");
		glBindVertexArray(BufferIds[0]);
		ExitOnGLError("ERROR: Could not bind the VAO");

		glGenBuffers(5, &BufferIds[1]);
		ExitOnGLError("ERROR: Could not generate the buffer objects");

		glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
		glBufferData(GL_ARRAY_BUFFER, sphere.numberVertices * 4 * sizeof(GLfloat), sphere.vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not set VAO attributes. Vertices");

		glBindBuffer(GL_ARRAY_BUFFER, BufferIds[2]);
		glBufferData(GL_ARRAY_BUFFER, sphere.numberVertices * 3 * sizeof(GLfloat), sphere.normals, GL_STATIC_DRAW);
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not set VAO attributes. Normals");

		glBindBuffer(GL_ARRAY_BUFFER, BufferIds[4]);
		glBufferData(GL_ARRAY_BUFFER, sphere.numberVertices * 2 * sizeof(GLfloat), sphere.texCoords, GL_STATIC_DRAW);
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not set VAO attributes. Texture coords");

		glBindBuffer(GL_ARRAY_BUFFER, BufferIds[5]);
		glBufferData(GL_ARRAY_BUFFER, sphere.numberVertices * 3 * sizeof(GLfloat), sphere.tangents, GL_STATIC_DRAW);
		ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not set VAO attributes. Tangents");

		numberIndices = sphere.numberIndices;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.numberIndices * sizeof(GLfloat), sphere.indices, GL_STATIC_DRAW);
		ExitOnGLError("ERROR: Could not bind the IBO to the VAO");


	}

	void reshapeImpl(int width, int height) {
		glViewport(0, 0, width, height);
		arcBall.SetWindowSize(width, height);
		aspect = static_cast<GLfloat>(width) / height;
		ProjectionMatrix = glm::perspective(60.0f, aspect, 1.0f, 100.0f);

		program->use();
		glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
		glUseProgram(0);
	}

	void ExitOnGLError(const char* error_message)
	{
		const GLenum ErrorValue = glGetError();

		if (ErrorValue != GL_NO_ERROR)
		{
			const char* APPEND_DETAIL_STRING = ": %s\n";
			const size_t APPEND_LENGTH = strlen(APPEND_DETAIL_STRING) + 1;
			const size_t message_length = strlen(error_message);
			char* display_message = (char*)malloc(message_length + APPEND_LENGTH);

			memcpy(display_message, error_message, message_length);
			memcpy(&display_message[message_length], APPEND_DETAIL_STRING, APPEND_LENGTH);

			fprintf(stderr, display_message, gluErrorString(ErrorValue));

			free(display_message);
			exit(EXIT_FAILURE);
		}
	}


private:
	ControlledCamera* camera;
	const ShaderProgram* program;
	FSShaderProgramLoader* loader;
	ArcBall arcBall;

	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	GLint numberIndices;

	glm::mat4 
		ProjectionMatrix,
		ViewMatrix,
		ModelMatrix;


	UniformWrapper<glm::mat4> gWVP;
	UniformWrapper<glm::mat4> gWorld;
	UniformWrapper<GLuint>    gSampler;
	UniformWrapper<glm::vec3> gLightColor;
	UniformWrapper<GLfloat>   gLightIntensity;
	UniformWrapper<glm::vec3> gLightDirection;
	UniformWrapper<GLfloat>   gDirLightIntensity;

	LightLocations LightLocation;
	MaterialLocations MaterialLocation;


	GLuint
		ProjectionMatrixUniformLocation,
		ViewMatrixUniformLocation,
		ModelMatrixUniformLocation,
		NormalMatrixLocation;

	GLuint 
		BufferIds[9];


	GLfloat var;
	GLfloat aspect;
};

//-----------------------------------------------------------------------------
// ArcBall
//-----------------------------------------------------------------------------
void ArcBall::Begin(int x, int y) {
	mDragging = true;
	mVecDown = ScreenToVector(static_cast<float>(x), static_cast<float>(y));
	mQuatDown = mQuatNow;
}
//-----------------------------------------------------------------------------
void ArcBall::End() {
	mDragging = false;
}
//-----------------------------------------------------------------------------
void ArcBall::Drag(int x, int y) {
	if (mDragging) {
		mVecNow = ScreenToVector(static_cast<float>(x), static_cast<float>(y));

		glm::vec3 p = glm::cross(mVecDown, mVecNow);

		if (glm::length(p) > 1e-5) {
			glm::quat q = glm::quat(glm::dot(mVecDown, mVecNow), p);
			mQuatNow = glm::cross(glm::normalize(q), mQuatDown);
		}
		else {
			mQuatNow = glm::cross(glm::quat(), mQuatDown);
		}
	}
}
//-----------------------------------------------------------------------------
void ArcBall::SetWindowSize(int width, int height) {
	mWidth = width;
	mHeight = height;
}
//-----------------------------------------------------------------------------
glm::vec3 ArcBall::ScreenToVector(float screenX, float screenY) {
	glm::vec2 v;
	v.x = ((screenX / ((mWidth - 1) / 2)) - 1);
	v.y = -((screenY / ((mHeight - 1) / 2)) - 1);

	float len = glm::length(v);
	if (len > 1.0f)
		return glm::vec3(v / sqrt(len), 0);

	return glm::vec3(v, sqrt(1.0f - len));
}
//-----------------------------------------------------------------------------