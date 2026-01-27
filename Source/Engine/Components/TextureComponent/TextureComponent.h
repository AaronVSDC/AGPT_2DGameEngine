#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H
#include <string>
#include <box2d\math_functions.h>
#include "BaseComponent.h"
#include "PapyrusAPI.h"

namespace Papyrus
{ 
	class Texture2D;
	class PAPYRUS_API TextureComponent final : public Papyrus::BaseComponent
	{
	public:
		TextureComponent(const std::string& fileName);
		TextureComponent(const TextureComponent&) = delete;
		TextureComponent& operator=(const TextureComponent&) = delete;
		TextureComponent(TextureComponent&&) = delete;
		TextureComponent& operator=(TextureComponent&&) = delete;

		void render() const override;

		b2Vec2 getSize() const;
		Papyrus::Texture2D* getTexture() const { return m_pTexture; }
		void setTexture(const std::string& fileName);

	private:
		 
		Papyrus::Texture2D* m_pTexture;
		std::string m_FileName;

	};
}

#endif