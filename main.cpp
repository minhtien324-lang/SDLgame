#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480, BLOCK_SIZE = 20;

struct snakeSegment
{
    int x, y;
};

enum class Direction
{
    UP, DOWN, LEFT, RIGHT

};

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Snake",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH , SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // varible game ;
    std::vector<snakeSegment> snake{
        {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2} };
    Direction direction = Direction::UP;
    int foodX = 0, foodY = 0;
    bool gameOver = false;

    // rand seed;
    std::srand(std::time(nullptr));
    // game loop;
    while (!gameOver){
        // handler event;
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                gameOver = true;
            }
        else if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
            case SDLK_UP:
                if (direction != Direction::DOWN){
                    direction = Direction::UP;
                }
                break;
            case SDLK_DOWN:
                if (direction != Direction::UP){
                    direction = Direction::DOWN;
                }
                break;
            case SDLK_LEFT:
                if (direction != Direction::RIGHT){
                    direction = Direction::LEFT;
                }
                break;
            case SDLK_RIGHT:
                if (direction != Direction::LEFT){
                    direction = Direction::RIGHT;
                }
                break;
            }
        }
    }
    //update game state
    switch (direction){
    case Direction::UP:
        snake.insert(snake.begin(),
                     {snake.front().x, snake.front().y - BLOCK_SIZE});
        break;
    case Direction::DOWN:
        snake.insert(snake.begin(),
                     {snake.front().x, snake.front().y + BLOCK_SIZE});
        break;
    case Direction::LEFT:
        snake.insert(snake.begin(), 
                    {snake.front().x - BLOCK_SIZE, snake.front().y});
        break;
    case Direction::RIGHT:
        snake.insert(snake.begin(), 
                    {snake.front().x + BLOCK_SIZE, snake.front().y});
        break;
    }
    if (snake.front().x == foodX && snake.front().y == foodY){
        foodX = std::rand() % (SCREEN_WIDTH / BLOCK_SIZE) * BLOCK_SIZE;
        foodY = std::rand() % (SCREEN_WIDTH / BLOCK_SIZE) * BLOCK_SIZE;
    }
    else{
        snake.pop_back();
    }
    // collision with wall
    if(snake.front().x < 0 || snake.front().x >= SCREEN_WIDTH || snake.front().y < 0 || snake.front().y >= SCREEN_HEIGHT){
        gameOver = true;
    }
    // collision with snake
    for(auto it = snake.begin() + 1; it != snake.end(); it++){
        if(snake.front().x == it -> x && snake.front().y == it -> y){
            gameOver = true;
            break;
        }
    }
    //clear screen
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    //food;
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_Rect foodRect {foodX, foodY, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    //Snake
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    for(const auto& segment : snake){
        SDL_Rect segmentRect{
            segment.x, segment.y, BLOCK_SIZE, BLOCK_SIZE
        };
        SDL_RenderFillRect(renderer, &segmentRect);
    }
        // update Screen 
        SDL_RenderPresent(renderer);
    }
   //clean up
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}