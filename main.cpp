#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "main.h"
}

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600


// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};

int rand_int(int min, int max) {
	return min + rand() % (max + 1 - min);
}
// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


// rysowanie prostok¹ta o d³ugoœci boków l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};


void MovementHandler(const Uint8* keystate, int death, int pause, float& xMovement, double& etiSpeed)
{
	if (keystate[SDL_SCANCODE_LEFT] && pause == 0) {
		xMovement -= 2;
	}
	else if (keystate[SDL_SCANCODE_RIGHT]&& pause == 0) {
		xMovement += 2;
	}
	else if (keystate[SDL_SCANCODE_UP] && pause == 0) {
		etiSpeed = 64;
	}
	else if (keystate[SDL_SCANCODE_DOWN] && pause == 0) {
		etiSpeed = 4;
	}
	else if (keystate[SDL_KEYUP]) {
		etiSpeed = 16;
	}
}

void TextureHandler(float xMovement, int& death, SDL_Surface* screen, int czarny, int zielony, SDL_Surface* eti, int distance, double& screenLeftX, double& screenLeftXBefore,
	double& screenRightX, double& screenRightXBefore, int& distanceBefore, int& distanceRight, int& distanceRightBefore, int& currentRect)
{
	
	int j = 0;
	CollisionHandler(xMovement, death, screenLeftX, screenRightX, screenLeftXBefore, screenRightXBefore, currentRect);
	SDL_FillRect(screen, NULL, czarny);
	if (currentRect >= 16) {
		currentRect = 0;
		screenLeftXBefore = screenLeftX;
		screenRightXBefore = screenRightX;
		distanceRightBefore = distanceRight;
		int r = rand_int(1, 5);
		switch (r) {
		case 1:
			if ((screenRightX-20) - (screenLeftX+20) > 100 && (screenRightX - 20) - (screenLeftX + 20) < 600) {
				screenLeftX += 20;
				screenRightX -= 20;
				distanceRight += 20;
			}
			break;
		case 2:
			if ((screenRightX + 20) - (screenLeftX - 20) > 100 && (screenRightX + 20) - (screenLeftX - 20) < 600) {
				screenLeftX -= 20;
				screenRightX += 20;
				distanceRight -= 20;
			}
			break;
		case 3:
			if ((screenRightX - 50) - (screenLeftX + 50) > 100 && (screenRightX - 50) - (screenLeftX + 50) < 600) {
				screenLeftX += 50;
				screenRightX -= 50;
				distanceRight += 50;
			}
			break;
		case 4:
			if ((screenRightX + 50) - (screenLeftX - 50) > 100 && (screenRightX + 50) - (screenLeftX - 50) < 600) {
				screenLeftX -= 50;
				screenRightX += 50;
				distanceRight -= 50;
			}
			break;

		}
	} 
	if (distance != distanceBefore) {
		distanceBefore = distance;
		currentRect++;
	}
	for (int i = 0; i < 16; i++) {
		 j = i * (SCREEN_HEIGHT / 16);
		 if (i <= currentRect) {
			DrawRectangle(screen, 0, j, screenLeftX, (SCREEN_HEIGHT / 16), zielony, zielony);
			DrawRectangle(screen, screenRightX, j, SCREEN_WIDTH / 4 + distanceRight, SCREEN_HEIGHT / 16, zielony, zielony);

		 }
		 else if (i > currentRect) {
			 DrawRectangle(screen, 0, j, screenLeftXBefore, (SCREEN_HEIGHT / 16), zielony, zielony);
			 DrawRectangle(screen, screenRightXBefore, j, SCREEN_WIDTH / 4 + distanceRightBefore , SCREEN_HEIGHT / 16, zielony, zielony);
		 }

	}
	DrawSurface(screen, eti, xMovement, SCREEN_HEIGHT / 2);
}
void CollisionHandler(float xMovement, int& death, double screenLeftX, double screenRightX, double screenLeftXBefore, double screenRightXBefore, int currentRect)
{
	float yMovement = SCREEN_HEIGHT / 2;
	if (currentRect >= 6) {
		if (xMovement - 21 < screenLeftX || xMovement + 21 > screenRightX) {
			death = 1;
		}
		else death = 0;
	}
	else {
		if (xMovement - 21 < screenLeftXBefore || xMovement + 21 > screenRightXBefore) {
			death = 1;
		}
		else death = 0;
	}
	
}
int InitializationHandler(int& rc, SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& screen, SDL_Texture*& scrtex, bool& retflag)
{
	retflag = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe³noekranowy / fullscreen mode
	//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


	SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2017");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);
	retflag = false;
	return {};
}

