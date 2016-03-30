//
//  viewer.cpp
//  Thanda

#include "viewer.hpp"

Viewer::Viewer()
{
    scene.read_json();
}

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 1024, HEIGHT = 720;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "gl_Position = MVP*vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1, 1, 1, 1);\n"
    "}\n\0";

const GLchar* p_vertexShaderSource = "#version 330 core\n"
    "in vec3 position;\n"
    "in vec4 in_color; \n"
    "out vec4 col;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "gl_Position = MVP*vec4(position.x, position.y, position.z, 1.0);\n"
                                     "col = in_color; \n"
    "}\0";
const GLchar* p_fragmentShaderSource = "#version 330 core\n"
    "in vec4 col; \n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = col;\n"
    "}\n\0";

// The MAIN function, from here we start the application and run the game loop
int Viewer::create_window()
{

    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW

    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions


    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* PARTICLE SHADER
     * PARTICLE SHADER*/

    // Build and compile our shader program
    // Vertex shader
    GLuint p_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(p_vertexShader, 1, &p_vertexShaderSource, NULL);
    glCompileShader(p_vertexShader);
    // Check for compile time errors
    GLint p_success;
    GLchar p_infoLog[512];
    glGetShaderiv(p_vertexShader, GL_COMPILE_STATUS, &success);
    if (!p_success)
    {
        glGetShaderInfoLog(p_vertexShader, 512, NULL, p_infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << p_infoLog << std::endl;
    }
    // Fragment shader
    GLuint p_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(p_fragmentShader, 1, &p_fragmentShaderSource, NULL);
    glCompileShader(p_fragmentShader);
    // Check for compile time errors
    glGetShaderiv(p_fragmentShader, GL_COMPILE_STATUS, &p_success);
    if (!p_success)
    {
        glGetShaderInfoLog(p_fragmentShader, 512, NULL, p_infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << p_infoLog << std::endl;
    }
    // Link shaders
    GLuint p_shaderProgram = glCreateProgram();
    glAttachShader(p_shaderProgram, p_vertexShader);
    glAttachShader(p_shaderProgram, p_fragmentShader);
    glLinkProgram(p_shaderProgram);
    // Check for linking errors
    glGetProgramiv(p_shaderProgram, GL_LINK_STATUS, &p_success);
    if (!p_success) {
        glGetProgramInfoLog(p_shaderProgram, 512, NULL, p_infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << p_infoLog << std::endl;
    }
    glDeleteShader(p_vertexShader);
    glDeleteShader(p_fragmentShader);


    camera = Camera(WIDTH,HEIGHT);

    // Set up vertex data (and buffer(s)) and attribute pointers
    //GLfloat vertices[] = {
    //  // First triangle
    //   0.5f,  0.5f,  // Top Right
    //   0.5f, -0.5f,  // Bottom Right
    //  -0.5f,  0.5f,  // Top Left
    //  // Second triangle
    //   0.5f, -0.5f,  // Bottom Right
    //  -0.5f, -0.5f,  // Bottom Left
    //  -0.5f,  0.5f   // Top Left
    //};
//    GLfloat vertices[] = {
//         0.5f,  0.5f, 0.0f,  // Top Right
//         0.5f, -0.5f, 0.0f,  // Bottom Right
//        -0.5f, -0.5f, 0.0f,  // Bottom Left
//        -0.5f,  0.5f, 0.0f   // Top Left
//    };
//    GLuint indices[] = {  // Note that we start from 0!
//        0, 1, 3,  // First Triangle
//        1, 2, 3   // Second Triangle
//    };
    scene.bounds->create();
    vertices = scene.bounds->vert_pos;
    indices = scene.bounds->idx;
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

    int position_index = glGetAttribLocation(shaderProgram,"position");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position_index);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, position_index); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(position_index);// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    /*PARTICLE ARRAY*/
    particles = scene.fluid_solver->particles;
    particle_colors = scene.fluid_solver->colors;
    for(int i = 0; i <particles->size(); i++) {
        p_idx.push_back(i);
    }

    GLuint p_VBO, p_VAO, p_EBO, p_COL;
    glGenVertexArrays(1, &p_VAO);
    glGenBuffers(1, &p_VBO);
    glGenBuffers(1, &p_EBO);
    glGenBuffers(1, &p_COL);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(p_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, p_COL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4), particle_colors->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * particles->size(), particles->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * p_idx.size(), p_idx.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    int col_index = glGetAttribLocation(p_shaderProgram,"in_color");
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)col_index);
    glEnableVertexAttribArray(col_index);


    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    double xpos_0, ypos_0;
    double xpos, ypos;
    float zoom_0;
    float theta_0;
    float phi_0;
    int count = 0;
    std::clock_t start;
    start = std::clock();
    while (!glfwWindowShouldClose(window))
    {
        double duration = (std::clock() - start)/(double) CLOCKS_PER_SEC;
        float time_step = 0.1;
        if(duration > time_step) {
            start = std::clock();
            scene.fluid_solver->update();
        }
        int width = WIDTH;
        int height = HEIGHT;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        if( glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS )
            camera.theta -= 0.7;
        if( glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS )
            camera.theta += 0.7;
        if( glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS )
            camera.phi -= 0.7;
        if( glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS )
            camera.phi += 0.7;
        if( glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS )
            camera.zoom += 0.1;
        if( glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS )
            camera.zoom -= 0.1;
        if( glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS ) {
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ) {
               glfwGetCursorPos(window, &xpos, &ypos);
               camera.theta = theta_0 - (xpos-xpos_0)*0.1;
               camera.phi = phi_0 - (ypos-ypos_0)*0.1;
            }
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS ) {
               glfwGetCursorPos(window, &xpos, &ypos);
               vec3 x_off = float((xpos-xpos_0)*0.01)*right_0;
               vec3 y_off = float((ypos-ypos_0)*0.01)*up_0;
               vec3 temp_target = target_0 - vec3(translate(mat4(1.0f),x_off)*vec4(0,0,0,1.0f));
               camera.camera_target = temp_target + vec3(translate(mat4(1.0f),y_off)*vec4(0,0,0,1.0f));
            }
            else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ) {
               glfwGetCursorPos(window, &xpos, &ypos);
               camera.zoom = zoom_0 - (xpos-xpos_0)*0.01;
            } else{
               glfwGetCursorPos(window, &xpos_0, &ypos_0);
               theta_0 = camera.theta;
               phi_0 = camera.phi;
               zoom_0 = camera.zoom;
               target_0 = camera.camera_target;
               right_0 = camera.camera_right;
               up_0 = camera.camera_up;
            }
        }

        glGenVertexArrays(1, &p_VAO);
        glGenBuffers(1, &p_VBO);
        glGenBuffers(1, &p_EBO);
        glGenBuffers(1, &p_COL);
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(p_VAO);


        glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*particles->size(), particles->data(), GL_STATIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(0);


        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind


        glBindBuffer(GL_ARRAY_BUFFER, p_COL);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*particle_colors->size(), particle_colors->data(), GL_STATIC_DRAW);

        int col_index = glGetAttribLocation(p_shaderProgram,"in_color");
        glVertexAttribPointer(col_index, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(col_index);

        glBindBuffer(GL_ARRAY_BUFFER, col_index); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*p_idx.size(), p_idx.data(), GL_STATIC_DRAW);

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
        glBindVertexArray(col_index); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO



        //TESTEST

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Render
        // Clear the colorbuffer

        // Draw our first triangle
        glUseProgram(shaderProgram);

        camera.update_camera();
        mvp = camera.viewproj_matrix()*mat4(1.0f);
        MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glUseProgram(p_shaderProgram);

        MatrixID = glGetUniformLocation(p_shaderProgram, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(p_VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glDrawElements(GL_POINTS, particles->size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteVertexArrays(1, &p_VAO);
    glDeleteBuffers(1, &p_VBO);
    glDeleteBuffers(1, &p_EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}



