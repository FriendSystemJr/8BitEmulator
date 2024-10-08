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
	chip.loadGame("test_opcode.ch8");

	while (!Renderer::windowShouldClose()) {
		chip.emulateCycle();
		if (chip.drawFlag) {
			grid.Clear();
			for (int x = 0; x < 64; ++x) {
				for (int y = 0; y < 32; ++y) {
					if (chip.GetGFX()[x][y] == (int)1) {
						grid.SetPixel(x, 31 - y);
					}
				}
			}
			chip.drawFlag = false;
		}

		Renderer::RenderGrid(grid, shader);
	}
}