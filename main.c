#include <SDL2/SDL.h>
#include <unistd.h>

#define WIN_W 1500
#define WIN_H 900

int main(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("Couldn't initialize SDL2");
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("VPaper", 200, 200, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* rend = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);

	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderClear(rend);

	sleep(5);

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
