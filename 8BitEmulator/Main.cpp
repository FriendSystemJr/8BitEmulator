#include <iostream>

#include "RenderAPI.h"
#include "chip8.h"

int main() {
	//Graphic init
	Renderer::Init(800, 400, "Chip-8");
	Renderer::Shader shader("shader/vertex.txt", "shader/fragment.txt");
	Renderer::Grid grid = Renderer::CreateGrid(64, 32);
	Renderer::BufferManager bufferManager(grid);
	bufferManager.SetGridStandard();

	Chip8 chip{ grid };
	chip.initialize();
}