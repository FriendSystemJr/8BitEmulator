#include "Shader.h"
#include "Grid.h"
#include "BufferManager.h"
#include "InputHandler.h"

namespace Renderer {
	
	extern "C" {
		__declspec(dllimport) void Init(unsigned int width, unsigned int height, const char* windowName);
		__declspec(dllimport) bool windowShouldClose();
		__declspec(dllimport) void RenderGrid(Grid& grid, Shader& shader);
		__declspec(dllimport) void Cleanup(unsigned int& VBO, unsigned int& VAO);
		__declspec(dllimport) void* GetRenderWindow();
	}
}