#include <SDL2/SDL.h>
#include <unistd.h>

#define WIN_W 1500
#define WIN_H 900
#define CELL_W 10
#define NUM_ROWS WIN_H / CELL_W
#define NUM_COLS WIN_W / CELL_W

void drawGrid(SDL_Renderer*);
void drawCells(SDL_Renderer*, int**);
void updateCells(int**);
void tick(SDL_Renderer*, int, int**);

int main(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("Couldn't initialize SDL2 video subsystem");
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("VPaper", 200, 200, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* rend = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);

	int running = 1;
	SDL_Event event;

	int** cells = malloc(sizeof(int*) * NUM_ROWS);
	for(int i = 0; i < NUM_ROWS; i++) {
		cells[i] = malloc(sizeof(int*) * NUM_COLS);
		memset(cells[i], 0, sizeof(int) * NUM_COLS);
	}

	cells[1][1] = 1;

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type){
			case SDL_MOUSEBUTTONDOWN:
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				int gridInd = mouseX / CELL_W;
				int gridInd2 = mouseY / CELL_W;
				cells[gridInd2][gridInd] = 1;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_q)
					running = 0;
				break;
			}
		}
		tick(rend, 35500, cells);
		updateCells(cells);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void drawGrid(SDL_Renderer* rend){
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 100);
	for(int i = 0; i < WIN_H; i += CELL_W) {
		SDL_RenderDrawLine(rend, 0, i, WIN_W, i);
	}
	for(int i = 0; i < WIN_W; i += CELL_W) {
		SDL_RenderDrawLine(rend, i, 0, i, WIN_H);
	}
}

void drawCells(SDL_Renderer* rend, int** cells){
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 100);
	for(int i = 0; i < NUM_ROWS; i++) {
		for(int j = 0; j < NUM_COLS; j++) {
			if(cells[i][j] == 1) {
				SDL_Rect rect = (SDL_Rect) {j * CELL_W, i * CELL_W, CELL_W, CELL_W};
				SDL_RenderFillRect(rend, &rect);
			}
		}
	}
}

/*
111	110	011	010
0	1	1	1


101	100	001	000
1	0	1	0
*/
void updateCells(int** cells){
	for(int i = 1; i < NUM_ROWS; i++) {
		for(int j = 0; j < NUM_COLS; j++) {
			if(cells[i][j] == 0) {
				if(cells[i][j - 1] == 1 && cells[i][j + 1] == 1) {
					cells[i - 1][j] = 1;
				}
				else if(cells[i][j - 1] == 1 && cells[i][j + 1] == 0) {
					cells[i - 1][j] = 0;
				}
				else if(cells[i][j - 1] == 0 && cells[i][j + 1] == 1) {
					cells[i - 1][j] = 1;
				}
				else if(cells[i][j - 1] == 0 && cells[i][j + 1] == 0) {
					cells[i - 1][j] = 0;
				}
			}
			else if(cells[i][j] == 1) {
				if(cells[i][j - 1] == 1 && cells[i][j + 1] == 1) {
					cells[i - 1][j] = 0;
				}
				else if(cells[i][j - 1] == 1 && cells[i][j + 1] == 0) {
					cells[i - 1][j] = 1;
				}
				else if(cells[i][j - 1] == 0 && cells[i][j + 1] == 1) {
					cells[i - 1][j] = 1;
				}
				else if(cells[i][j - 1] == 0 && cells[i][j + 1] == 0) {
					cells[i - 1][j] = 1;
				}
			}
		}
	}
}

void tick(SDL_Renderer* rend, int updateInteveralNanoSeconds, int** cells){
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderClear(rend);

	//drawGrid(rend);
	drawCells(rend, cells);

	SDL_RenderPresent(rend);
	usleep(updateInteveralNanoSeconds);
}
