#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>


class Shader
{
    public:
        Shader(const std::string& fileName);

        void Bind();

        void UpdateTime(const float& seconds);
        void UpdateResolution(const float& width, const float& height);

        virtual ~Shader();

    protected:
    private:

    Shader(const Shader& other);
    void operator=(const Shader& other);
    static const unsigned int NUM_SHADERS = 2;

    enum
    {
        ITIME_U,
        IRES_U,

        NUM_UNIFORMS
    };

    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];


};

#endif // SHADER_H
