#include "TextureComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "AnimationComponent.h" 
#include "PlayerAnimationComponent.h" 

namespace Papyrus
{
	TextureComponent::TextureComponent(const std::string& fileName)
		:m_FileName(fileName) 
	{
		m_pTexture = Papyrus::ResourceManager::getInstance().loadTexture(m_FileName);
	}

	void TextureComponent::render() const
	{
		if (getOwner()->hasComponent<Papyrus::AnimationComponent>())
			return;
		if (getOwner()->hasComponent<PlayerAnimationComponent>())  
			return; 
		auto pos = getOwner()->m_Transform.position; 
		Papyrus::Renderer::getInstance().renderTexture(*m_pTexture, pos.x, pos.y);
	}

	b2Vec2 TextureComponent::getSize() const
	{
		return m_pTexture->getSize();
	}

	void TextureComponent::setTexture(const std::string& fileName)
	{
		m_FileName = fileName;
		m_pTexture = Papyrus::ResourceManager::getInstance().loadTexture(m_FileName); 
	}
}