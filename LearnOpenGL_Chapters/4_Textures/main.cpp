#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Util/Shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* hWindow);


int main()
{
	// 1- Init window context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Faruk's Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 2- Init GLAD for enabling GL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 640, 480);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//float vertices[] = {
	// 0.5f,  0.5f, 0.0f,  // top right
	// 0.5f, -0.5f, 0.0f,  // bottom right
	//-0.5f, -0.5f, 0.0f,  // bottom left
	//-0.5f,  0.5f, 0.0f   // top left 
	//};

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;



	float vertices[] = {
		// positions       |   colors		|    textCoord
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f  // top 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	Util::Shader shader("../../shaders/texturesExample.vert", "../../shaders/texturesExample.frag");
	shader.printSourceCode();

	// use either VBO or EBO
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	unsigned int hTexture;
	glGenTextures(1, &hTexture);
	glBindTexture(GL_TEXTURE_2D, hTexture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load("../../textures/tiles2.jpg", &width, &height, &nrChannels, 0);
	if (imgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imgData);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hTexture);
	shader.use();
	shader.setInt("ourTexture", 0);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hTexture);

		
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* hWindow)
{

	if (glfwGetKey(hWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(hWindow, true);

}