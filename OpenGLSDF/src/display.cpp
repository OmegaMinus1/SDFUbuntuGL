#include "display.h"

Display::Display(int width, int height, const std::string& title)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    Width = width;
    Height = height;

    m_window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, SDL_WINDOW_OPENGL);

    m_glContext = SDL_GL_CreateContext(m_window);

    GLenum status = glewInit();

    if(status != GLEW_OK)
    {
       std::cerr << "GLEW : Failure with startup." << std::endl;
       std::cerr << GLEW_OK << std::endl;
       std::cerr << status << std::endl;
    }

    m_isRunning = true;
}

Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);

    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

bool Display::IsRunning()
{
    return m_isRunning;
}

void Display::Update()
{
    SDL_GL_SwapWindow(m_window);

    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            m_isRunning = false;
        }
        else if(event.type == SDL_KEYUP)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
            }
        }
    }
}

void Display::ClearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
}

void Display::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Display::SetFullScreen()
{
    //Get desktop resolution
    SDL_DisplayMode current;

    for(size_t i = 0; i < (size_t)SDL_GetNumVideoDisplays(); ++i)
    {
        int result = SDL_GetCurrentDisplayMode(i, &current);

        if(result != 0)
        {
            //Error
        }
        else
        {
            Width = current.w;
            Height = current.h;

            std::cerr << "Desktop Resolution: " << Width << "x" << Height << " @ " << current.refresh_rate << "hz" << std::endl;
        }
    }

    //Set the window size to Desktop Resolution
    SDL_SetWindowSize(m_window, Width, Height);

    //Tell OpenGL that we changed resolution
    glViewport(0,0,Width, Height);

    //Set to fullscreen mode
    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}



