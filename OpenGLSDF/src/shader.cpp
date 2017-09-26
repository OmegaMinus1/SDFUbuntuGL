#include "shader.h"

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);


Shader::Shader(const std::string& fileName)
{
    m_program = glCreateProgram();

    m_shaders[0] = CreateShader(LoadShader(fileName + ".vert"), GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(LoadShader(fileName + ".frag"), GL_FRAGMENT_SHADER);

    for(size_t i = 0; i < NUM_SHADERS; i++)
    {
        glAttachShader(m_program, m_shaders[i]);
    }

    glBindAttribLocation(m_program, 0, "position");
    glBindAttribLocation(m_program, 1, "texCoord");

    glLinkProgram(m_program);

    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program Link Error: ");

    glValidateProgram(m_program);

    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program Validation Error: ");

    m_uniforms[ITIME_U] = glGetUniformLocation(m_program, "iTime");
    m_uniforms[IRES_U] = glGetUniformLocation(m_program, "iResolution");

}

Shader::~Shader()
{
    for(size_t i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

    glDeleteProgram(m_program);
}

void Shader::Bind()
{
    glUseProgram(m_program);
}

static std::string LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
    {
        std::cerr << "Shader Error: Open File Failed: " << fileName << std::endl;
    }

    return output;
}

static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if(shader == 0)
    {
        std::cerr << "Error: Shader Creation Failed!" << std::endl;
    }

    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringsLengths[1];

    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringsLengths[0] = text.length();

    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringsLengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader Compile Error: ");

    return shader;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
    {
        glGetProgramiv(shader, flag, &success);
    }
    else
    {
        glGetShaderiv(shader, flag, &success);
    }

    if(success == GL_FALSE)
    {
        if(isProgram)
        {
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        }
        else
        {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }

        std::cerr << errorMessage << ": " << error << std::endl;
    }
}

void Shader::UpdateTime(const float& seconds)
{
    glUniform1f(m_uniforms[ITIME_U], seconds);
}

void Shader::UpdateResolution(const float& width, const float& height)
{
    GLfloat res[2] = { width, height };
    glUniform2fv(m_uniforms[IRES_U], 1, res);
}


