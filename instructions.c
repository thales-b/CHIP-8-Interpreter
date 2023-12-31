#include "chip8.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "io.h"

void clear_screen(Chip8 *chip8) {
	SDL_SetRenderDrawColor(chip8->sdl->renderer, BLACK, BLACK, BLACK, 255);
	SDL_RenderClear(chip8->sdl->renderer);
	SDL_RenderPresent(chip8->sdl->renderer);

	memset(chip8->screen, 0, sizeof(chip8->screen));
}

void jump(Chip8 *chip8, uint16_t nnn) {
	chip8->pc = nnn;
}

void set_vx_const(Chip8 *chip8, uint8_t x, uint8_t nn) {
	chip8->registers[x] = nn;
}

void set_i(Chip8 *chip8, uint16_t nnn) {
	chip8->i_register = nnn;
}

void draw(Chip8 *chip8, uint8_t x, uint8_t y, uint8_t n) {
    uint8_t x_pos = chip8->registers[x] % SCREEN_WIDTH;
    uint8_t y_pos = chip8->registers[y] % SCREEN_HEIGHT;

    chip8->registers[VF] = 0;

    for (int i = 0; i < n; i++) {
        uint8_t byte = chip8->memory[chip8->i_register + i];
        for (int j = 0; j < SPRITE_WIDTH; j++) {
            uint8_t pixel = (byte >> (7 - j)) & 0x1;

            int screen_x = (x_pos + j) % SCREEN_WIDTH;
            int screen_y = (y_pos + i) % SCREEN_HEIGHT;			

            if (pixel == WHITE && chip8->screen[screen_x][screen_y] == WHITE) {
                	chip8->registers[VF] = 1;
			}	
			chip8->screen[screen_x][screen_y] ^= pixel;
			uint8_t color = chip8->screen[screen_x][screen_y] * WHITE;
            SDL_Rect rect = {
                .x = screen_x * PIXEL_WIDTH,
                .y = screen_y * PIXEL_HEIGHT,
                .w = PIXEL_WIDTH,
                .h = PIXEL_HEIGHT
            };
            SDL_SetRenderDrawColor(chip8->sdl->renderer, color, color, color, 255);
            SDL_RenderFillRect(chip8->sdl->renderer, &rect);		
		}
    }
    SDL_RenderPresent(chip8->sdl->renderer);
}

void return_subroutine(Chip8 *chip8) {
	chip8->pc = chip8->stack->stack[--chip8->stack->sp];
}

void subroutine_call(Chip8 *chip8, uint16_t nnn) {
	chip8->stack->stack[chip8->stack->sp] = chip8->pc;
	chip8->stack->sp++;
	chip8->pc = nnn;
}

void vx_eq_skip(Chip8 *chip8, uint8_t x, uint8_t nn) {
	if (chip8->registers[x] == nn) chip8->pc += 2;
}

void vx_neq_skip(Chip8 *chip8, uint8_t x, uint8_t nn) {
	if (chip8->registers[x] != nn) chip8->pc += 2;
}

void vx_vy_eq_skip(Chip8 *chip8, uint8_t x, uint8_t y) {
	if (chip8->registers[x] == chip8->registers[y]) chip8->pc += 2;
}

void add_vx_const(Chip8 *chip8, uint8_t x, uint8_t nn) {
	chip8->registers[x] += nn;
}

void set_vx_vy(Chip8 *chip8, uint8_t x, uint8_t y) {
	chip8->registers[x] = chip8->registers[y];
}

void or_vx_vy(Chip8 *chip8, uint8_t x, uint8_t y) {
	chip8->registers[x] |= chip8->registers[y];
}

void and_vx_vy(Chip8 *chip8, uint8_t x, uint8_t y) {
	chip8->registers[x] &= chip8->registers[y];
}

void xor_vx_vy(Chip8 *chip8, uint8_t x, uint8_t y) {
	chip8->registers[x] ^= chip8->registers[y];
}

void add_vx_vy(Chip8 *chip8, uint8_t x, uint8_t y) {
	uint8_t sum = chip8->registers[x] + chip8->registers[y];
	chip8->registers[VF] = sum > 255 ? 1 : 0;
	chip8->registers[x] = sum & 0x0FF; 
}

void sub_vx_vy(Chip8 *chip8, uint8_t x, uint8_t y) {
	uint8_t sub = chip8->registers[x] - chip8->registers[y];
	chip8->registers[VF] = sub > 0  ? 1 : 0;
	chip8->registers[x] = sub; 
}

void rshift_vx(Chip8 *chip8, uint8_t x) {
	chip8->registers[VF] = chip8->registers[x] & 0x1;
	chip8->registers[x] >>= 1;
}

void sub_vy_vx(Chip8 *chip8, uint8_t x, uint8_t y) {
	uint8_t sub = chip8->registers[y] - chip8->registers[x];
	chip8->registers[VF] = sub > 0  ? 1 : 0;
	chip8->registers[x] = sub; 
}

void lshift_vx(Chip8 *chip8, uint8_t x) {
	chip8->registers[VF] = (chip8->registers[x] >> 7) & 0x1;
	chip8->registers[x] <<= 1;
}

void vx_vy_neq_skip(Chip8 *chip8, uint8_t x, uint8_t y) {
	if (chip8->registers[x] != chip8->registers[y]) chip8->pc += 2;
}

