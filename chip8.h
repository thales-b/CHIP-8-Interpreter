#ifndef CHIP8_H
#define CHIP8_H

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

#define STACK_HEIGHT  	16
#define NUM_REGISTERS 	16
#define VF				15		
#define MEMORY_SIZE		4096
#define ROM_START		0x200
#define FONTSET_START	0x50
#define NUM_KEYS		16

#define WINDOW_HEIGHT	768
#define WINDOW_WIDTH	1024
#define SCREEN_HEIGHT	32	
#define SCREEN_WIDTH	64
#define SPRITE_WIDTH	8
#define PIXEL_HEIGHT	(WINDOW_HEIGHT / SCREEN_HEIGHT)
#define PIXEL_WIDTH		(WINDOW_WIDTH / SCREEN_WIDTH)
#define BLACK			0
#define WHITE			255
#define SAMPLE_RATE		44100
#define FREQUENCY		440.0
#define SAMPLES			2048
#define SOUND_DURATION	1


typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_AudioDeviceID audio_device;
} Sdl;

typedef struct {
	uint16_t stack[STACK_HEIGHT];
	uint8_t sp;
} Stack;


typedef struct {
	uint8_t registers[NUM_REGISTERS];
	Stack *stack;
	uint8_t memory[MEMORY_SIZE];
	uint16_t i_register;
	uint16_t pc;
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];
	Sdl *sdl;
	uint8_t keys[NUM_KEYS];
} Chip8;

extern const uint8_t fontset[80];

void init_chip8_sdl(Chip8 *chip8);
void destroy_chip8_sdl(Chip8 *chip8);
void init_chip8_stack(Chip8 *chip8);
void destroy_chip8_stack(Chip8 *chip8);
void init_chip8(Chip8 *chip8);
void destroy_chip8(Chip8 *chip8);
void load_rom(Chip8 *chip8, const char *rom_path);

#endif
