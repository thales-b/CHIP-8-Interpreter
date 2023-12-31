#include "chip8.h"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <math.h>

void keypress(Chip8 *chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        chip8->keys[0x1] = 1;
                        break;
                    case SDLK_2:
                        chip8->keys[0x2] = 1;
                        break;
                    case SDLK_3:
                        chip8->keys[0x3] = 1;
                        break;
                    case SDLK_4:
                        chip8->keys[0xC] = 1;
                        break;
                    case SDLK_q:
                        chip8->keys[0x4] = 1;
                        break;
                    case SDLK_w:
                        chip8->keys[0x5] = 1;
                        break;
                    case SDLK_e:
                        chip8->keys[0x6] = 1;
                        break;
                    case SDLK_r:
                        chip8->keys[0xD] = 1;
                        break;
                    case SDLK_a:
                        chip8->keys[0x7] = 1;
                        break;
                    case SDLK_s:
                        chip8->keys[0x8] = 1;
                        break;
                    case SDLK_d:
                        chip8->keys[0x9] = 1;
                        break;
                    case SDLK_f:
                        chip8->keys[0xE] = 1;
                        break;
                    case SDLK_z:
                        chip8->keys[0xA] = 1;
                        break;
                    case SDLK_x:
                        chip8->keys[0x0] = 1;
                        break;
                    case SDLK_c:
                        chip8->keys[0xB] = 1;
                        break;
                    case SDLK_v:
                        chip8->keys[0xF] = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        chip8->keys[0x1] = 0;
                        break;
                    case SDLK_2:
                        chip8->keys[0x2] = 0;
                        break;
                    case SDLK_3:
                        chip8->keys[0x3] = 0;
                        break;
                    case SDLK_4:
                        chip8->keys[0xC] = 0;
                        break;
                    case SDLK_q:
                        chip8->keys[0x4] = 0;
                        break;
                    case SDLK_w:
                        chip8->keys[0x5] = 0;
                        break;
                    case SDLK_e:
                        chip8->keys[0x6] = 0;
                        break;
                    case SDLK_r:
                        chip8->keys[0xD] = 0;
                        break;
                    case SDLK_a:
                        chip8->keys[0x7] = 0;
                        break;
                    case SDLK_s:
                        chip8->keys[0x8] = 0;
                        break;
                    case SDLK_d:
                        chip8->keys[0x9] = 0;
                        break;
                    case SDLK_f:
                        chip8->keys[0xE] = 0;
                        break;
                    case SDLK_z:
                        chip8->keys[0xA] = 0;
                        break;
                    case SDLK_x:
                        chip8->keys[0x0] = 0;
                        break;
                    case SDLK_c:
                        chip8->keys[0xB] = 0;
                        break;
                    case SDLK_v:
                        chip8->keys[0xF] = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

uint8_t get_key() {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    return 0x1;
                case SDLK_2:
                    return 0x2;
                case SDLK_3:
                    return 0x3;
                case SDLK_4:
                    return 0xC;
                case SDLK_q:
                    return 0x4;
                case SDLK_w:
                    return 0x5;
                case SDLK_e:
                    return 0x6;
                case SDLK_r:
                    return 0xD;
                case SDLK_a:
                    return 0x7;
                case SDLK_s:
                    return 0x8;
                case SDLK_d:
                    return 0x9;
                case SDLK_f:
                    return 0xE;
                case SDLK_z:
                    return 0xA;
                case SDLK_x:
                    return 0x0;
                case SDLK_c:
                    return 0xB;
                case SDLK_v:
                    return 0xF;
                default:
                    break;
            }
        }
    }
    return 0; 
}

void sound(Chip8 *chip8) {
    if (chip8->sound_timer <= 0) return;

    const int num_samples = (SOUND_DURATION * SAMPLE_RATE) / 1000;
    const double increment = FREQUENCY * 2.0 * M_PI / SAMPLE_RATE;
    double angle = 0.0;

    Sint16* buffer = (Sint16*)malloc(num_samples * sizeof(Sint16));

    for (int i = 0; i < num_samples; ++i) {
        buffer[i] = (Sint16)(32767.0 * sin(angle)); 

        angle += increment;
        if (angle > 2.0 * M_PI) {
            angle -= 2.0 * M_PI;
        }
    }

    SDL_QueueAudio(chip8->sdl->audio_device, buffer, num_samples * sizeof(Sint16));
    SDL_PauseAudioDevice(chip8->sdl->audio_device, 0);

    SDL_Delay(SOUND_DURATION); 

    free(buffer);
}