void KeyEventHandler(SDL_Event& event, int& quit, int& death, double& etiSpeed, float& score, float& xMovement, double& worldTime, int& pause, double& screenLeftX, double& screenLeftXBefore,
	double& screenRightX, double& screenRightXBefore, int& distanceBefore, int& distanceRight, int& distanceRightBefore, int& currentRect)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
		if (event.key.keysym.sym == SDLK_n) {
			xMovement = SCREEN_WIDTH / 2;
			worldTime = 0, score = 0, screenLeftX = SCREEN_WIDTH / 4, screenRightX = SCREEN_WIDTH - (SCREEN_WIDTH / 4), screenLeftXBefore = SCREEN_WIDTH / 4, 
				screenRightXBefore = SCREEN_WIDTH - (SCREEN_WIDTH / 4), distanceBefore = 0, distanceRight = 0, distanceRightBefore = 0, currentRect = 0;
		}
		if (event.key.keysym.sym == SDLK_p && pause == 0) {
			pause = 1;
		}
		else if (event.key.keysym.sym == SDLK_p && pause == 1) {
			pause = 0;
		}
		break;
	case SDL_KEYUP:
		break;
	case SDL_QUIT:
		quit = 1;
		break;
	};
}

void StringRectangles(SDL_Surface* screen, int czerwony, int niebieski, int death, char  text[128], SDL_Surface* charset, int pause, double worldTime, double fps, float score)
{
	//// tekst informacyjny / info text
	DrawRectangle(screen, 1, 1, SCREEN_WIDTH - 8, 30, czerwony, niebieski);
	if (pause == 1) {
		sprintf(text, "GAME PAUSED. Press P to resume.");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2, text, charset);
	}
	sprintf(text, "Jakub Andrunik 193166  Time Elapsed = %.1lfs  %.0lf FPS/s  Score = %.0f", worldTime, fps, score);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
	DrawRectangle(screen, SCREEN_WIDTH - (SCREEN_WIDTH / 4), SCREEN_HEIGHT - SCREEN_HEIGHT / 12, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 16, czerwony, niebieski);
	sprintf(text, "a,b,c,d,e,f,g,i,j");
	DrawString(screen, SCREEN_WIDTH - (SCREEN_WIDTH / 4) + strlen(text) *2, SCREEN_HEIGHT - SCREEN_HEIGHT / 12 + 15, text, charset);
	////	      "esc - exit, \030 - faster, \031 - slower"
	//sprintf(text, "esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
	//drawstring(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
}
// main
#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc, tick = 0;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	SDL_Event event;
	SDL_Surface* screen, *charset;
	SDL_Surface* eti;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	bool retflag;
	int retval = InitializationHandler(rc, window, renderer, screen, scrtex, retflag);
	if (retflag) return retval;

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(picture.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
		SDL_SetColorKey(charset, true, 0x000000);

		eti = SDL_LoadBMP("./picture.bmp");
		if (eti == NULL) {
			printf("SDL_LoadBMP(picture.bmp) error: %s\n", SDL_GetError());
			SDL_FreeSurface(charset);
			SDL_FreeSurface(screen);
			SDL_DestroyTexture(scrtex);
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
			SDL_Quit();
			return 1;
		}

		char text[128];
		int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
		float score = 0;
		float deathScore = 0;
		t1 = SDL_GetTicks();

		frames = 0;
		fpsTimer = 0;
		fps = 0;
		quit = 0;
		worldTime = 0;
		distance = 0;
		etiSpeed = 16;
		int death = 0, pause = 0, rectangleSpeed = 0;
		float xMovement = SCREEN_WIDTH / 2;
		float yMovement = SCREEN_HEIGHT / 2;
		double screenLeftX = SCREEN_WIDTH / 4, screenRightX = SCREEN_WIDTH - (SCREEN_WIDTH / 4);
		double screenLeftXBefore = SCREEN_WIDTH / 4, screenRightXBefore = SCREEN_WIDTH - (SCREEN_WIDTH / 4);
		int distanceBefore = 0, currentRect = 0, distanceRight = 0, distanceRightBefore = 0, currentDistance = 0;

		while (!quit) {
			const Uint8* keystate = SDL_GetKeyboardState(NULL);
			t2 = SDL_GetTicks();
			
			delta = (t2 - t1) * 0.001;
			t1 = t2;
			currentDistance = (int)distance;
			if (pause == 0) {
				worldTime += delta;
				distance += etiSpeed * delta;

				if (death == 0) {
					if((int)distance % 16 == 0 && (int)distance > currentDistance) score += 50;
				}
			}
			
			MovementHandler(keystate, death, pause, xMovement, etiSpeed);
			TextureHandler(xMovement, death, screen, czarny, zielony, eti, distance, screenLeftX, screenLeftXBefore, screenRightX, screenRightXBefore, 
				distanceBefore, distanceRight, distanceRightBefore, currentRect);

			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};
			while (SDL_PollEvent(&event)) {
				KeyEventHandler(event, quit, death, etiSpeed, score, xMovement, worldTime, pause, screenLeftX, screenLeftXBefore, screenRightX, screenRightXBefore,
					distanceBefore, distanceRight, distanceRightBefore, currentRect);
			};
			StringRectangles(screen, czerwony, niebieski, death, text, charset, pause, worldTime, fps, score);

			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);

			// obs³uga zdarzeñ (o ile jakieœ zasz³y) / handling of events (if there were any)
			frames++;
			tick++;
		};

		// zwolnienie powierzchni / freeing all surfaces
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return 0;
	}

	


	
