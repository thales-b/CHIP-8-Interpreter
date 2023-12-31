#include "chip8.h"
#include "instructions.h"
#include <SDL2/SDL_timer.h>
#include "io.h"


int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <path_to_rom>\n", argv[0]);
		return EXIT_FAILURE;
	}

	Chip8 *chip8 = malloc(sizeof(Chip8));
	if (chip8 == NULL) {
		printf("Could not allocate memory for chip-8\n");
		return EXIT_FAILURE;
	}
	init_chip8(chip8);
	load_rom(chip8, argv[1]);

	while (1) {
		keypress(chip8);
		sound(chip8);
		execute_instruction(chip8);
		if (chip8->delay_timer > 0) chip8->delay_timer--;
		if (chip8->sound_timer > 0) chip8->sound_timer--;
		SDL_Delay(1000 / 250); 
	}

	destroy_chip8(chip8);
	return EXIT_SUCCESS;
}

