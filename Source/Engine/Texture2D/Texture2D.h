#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <string>
#include <box2d\math_functions.h>

struct SDL_Texture;
namespace Papyrus
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* getSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		explicit Texture2D(const std::string& fullPath);
		~Texture2D();

		b2Vec2 getSize() const;

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;
	private:
		SDL_Texture* m_texture;
	};
}
#endif