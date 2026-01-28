#ifndef UPDATE_UI_SCORE_COMPONENT
#define UPDATE_UI_SCORE_COMPONENT

#include "BaseComponent.h"
#include <string>

namespace Papyrus { class GameObject; }

namespace xc
{
	class UpdateUIScoreComponent final : public Papyrus::BaseComponent
	{
	public:
		// Call this once in load() so we can update the HI text
		void setHighScoreTextObject(Papyrus::GameObject* go) { m_pHighScoreTextGO = go; }

		// Optional: change where it saves (default "highscore.txt")
		void setSaveFile(const std::string& path) { m_SavePath = path; }

		void update(float dt) override;

	private:
		int m_ScoreCounter = 0;

		// persistent high score
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
