#include "UpdateUIScoreComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

#include <fstream>
#include <algorithm>
#include <string>

int xc::UpdateUIScoreComponent::loadHighScore() const
{
	std::ifstream f(m_SavePath);
	int v = 0;
	if (f >> v) return v;
	return 0;
}

void xc::UpdateUIScoreComponent::saveHighScore(int v) const
{
	std::ofstream f(m_SavePath, std::ios::trunc);
	if (f) { f << v; f.flush(); }
}

void xc::UpdateUIScoreComponent::update(float /*dt*/)
{
	auto* owner = getOwner();
	if (!owner) return;

	auto* scoreText = owner->getComponent<Papyrus::TextComponent>();
	if (!scoreText) return;

	// Load once
	if (!m_Loaded)
	{
		m_HighScore = loadHighScore();
		m_Loaded = true;

		// write HI immediately
		if (m_pHighScoreTextGO)
		{
			if (auto* hiText = m_pHighScoreTextGO->getComponent<Papyrus::TextComponent>())
				hiText->setText(std::to_string(m_HighScore));
		}
	}

	// Update score (replace with your real scoring later)
	m_ScoreCounter += 1;

	// Update score UI
	scoreText->setText(std::to_string(m_ScoreCounter));

	// High score update + save
	if (m_ScoreCounter > m_HighScore)
	{
		m_HighScore = m_ScoreCounter;
		saveHighScore(m_HighScore);

		if (m_pHighScoreTextGO)
		{
			if (auto* hiText = m_pHighScoreTextGO->getComponent<Papyrus::TextComponent>())
				hiText->setText(std::to_string(m_HighScore));
		}
	}
}
