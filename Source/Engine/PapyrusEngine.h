#include <SDL3/SDL.h>
#include <functional>

namespace Papyrus
{

	class PapyrusEngine final
	{
	public: 
		PapyrusEngine(); 
		~PapyrusEngine(); 
		PapyrusEngine(const PapyrusEngine&) = delete; 
		PapyrusEngine(const PapyrusEngine&&) = delete; 
		PapyrusEngine& operator=(const PapyrusEngine&) = delete; 
		PapyrusEngine& operator=(const PapyrusEngine&&) = delete;


		void run(const std::function<void()>& load);

	private: 
		void load(); 


	};


}