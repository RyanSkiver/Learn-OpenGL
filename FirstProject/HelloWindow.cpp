#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream> //text
// ON SHADERS/GLSL
//SOURCE CODES
const char* vertexShaderSource = "#version 330 core\n" //source code for the vertex shader
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderSource = "#version 330 core\n" //frag shader source code
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

//function to check if pressed escape to close
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //if escape has been pressed while in the window
		glfwSetWindowShouldClose(window, true); //close the window
}

int main() {
	glfwInit(); //initialize GLFW : config glfw -> glfwWindowHint(what option, value of option)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //set max vers
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //set min vers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //set profile to core only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//create and initialize window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); //create window (width, height, name)
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //make window context

	//loads up GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1; //stops if GLAD doesnt initialize
	}
	
	//SHADERS SETUP ---------------------------------------
	//vertex shader setup
	unsigned int vertexShader; //create var for Vertex shader object
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //create the actual shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //attach shader source code to the shader object (to compile to, # strings as source code, source code, NULL)
	glCompileShader(vertexShader); //compile the shader
	
	//fragment shader setup (color)
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	//link shaders - shader program
	unsigned int shaderProgram; //create the program object
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader); //attach the shaders to the program
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); //link them
	glDeleteShader(vertexShader); //delete shader objects after linking, we dont need anymore
	glDeleteShader(fragmentShader);
	glUseProgram(shaderProgram); //use the newly created program in every shader and rendering call from now on


	//tell OpenGl size of rendering window
	glViewport(0, 0, 800, 600); //(first 2: set location of lower left corner,, width, height in pixels)


	//create triangle ------------------------------------------
	float vertices[] = { //triangle vertices (rectangle)
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};
	unsigned int indices[] = { //indices for the rectangle (2 triangles)
		0, 1, 3,
		1, 2, 3
	};
	unsigned int VAO; //initialize Vertex Array Object
	glGenVertexArrays(1, &VAO); //generate array with unique id 1
	unsigned int VBO; //initialize Vertex Buffer Object
	glGenBuffers(1, &VBO); //generate buffer with unique id 1
	unsigned int EBO;
	glGenBuffers(1, &EBO); //generate buffer with unique id 1
	// ..:: Initialization code :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO); //bind the vertex array object
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer to gl_array_buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //make call to GL_ARRAY_BUFFER, copies vertex data into buffer memory
			//(type of buffer to copy data into, size of the data in bytes to pass, actual data to send, how to manage the data)
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the element buffer object to the GL_ELEMENT_ARRAY_BUFFER target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //copy the index data into the buffer memory
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //tell OpenGL how to interpret the vertex data (location, size, type, normalized?, stride, offset)
	glEnableVertexAttribArray(0); //enable the vertex attribute at location 0 (the one we set in the vertex shader)

	//render loop ------------------------------------------
	while (!glfwWindowShouldClose(window)){ //check if there is an instruction to close
		//input
		processInput(window); //check key press every frame (esc == close)
		//render commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set background color for clearing the screen (r, g, b, alpha)
		glClear(GL_COLOR_BUFFER_BIT); //wipes color buffer, fills with what was set
		//draw triangle
		glUseProgram(shaderProgram); // use our shader program when we want to render an object
		glBindVertexArray(VAO); // bind the VAO (the triangle we want to draw)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw the triangle (mode, how many vertices, type of indices, offset)
		glBindVertexArray(0); //unbind the VAO
		//check and call events, swap the buffer
		glfwSwapBuffers(window); //swap color buffer, show as output to screen
		glfwPollEvents(); //check if any events are triggered like mouse, keyboard
	}
	//exit loop
	glfwTerminate(); //clear GLFW's resources
	return 0;
}