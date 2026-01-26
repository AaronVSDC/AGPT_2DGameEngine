#ifndef WINDOW_H
#define WINDOW_H
#include <SDL3/SDL.h>
#include <string>
#include <memory> 
namespace Papyrus
{
    class Window final
    {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        SDL_Window* getHandle() const { return m_Window; }
        int getWidth()  const { return m_Width; }
        int getHeight() const { return m_Height; }

        // TODO: call this from your SDL event loop when a resize happens:
        void setSize(int w, int h) { m_Width = w; m_Height = h; } 

    private:
        SDL_Window* m_Window = nullptr;
        SDL_GLContext m_GLContext = nullptr;
        int m_Width = 0;
        int m_Height = 0;
    };

    extern std::unique_ptr<Window> GWindow; 

}


#endif


