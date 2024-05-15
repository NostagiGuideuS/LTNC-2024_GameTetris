#pragma once

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;

// Screen
const int g_w = 12;
const int g_h = 24;
const int BlockSize_Input = 24;
const int BlockSize_Output = 24;
const int g_Width = (2 * g_w + 2) * BlockSize_Output;
const int g_Height = (g_h + 2) * BlockSize_Output;

const int t_Delay = 1000 / 50;
static int t_Drop = 50; // after 50 lapse, a.k.a 1 second

static int hardcore = 0;
static long long HighScore[5];
static int scoreMode = 2;

// Audio file
static Mix_Chunk* whenMove; // Channel 2
static Mix_Chunk* whenDrop; // Channel 2
static Mix_Chunk* whenAdd2Base; // Channel 1
static Mix_Chunk* whenGameOver; // Channel 3
static Mix_Chunk* whenGet1Line[4]; // Channel 1
static Mix_Chunk* whenChangeFrame; // Channel 4
static Mix_Chunk* whenClickOption; // Channel 2

// Copy by 23021686, 2324II_INT2215_25

const int playGroundColor_R = 100;
const int playGroundColor_G = 200;
const int playGroundColor_B = 255;
