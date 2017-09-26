#ifndef OPENCL_H
#define OPENCL_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

class OpenCL
{
    public:
        OpenCL(SDL_GLContext* glContext, SDL_Window *window);



        virtual ~OpenCL();

    protected:

    private:
        OpenCL(const OpenCL& other) {}
        void operator=(const OpenCL& other) {}
};

#endif // OPENCL_H