void jump_v0(Chip8 *chip8, uint16_t nnn) {
	chip8->pc = chip8->registers[0] + nnn;
}

void set_vx_rand(Chip8 *chip8, uint8_t x, uint8_t nn) {
	chip8->registers[x] = rand() & nn;
}

void key_eq_skip(Chip8 *chip8, uint8_t x) {
	if (chip8->keys[chip8->registers[x]]) chip8->pc += 2;
}

void key_neq_skip(Chip8 *chip8, uint8_t x) {
	if (!chip8->keys[chip8->registers[x]]) chip8->pc += 2;
}

void set_vx_delay(Chip8 *chip8, uint8_t x) {
	chip8->registers[x] = chip8->delay_timer;
}

void set_vx_key(Chip8 *chip8, uint8_t x) {
	chip8->registers[x] = get_key();
}

void set_delay(Chip8 *chip8, uint8_t x) {
	chip8->delay_timer = chip8->registers[x];
}

void set_sound_timer(Chip8 *chip8, uint8_t x) {
	chip8->sound_timer = chip8->registers[x];
}

void add_i_vx(Chip8 *chip8, uint8_t x) {
	chip8->i_register += chip8->registers[x];
}

void set_i_sprite(Chip8 *chip8, uint8_t x) {
	chip8->i_register = FONTSET_START + (chip8->registers[x] * 5);
}

void set_i_bcd(Chip8 *chip8, uint8_t x) {
	uint8_t vx = chip8->registers[x];
	chip8->memory[chip8->i_register] = vx / 100;
    chip8->memory[chip8->i_register + 1] = (vx / 10) % 10;
    chip8->memory[chip8->i_register + 2] = (vx % 100) % 10;
}

void dump(Chip8 *chip8, uint8_t x) {
	for (uint8_t i = 0; i <= x; i++) {
		chip8->memory[chip8->i_register + i] = chip8->registers[i];
	}
}

void load(Chip8 *chip8, uint8_t x) {
	for (uint8_t i = 0; i <= x; i++) {
		chip8->registers[i] = chip8->memory[chip8->i_register + i];
	}
}

void execute_instruction(Chip8 *chip8) {
	uint16_t instruction = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
	uint8_t op = (instruction >> 12) & 0x0F;
	uint8_t x = (instruction >> 8) & 0x0F;
	uint8_t y = (instruction >> 4) & 0x0F;
	uint8_t n = instruction & 0x0F;
	uint8_t nn = instruction & 0x0FF;
	uint16_t nnn = instruction & 0x0FFF;

	switch (op) {
		case 0x0:
			switch (nn) {
				case 0xE0:
					clear_screen(chip8);
					break;
				case 0xEE:
					return_subroutine(chip8);
					break;
			}
			break;
		case 0x1:
			jump(chip8, nnn);
			break;
		case 0x2:
			subroutine_call(chip8, nnn);
			break;
		case 0x3:
			vx_eq_skip(chip8, x, nn);
			break;
		case 0x4:
			vx_neq_skip(chip8, x, nn);
			break;
		case 0x5:
			vx_vy_eq_skip(chip8, x, y);
			break;
		case 0x6:
			set_vx_const(chip8, x, nn);
			break;
		case 0x7:
			add_vx_const(chip8, x, nn);
			break;
		case 0x8:
			switch (n) {
				case 0x0:
					set_vx_vy(chip8, x, y);
					break;
				case 0x1:
					or_vx_vy(chip8, x, y);
					break;
				case 0x2:
					and_vx_vy(chip8, x, y);
					break;
				case 0x3:
					xor_vx_vy(chip8, x, y);
					break;
				case 0x4:
					add_vx_vy(chip8, x, y);
					break;
				case 0x5:
					sub_vx_vy(chip8, x, y);
					break;
				case 0x6:
					rshift_vx(chip8, x);
					break;
				case 0x7:
					sub_vy_vx(chip8, x, y);
					break;
				case 0xE:
					lshift_vx(chip8, x);
					break;
			}
			break;
		case 0x9:
			vx_vy_neq_skip(chip8, x, y);
			break;
		case 0xA:
			set_i(chip8, nnn);
			break;
		case 0xB:
			jump_v0(chip8, nnn);
			break;
		case 0xC:
			set_vx_rand(chip8, x, nn);
		case 0xD:
			draw(chip8, x, y, n);
			break;
		case 0xE:
			switch (nn) {
				case 0x9E:
					key_eq_skip(chip8, x);
					break;
				case 0xA1:
					key_neq_skip(chip8, x);
					break;
			}
			break;
		case 0xF:
			switch (nn) {
				case 0x07:
					set_vx_delay(chip8, x);
					break;
				case 0x0A:
					set_vx_key(chip8, x);
					break;
				case 0x15:
					set_delay(chip8, x);
					break;
				case 0x18:
					set_sound_timer(chip8, x);	
					break;
				case 0x1E:
					add_i_vx(chip8, x);
					break;
				case 0x29:
					set_i_sprite(chip8, x);
					break;
				case 0x33:
					set_i_bcd(chip8, x);
					break;
				case 0x55:
					dump(chip8, x);
					break;
				case 0x65:
					load(chip8, x);
					break;	
			}
			break;
		default:
			printf("Unknown instruction: %d\n", instruction);
			exit(EXIT_FAILURE);
	}
	
	if (op != 0x1 && op != 0x2) chip8->pc += 2;
}




