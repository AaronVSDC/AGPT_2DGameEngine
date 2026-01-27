#ifndef WINDOW_H
#define WINDOW_H
#include <SDL3/SDL.h>
#include <string>
#include <memory>  
#include "Singleton.h"
#include "PapyrusAPI.h"

namespace Papyrus
{
    class PAPYRUS_API Window final : public Singleton<Window>
    {
    public:
        Window();
        ~Window();

        static void create(const std::string& title, int width, int height); 
        static SDL_Window* getHandle() { return m_Window; }
        static int getWidth(){ return m_Width; }
        static int getHeight() { return m_Height; }

        // TODO: call this from your SDL event loop when a resize happens:
        static void setSize(int w, int h) { m_Width = w; m_Height = h; } 

    private:
        static SDL_Window* m_Window;
        static SDL_GLContext m_GLContext;
        static int m_Width;
        static int m_Height;
    };

}


#endif


