#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Display.h"
#include "Shader.h"
#include "Texture.h"

int main(int argc, char** argv)
{
	const int FRAMES = 60;
	const int W_WIDTH = 800;
	const int W_HEIGHT = 600;

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	Uint32 frameRate(1000 / FRAMES);
	Uint32 debutBoucle(0), finBoucle(0), tempsEcoule(0);

	Display display(W_WIDTH, W_HEIGHT, "My display");

	SDL_Event test_event;

	Shader shader("shaders/basicShader");

	Texture texture("../assets/textures/tileset.jpg");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
	   0.0f, 0.0f, 0.0f,
	   400.0f, 400.0f, 0.0f,
	   0.0f, 400.0f, 0.0f,

	   400.0f, 0.0f, 0.0f,
	   0.0f, 0.0f, 0.0f,
	   400.0f, 400.0f, 0.0f
	};

	float colours_1[] = { 1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, 1.0 };

	float colours_2[] = { 1.0, 1.0, 1.0,   0.0, 0.0, 0.0,   0.4, 0.4, 0.4 };

	float texcoord_1[] = { 0.0, 0.0,   0.0, 1.0,  1.0, 0.0 };

	while (!display.IsClosed())
	{

		debutBoucle = SDL_GetTicks();


		while (SDL_PollEvent(&test_event))
		{
			switch (test_event.type)
			{

			case SDL_KEYDOWN:

				switch (test_event.key.keysym.sym)
				{

				case SDLK_s:
					std::cout << x << std::endl;
					x += 0.1;
					break;

				case SDLK_z:
					std::cout << y << std::endl;
					y += 0.1;
					break;

				case SDLK_q:
					std::cout << z << std::endl;
					z += 0.1;
					break;

				case SDLK_d:
					std::cout << "SDL KEY : D" << std::endl;
					break;

				case SDLK_ESCAPE:
					SDL_Quit();
					display.Close();
					break;

				}

			}
		}

		display.Clear(0.0f, 0.15f, 0.3f, 1.0f);

		shader.Bind();

		// ortho camera projection :
		glm::mat4 Projection = glm::ortho(0.0f, (float)W_WIDTH, (float)W_HEIGHT, 0.0f, -1.0f, 1.1f); // In world coordinates

		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0, 0, 1), // Camera is at (0,0,1), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::rotate(Model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MatrixID = glGetUniformLocation(shader.getProgramID(), "mvp");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		// This will identify our vertex buffer
		GLuint vertexbuffer;
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glUseProgram(shader.getProgramID());

		display.Update();

		finBoucle = SDL_GetTicks();
		tempsEcoule = finBoucle - debutBoucle;

		if (tempsEcoule < frameRate)
			SDL_Delay(frameRate - tempsEcoule);
	}

	return 0;
}