#include <SDL2/SDL.h>
#include <unistd.h>

#define WIN_W 1500
#define WIN_H 900

void tick(SDL_Renderer* rend, int updateInteveralNanoSeconds){
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderClear(rend);
	SDL_RenderPresent(rend);
	usleep(updateInteveralNanoSeconds);
}

int main(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("Couldn't initialize SDL2");
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("VPaper", 200, 200, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* rend = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);

	int running = 1;
	SDL_Event event;

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type){
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_q)
					running = 0;
				break;
			}
		}

		tick(rend, 1000);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
