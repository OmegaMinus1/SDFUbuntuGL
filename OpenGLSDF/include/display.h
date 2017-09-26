#ifndef DISPLAY_H
#define DISPLAY_H
#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <string>

class Display
{
    public:
        Display(int width, int height, const std::string& title);

        void ClearColor(float red, float green, float blue, float alpha);
        void Clear();
        void Update();
        bool IsRunning();

        unsigned int Width;
        unsigned int Height;

        void SetFullScreen();

        inline SDL_Window* GetWindow() { return m_window; }
        inline SDL_GLContext* GetContextGL() { return &m_glContext; }

        bool m_useOpenClRendering;

        virtual ~Display();
    private:

        Display(const Display& other){}
        void operator=(const Display& other){}

        SDL_Window* m_window;
        SDL_GLContext m_glContext;

        bool m_isRunning;



};

#endif // DISPLAY_H
