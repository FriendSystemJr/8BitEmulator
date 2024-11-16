#include <iostream>

#include <chrono>
#include <thread>

#include "RenderAPI.h"
#include "chip8.h"

void HandleInput(GLFWwindow* window, Chip8& chip);

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

	//Cycles per second
	const double cyclesPerSecond = 500.0;
	const double timePerCycle = 1.0 / cyclesPerSecond;

	//Timing
	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	double deltaTime = 0.0;

	while (!Renderer::windowShouldClose()) {
		//Current time
		auto currentCycleTime = std::chrono::high_resolution_clock::now();
		//Elapsed time
		std::chrono::duration<double> elapsed = currentCycleTime - lastCycleTime;

		deltaTime += elapsed.count();

		lastCycleTime = currentCycleTime;

		if (deltaTime >= timePerCycle) {
			chip.emulateCycle();
			deltaTime -= timePerCycle;

			HandleInput((GLFWwindow*)Renderer::GetRenderWindow(), chip);

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
				Renderer::RenderGrid(grid, shader);
				Renderer::BufferSwap(Renderer::GetRenderWindow());
				Renderer::PollEvents();
				
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		
	}
}

void KeyDown(GLFWwindow* window, Chip8& chip, Renderer::InputHandler::KeyCode key, int reg) {
	if (Renderer::InputHandler::GetKey(window, key) == Renderer::InputHandler::KEY_PRESSED) {
		chip.m_key[reg] = 1;
		//std::cout << "Pressed Key: " << key << "\n";
	}
}

void KeyUP(GLFWwindow* window, Chip8& chip, Renderer::InputHandler::KeyCode key, int reg) {
	if (Renderer::InputHandler::GetKey(window, key) == Renderer::InputHandler::KEY_RELEASED) {
		chip.m_key[reg] = 0;
	}
}

void HandleInput(GLFWwindow* window, Chip8& chip) {
	KeyDown(window, chip, Renderer::InputHandler::KEY_1, 0x1);
	KeyDown(window, chip, Renderer::InputHandler::KEY_2, 0x2);
	KeyDown(window, chip, Renderer::InputHandler::KEY_3, 0x3);
	KeyDown(window, chip, Renderer::InputHandler::KEY_4, 0xC);

	KeyDown(window, chip, Renderer::InputHandler::KEY_Q, 0x4);
	KeyDown(window, chip, Renderer::InputHandler::KEY_W, 0x5);
	KeyDown(window, chip, Renderer::InputHandler::KEY_E, 0x6);
	KeyDown(window, chip, Renderer::InputHandler::KEY_R, 0xD);

	KeyDown(window, chip, Renderer::InputHandler::KEY_A, 0x7);
	KeyDown(window, chip, Renderer::InputHandler::KEY_S, 0x8);
	KeyDown(window, chip, Renderer::InputHandler::KEY_D, 0x9);
	KeyDown(window, chip, Renderer::InputHandler::KEY_F, 0xE);

	KeyDown(window, chip, Renderer::InputHandler::KEY_Y, 0xA);
	KeyDown(window, chip, Renderer::InputHandler::KEY_X, 0x0);
	KeyDown(window, chip, Renderer::InputHandler::KEY_C, 0xB);
	KeyDown(window, chip, Renderer::InputHandler::KEY_V, 0xF);

	//
	KeyUP(window, chip, Renderer::InputHandler::KEY_1, 0x1);
	KeyUP(window, chip, Renderer::InputHandler::KEY_2, 0x2);
	KeyUP(window, chip, Renderer::InputHandler::KEY_3, 0x3);
	KeyUP(window, chip, Renderer::InputHandler::KEY_4, 0xC);

	KeyUP(window, chip, Renderer::InputHandler::KEY_Q, 0x4);
	KeyUP(window, chip, Renderer::InputHandler::KEY_W, 0x5);
	KeyUP(window, chip, Renderer::InputHandler::KEY_E, 0x6);
	KeyUP(window, chip, Renderer::InputHandler::KEY_R, 0xD);

	KeyUP(window, chip, Renderer::InputHandler::KEY_A, 0x7);
	KeyUP(window, chip, Renderer::InputHandler::KEY_S, 0x8);
	KeyUP(window, chip, Renderer::InputHandler::KEY_D, 0x9);
	KeyUP(window, chip, Renderer::InputHandler::KEY_F, 0xE);

	KeyUP(window, chip, Renderer::InputHandler::KEY_Y, 0xA);
	KeyUP(window, chip, Renderer::InputHandler::KEY_X, 0x0);
	KeyUP(window, chip, Renderer::InputHandler::KEY_C, 0xB);
	KeyUP(window, chip, Renderer::InputHandler::KEY_V, 0xF);
}