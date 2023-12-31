#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "chip8.h"

const uint8_t fontset[80] = 
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void init_chip8_sdl(Chip8 *chip8) {
	chip8->sdl = malloc(sizeof(Sdl));
	if (chip8->sdl == NULL) {
		perror("Could not allocate memory for the sdl utils of chip-8\n");
		exit(EXIT_FAILURE);
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		perror("Could not initialize SDL\n");
		exit(EXIT_FAILURE);
	}

	chip8->sdl->window = SDL_CreateWindow("Chip-8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	chip8->sdl->renderer = SDL_CreateRenderer(chip8->sdl->window, -1, SDL_RENDERER_ACCELERATED);

    SDL_AudioSpec desired, obtained;

    desired.freq = SAMPLE_RATE;
    desired.format = AUDIO_S16SYS;
    desired.channels = 1;
    desired.samples = SAMPLES; 
    desired.callback = NULL;

	chip8->sdl->audio_device = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
	if (chip8->sdl->audio_device == 0) {
        fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void destroy_chip8_sdl(Chip8 *chip8) {
	SDL_DestroyRenderer(chip8->sdl->renderer);
	SDL_DestroyWindow(chip8->sdl->window);
	SDL_CloseAudioDevice(chip8->sdl->audio_device);
	SDL_Quit();	
}

void init_chip8_stack(Chip8 *chip8) {
	Stack *stack = malloc(sizeof(uint16_t) * STACK_HEIGHT);
	if (stack == NULL) {
		perror("Could not allocate memory for the chip-8 stack\n");
		exit(EXIT_FAILURE);
	}
	memset(stack->stack, 0, sizeof(stack->stack));
	stack->sp = 0;
	chip8->stack = stack;
}

void destroy_chip8_stack(Chip8 *chip8) {
	free(chip8->stack);
}

void init_chip8(Chip8 *chip8) {
	init_chip8_sdl(chip8);
	init_chip8_stack(chip8);

	memset(chip8->memory, 0, sizeof(chip8->memory));
	memcpy(chip8->memory + FONTSET_START, fontset, sizeof(fontset));
	memset(chip8->registers, 0, sizeof(chip8->registers));
	memset(chip8->screen, BLACK, sizeof(chip8->screen));
	memset(chip8->keys, 0, sizeof(chip8->keys));
	chip8->pc = ROM_START;
}

void destroy_chip8(Chip8 *chip8) {
	destroy_chip8_sdl(chip8);
	destroy_chip8_stack(chip8);

	free(chip8);
}

void load_rom(Chip8 *chip8, const char *rom_path) {
	FILE *rom = fopen(rom_path, "rb");
	if (rom == NULL) {
		printf("Could not open rom at %s\n", rom_path);
		exit(EXIT_FAILURE);
	}
	fread(chip8->memory + ROM_START, sizeof(uint8_t), MEMORY_SIZE - ROM_START - 1, rom);

	fclose(rom);
}


