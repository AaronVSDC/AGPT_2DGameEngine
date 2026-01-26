#include "main.h"
#include "PapyrusEngine.h"

int main()
{
	Papyrus::PapyrusEngine engine{};

	try
	{
		engine.run();
	}
	catch (std::exception& e)
	{
		std::cout << "RUNTIME ERROR: " << e.what() << std::endl; 
	}
	return 0;
}
