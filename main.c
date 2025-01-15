#include <SDL2/SDL.h>
#include <unistd.h>

#define WIN_W 1500
#define WIN_H 900
#define CELL_W 10 
#define NUM_ROWS WIN_H / CELL_W
#define NUM_COLS WIN_W / CELL_W
#define UPDATE_INTERVAL 25500

void drawGrid(SDL_Renderer*);
void drawCells(SDL_Renderer*, int**);
void updateCells(int**, int[]);
void shiftCells(int** cells);
int checkFirstRow(int* firstRow);
void tick(SDL_Renderer*, int**);

int main(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("Couldn't initialize SDL2 video subsystem");
		return -1;
	}

	int rule110[] = {0, 1, 1, 1, 0, 1, 1, 0};
	SDL_Window* window = SDL_CreateWindow("RULE110", 200, 200, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* rend = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);

	int** cells = malloc(sizeof(int*) * NUM_ROWS);
	for(int i = 0; i < NUM_ROWS; i++) {
		cells[i] = malloc(sizeof(int) * NUM_COLS);
		memset(cells[i], 0, sizeof(int) * NUM_COLS);
	}


	cells[NUM_ROWS-1][NUM_COLS-1] = 1;
	int gridInd, gridInd2;
	int rowOnePopulated = 0;
	int play = 0;
	int running = 1;
	SDL_Event event;
	tick(rend, cells);
	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type){
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_q)
					running = 0;
				if(event.key.keysym.sym == SDLK_SPACE)
					play = !play;
				break;
			}
		}
		if(play) {
			tick(rend, cells);
			if(!rowOnePopulated) {
				rowOnePopulated = checkFirstRow(cells[0]);
			} else {
				shiftCells(cells);
			}
			updateCells(cells, rule110);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

int checkFirstRow(int* firstRow){
	int isPopulated = 0,ind = 0;
	while(ind < NUM_COLS && !isPopulated) {
		isPopulated += firstRow[ind];
		ind++;
	}
	return isPopulated;
}

void drawGrid(SDL_Renderer* rend){
	for(int i = 0; i < WIN_H; i += CELL_W) {
		SDL_RenderDrawLine(rend, 0, i, WIN_W, i);
	}
	for(int i = 0; i < WIN_W; i += CELL_W) {
		SDL_RenderDrawLine(rend, i, 0, i, WIN_H);
	}
}

void drawCells(SDL_Renderer* rend, int** cells){
	SDL_SetRenderDrawColor(rend, 244, 185, 66, 255);
	for(int i = 0; i < NUM_ROWS; i++) {
		for(int j = 0; j < NUM_COLS; j++) {
			if(cells[i][j] == 1) {
				SDL_Rect rect = (SDL_Rect) {j * CELL_W, i * CELL_W, CELL_W, CELL_W};
				SDL_RenderFillRect(rend, &rect);
			}
		}
	}
}

int convertBinaryToInt(const char *s) {
	return (int) strtol(s, NULL, 2);
}

void updateCells(int** cells, int rule110[]){
	char* buffer = malloc(sizeof(char) * 3);
	for(int i = 1; i < NUM_ROWS; i++) {
		for(int j = 0; j < NUM_COLS; j++) {
			if(j == NUM_COLS - 1)
				sprintf(buffer, "%d%d%d", cells[i][j - 1], cells[i][j], cells[i][0]);
			else if(j == 0)
				sprintf(buffer, "%d%d%d", cells[i][NUM_COLS - 1], cells[i][j], cells[i][j + 1]);
			else
				sprintf(buffer, "%d%d%d", cells[i][j - 1], cells[i][j], cells[i][j + 1]);
			cells[i - 1][j] = rule110[convertBinaryToInt(buffer)];
		}
	}
	free(buffer);
}

void shiftCells(int** cells){
	for(int i = NUM_ROWS - 1; i > 0; i--) {
		for(int j = 0; j < NUM_COLS; j++) {
			cells[i][j] = cells[i - 1][j];
		}
	}
}


void tick(SDL_Renderer* rend, int** cells){
	SDL_SetRenderDrawColor(rend, 239, 242, 241, 255);
	SDL_RenderClear(rend);
	drawCells(rend, cells);
	SDL_RenderPresent(rend);
	usleep(UPDATE_INTERVAL);
}
