
#ifndef HIGH_SCORE_COMPONENT
#define HIGH_SCORE_COMPONENT

#include "BaseComponent.h"
#include <string>

namespace Papyrus { class GameObject; }

namespace xc
{
	class HighScoreComponent final : public Papyrus::BaseComponent
	{
	public:
		void setHighScoreTextObject(Papyrus::GameObject* pGO) { m_pHighScoreTextGO = pGO; }

		void setSaveFile(const std::string& path) { m_SavePath = path; }

		void update(float dt) override;

	private:
		int m_ScoreCounter = 0;
		int m_HighScore = 0;

		bool m_Loaded = false;
		std::string m_SavePath = "highscore.txt";

		Papyrus::GameObject* m_pHighScoreTextGO = nullptr;

	private:
		int loadHighScore() const;
		void saveHighScore(int v) const;
	};
}

#endif
