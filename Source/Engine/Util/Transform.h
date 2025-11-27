#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <box2d/math_functions.h>

namespace Papyrus 
{
	struct Transform
	{
		b2Vec2 position{ 0.0f, 0.0f };  
		float  rotation{ 0.0f };        
		b2Vec2 scale{ 1.0f, 1.0f };    
	};
}
#endif