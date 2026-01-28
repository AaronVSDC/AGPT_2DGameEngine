#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <string>
#include <vector>

#include "GameObject.h"
#include "PapyrusAPI.h"

namespace Papyrus
{
	class SceneManager;
	class PAPYRUS_API Scene final
	{
		friend class SceneManager;
	public:
		void add(std::unique_ptr<GameObject> object);
		void removeAll();

		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const;
		void start();
		void onEnable();
		void onDisable();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		bool getIsActive() const { return m_IsActive; }
		void setIsActive(bool newValue) { m_IsActive = newValue; }
		GameObject* findGameObjectByTag(std::string_view tag) noexcept; 
		std::vector<GameObject*> findGameObjectsByTag(std::string_view tag) noexcept;
		void setIsPaused(bool newVal) { m_IsPaused = newVal;  }

	private:  
		explicit Scene(const std::string& name);

		void flushPendingAdds();
		 
		void checkRemovalGameObject(); 

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects{};
		std::vector<std::unique_ptr<GameObject>> m_PendingAdds{};

		bool m_HasStarted = false;
		static unsigned int m_idCounter;

		bool m_IsActive = true;

		bool m_IsPaused = false; 
	};

}
#endif SCENE_H