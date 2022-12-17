#include "Core/test.h"
#include "Core/Engine.h"
int main(void)
{
	JJEngine::instance()->update();
	hello();
	return 0;
}