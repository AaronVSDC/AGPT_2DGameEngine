
#include "HighScoreComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

#include <fstream>
#include <algorithm>

int xc::HighScoreComponent::loadHighScore() const
{
	std::ifstream f(m_SavePath);
	int v = 0;
	if (f >> v) return v;
	return 0;
}

void xc::HighScoreComponent::saveHighScore(int v) const
{
	std::ofstream f(m_SavePath, std::ios::trunc);
	if (f) f << v;
}

void xc::HighScoreComponent::update(float /*dt*/)
{
	auto* owner = getOwner();
	if (!owner) return;

	auto* scoreText = owner->getComponent<Papyrus::TextComponent>();
	if (!scoreText) return;

	// Load persisted high score once
	if (!m_Loaded)
	{
		m_HighScore = loadHighScore();
		m_Loaded = true;

		// Update high score UI immediately
		if (m_pHighScoreTextGO)
		{
			if (auto* hiText = m_pHighScoreTextGO->getComponent<Papyrus::TextComponent>())
				hiText->setText("HI " + std::to_string(m_HighScore));
		}
	}

	// Update score (you can replace this with "score += points" from gameplay)
	m_ScoreCounter += 1;

	// Write score UI
	scoreText->setText("SCORE " + std::to_string(m_ScoreCounter));

	// Update high score + persist only when it improves
	const int oldHi = m_HighScore;
	m_HighScore = std::max(m_HighScore, m_ScoreCounter);

	if (m_HighScore != oldHi)
	{
		saveHighScore(m_HighScore);

		if (m_pHighScoreTextGO)
		{
			if (auto* hiText = m_pHighScoreTextGO->getComponent<Papyrus::TextComponent>())
				hiText->setText("HI " + std::to_string(m_HighScore));
		}
	}
}
