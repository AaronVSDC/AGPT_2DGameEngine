#include <SDL3/SDL.h>

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


		void run(); 

	private: 
		void load(); 


	};


}