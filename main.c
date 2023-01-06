#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 576
#define SCREEN_HEIGHT 576


static int is_press = 0;
static int mouse_press = 0;

int* checker(int* board, int x, int y);
void update(int* board);
void assign(int* board, int c);

void assign(int* board, int c){
	int mx,my;
	SDL_GetMouseState(&mx,&my);
	if(c==1)board[(mx/64)+(my/64)*9] += 1;
	if(c==2)board[(mx/64)+(my/64)*9] -= 1;
}


int* checker(int* board, int x, int y){
    int rock[] = {1,2,3,4,5,6,7,8,9};
    int* arr = malloc(sizeof(int)*9);
    SDL_memmove(arr, &rock[0], sizeof(int)*9);
    for(int i=0;i<9;i++){
        //for horizontal!
        if(board[x+i*9]!=0) arr[board[x+i*9]-1] = 0;
        //for vertical!!
        if(board[i+y*9]!=0) arr[board[i+y*9]-1] = 0;
    }

	int bx = (x/3)*3;
	int by = (y/3)*3;
	for(int i=by;i<3+by;i++){
		for(int j=bx;j<3+bx;j++){
			int index = (j+i*9);
			if(board[index]!=0) arr[board[index]-1] = 0;
		}
		//printf("\n");
	}
	//printf("\n--------------------\n");
    return arr;
}


void update(int* board){
	//finding da stuff with the lowest possibility!!.
	int min = 999;
	int tx,ty;
	for(int y=0;y<9;y++){
		for(int x=0;x<9;x++){
			if(board[x+y*9] !=0) continue;
			//getting how many possible value an XY grid can have!!
			int* possible_arr = checker(board,x,y);
			int temp_counter = 0;	
			for(int i=0;i<9;i++) temp_counter += (possible_arr[i] != 0);
			
			if(temp_counter < min && temp_counter !=0){
				min = temp_counter;
				tx = x; 
				ty = y;
			}
		}
	}
	if(min == 999) return;
	//assigning some value to that tiny stuff!!.
	int* possible_arr = checker(board,tx,ty);
	int temp_arr[10]; int temp_size=0;
	for(int i=0;i<9;i++){
		if(possible_arr[i]!=0){
			temp_arr[temp_size] = possible_arr[i];
			temp_size++;
			//board[tx+ty*9] = possible_arr[i];
		}
	}
	
	int seed = ((tx*3434)*(ty*4343))%temp_size;
	board[tx+ty*9] = temp_arr[seed];

}


int main(int argc, char** argv){
    //the game data and stuff
    int board[81];
	for(int i=0;i<81;i++) board[i] = 0;

    
    //The Whole SDL stuff!!!.
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    //initiazling all the textures!!.
    const char* names[]={"asset/0.png", "asset/1.png","asset/2.png","asset/3.png", "asset/4.png","asset/5.png","asset/6.png", "asset/7.png","asset/8.png","asset/9.png"};
    SDL_Surface* lettuce_sur[10];
    for(int i=0;i<10;i++) lettuce_sur[i] = IMG_Load(names[i]);

    SDL_Texture* lettuce_tex[10];
    for(int i=0;i<10;i++) lettuce_tex[i] = SDL_CreateTextureFromSurface(renderer,lettuce_sur[i]);
    
    SDL_Rect srcrect;
    SDL_Rect dstrect;

    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = 40;
    srcrect.h = 40;

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 64;
    dstrect.h = 64;


    char running = 1;
    while(running){
        //SDL_Event poll!!
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = 0;
                    break;
				
				case SDL_MOUSEBUTTONDOWN:
					if(mouse_press == 0){
						if(event.button.button == SDL_BUTTON_LEFT)	assign(&board[0],1);
						if(event.button.button == SDL_BUTTON_RIGHT) assign(&board[0],2);
						mouse_press = 1;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					mouse_press = 0;
					break;

				//for updating the board stuff!!.
				case SDL_KEYDOWN:
					if(SDLK_a == event.key.keysym.sym && is_press == 0){
						update(&board[0]);
						is_press = 1;
					}
					break;
				case SDL_KEYUP:
						is_press = 0;
					break;
                default:
                    break;
            }
        }
        //the whole drawing stuff happens here!!
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        //drawing all da stuff!!
        for(int i=0;i<81; i++){
            int x = i%9;
            int y = i/9;
            dstrect.x = x*64;
            dstrect.y = y*64;
            SDL_RenderCopy(renderer, lettuce_tex[board[i]], &srcrect, &dstrect);
        }
        SDL_RenderPresent(renderer);
    }
    return 0;
}



