#include <iostream>
#include <chrono>
#include <SDL.h>
#include "include.h"



struct Windowconfig {
    SDL_Window *win ;
    SDL_Renderer *ren ;

    int screen_size_x ;
    int screen_size_y ;
    int grid_size_x ;
    int grid_size_y ;
    int cell_size_x = screen_size_x / grid_size_x;
    int cell_size_y = screen_size_y / grid_size_y;

    bool is_good_position(position p) {
        if (p.x < 0 || p.x >= grid_size_x || p.y < 0 || p.y >= grid_size_y) {
            printf("error: position (%d,%d) out of grid\n", p.x, p.y);
            return false ;
        }
        else {
            return true ;
        }
    }

    void drawcase_colorcode(position p, color c){
        if (!is_good_position(p)) {
            return ;
        }
        SDL_Rect rect;
        rect.x = p.x * cell_size_x;
        rect.y = p.y * cell_size_y;
        rect.w = cell_size_x;
        rect.h = cell_size_y;
        SDL_SetRenderDrawColor(ren, c[0],c[1],c[2],c[3]);  // Black color
        SDL_RenderFillRect(ren, &rect);
    }

    void drawcase(position p, std::string color, bool small = false){
        if (!is_good_position(p)) {
            return ;
        }


        int a,b,c,d ; //color code
        if (color == "WHITE"){
            a = 255 ;
            b = 255 ;
            c = 255 ;
            d = 255 ;
        }
        else if (color == "BLACK"){
            a = 0 ;
            b = 0 ;
            c = 0 ;
            d = 255 ;
        }
        else if (color == "RED"){
            a = 255 ;
            b = 0 ;
            c = 0 ;
            d = 255 ;
        }
        else {
            printf("Error: invalid color\n");
            return ;
        }

        SDL_Rect rect;

        if (small) {
            rect.x = p.x * cell_size_x + cell_size_x/4;
            rect.y = p.y * cell_size_y + cell_size_y/4;
            rect.w = cell_size_x/2;
            rect.h = cell_size_y/2;
        } else {
            rect.x = p.x * cell_size_x;
            rect.y = p.y * cell_size_y;
            rect.w = cell_size_x;
            rect.h = cell_size_y;
        }

        SDL_SetRenderDrawColor(ren, a,b,c,d);  // Black color
        SDL_RenderFillRect(ren, &rect);
    }

    void erasecase(position p){
        if (!is_good_position(p)) {
            return ;
        }

        SDL_Rect rect;
        rect.x = p.x * cell_size_x;
        rect.y = p.y * cell_size_y;
        rect.w = cell_size_x;
        rect.h = cell_size_y;
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);  // White color
        SDL_RenderFillRect(ren, &rect);
    
    }

} ;

float compute_update_period(float start_update_period, int level){
    //we will reach start_update_period/2 at an infnite level
    float tau = 30 ; //the characteristic time of the exponential
    return start_update_period/2 + exp(- level/tau) * start_update_period/2 ;
    
}


int main(int argc, char* argv[]) {
    
    /* Initialize SDL */

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Window *win = SDL_CreateWindow("Snake Game", 100, 100, 480, 480, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Clear screen with a color (e.g., blue)
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);  // White color
    SDL_RenderClear(ren);


    /* Initialize variables */

    int screen_size_x = 480 ;
    int screen_size_y = 480 ;
    int grid_size_x = 20 ;
    int grid_size_y = 20 ;
    int cell_size_x = screen_size_x / grid_size_x ;
    int cell_size_y = screen_size_y / grid_size_y ;
    Windowconfig window_config = {win, ren, screen_size_x, screen_size_y, grid_size_x, grid_size_y, cell_size_x, cell_size_y} ;

    float start_update_period = 0.23 ; //seconds
    int level = 0 ;
    float update_period = compute_update_period(start_update_period, level) ;
    float period_inaction = 3. ; //The period during which nothing happens
    float time_elapsed ;
    float current_time ;

    printf("Level %d\n", level);
    printf("Update period: %f\n", update_period);
    

    snake_update * last_update ; //a table of two positions, the first one is the new head, the second one is the previous tail


    Snake snake(grid_size_x, grid_size_y) ;
    for (auto p : snake.get_snake_cases()) {
        window_config.drawcase_colorcode(p, snake.snake_color) ;
        snake.update_snake_color();
    }

    window_config.drawcase(snake.food, "BLACK",true) ;

    SDL_RenderPresent(ren);



    // Main loop
    bool quit = false;
    SDL_Event e;
    auto last_clock_time = std::chrono::high_resolution_clock::now() ;
    auto init_clock_time = last_clock_time ;

    while (!quit && ((std::chrono::high_resolution_clock::now() - init_clock_time).count() / 1e9 <= period_inaction)){
            while(SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
    }

    while (!quit) {


        while (SDL_PollEvent(&e) || ((std::chrono::high_resolution_clock::now() - last_clock_time).count() / 1e9 <= update_period)) {

            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {  // Key pressed
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (snake.current_direction != "DOWN"){
                            snake.current_direction = "UP" ;
                        }
                        break ;
                    case SDLK_DOWN:
                        if (snake.current_direction != "UP"){
                            snake.current_direction = "DOWN" ;
                        }
                        break;
                    case SDLK_LEFT:
                        if (snake.current_direction != "RIGHT"){
                            snake.current_direction  = "LEFT" ;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (snake.current_direction != "LEFT"){
                            snake.current_direction = "RIGHT" ;
                        }
                        break;
                    default:
                        printf("Error: invalid key\n");
                        break;
                        
                }
            }
        }

        //When the time elapsed is greater than the update period, we update the snake
        last_clock_time = std::chrono::high_resolution_clock::now() ;
        last_update = snake.move(snake.current_direction) ;

        if (last_update->dead) {
            printf("Game over\n");
            quit = true ;
        } else {
            if (last_update->eaten) {
                window_config.drawcase_colorcode(last_update->new_head, snake.snake_color) ;
                snake.update_snake_color();
                if (snake.snake_length >= grid_size_x * grid_size_y) {
                    printf("You win !\n");
                    quit = true ;
                } else {
                    update_period = compute_update_period(start_update_period, ++level) ;
                    printf("--------------------\n");
                    printf("Level %d\n", level);
                    printf("New update period: %f\n", update_period);
                    //The food has been eaten and a new one has been generated
                    window_config.drawcase(snake.food, "BLACK",true) ;
                }
            } else {
                window_config.erasecase(last_update->old_tail) ;
                window_config.drawcase_colorcode(last_update->new_head, snake.snake_color) ;
                snake.update_snake_color();
            }
            SDL_RenderPresent(ren);
        }

        
    }


    

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}