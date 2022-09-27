#include "Util/Shader.h"

Util::Shader::Shader(const char* vertShaderPath, const char* fragShaderPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFileStream;
    std::ifstream fShaderFileStream;
    // ensure ifstream objects can throw exceptions:
    vShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFileStream.open(vertShaderPath);
        fShaderFileStream.open(fragShaderPath);
        std::stringstream vShaderStringStream, fShaderStringStream;
        // read file's buffer contents into streams
        vShaderStringStream << vShaderFileStream.rdbuf();
        fShaderStringStream << fShaderFileStream.rdbuf();
        // close file handlers
        vShaderFileStream.close();
        fShaderFileStream.close();
        // convert stream into string
        vertexCodeStr = vShaderStringStream.str();
        fragmentCodeStr = fShaderStringStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            }
    const char* vShaderCode = vertexCodeStr.c_str();
    const char* fShaderCode = fragmentCodeStr.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Util::Shader::checkCompileErrors(unsigned int shader, std::string shaderType)
{
    int success;
    char infoLog[1024];
    if (shaderType != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n" 
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << shaderType << "\n" 
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Util::Shader::use()
{
    glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Util::Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Util::Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Util::Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Util::Shader::printSourceCode()
{
    if (!vertexCodeStr.empty())
    {
        std::cout << std::string(vertexCodeStr) << std::endl;
    }
    if (!fragmentCodeStr.empty())
    {
        std::cout << std::string(fragmentCodeStr) << std::endl;
    }
}
