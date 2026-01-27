#if defined(_WIN32)
	#if defined(PAPYRUS_ENGINE_BUILD_DLL)
		#define PAPYRUS_API __declspec(dllexport)
	#else
		#define PAPYRUS_API __declspec(dllimport)
	#endif
#else
#define PAPYRUS_API 
#endif