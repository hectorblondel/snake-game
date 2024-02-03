//details the snake class

#include "include.h"

std::random_device rd ;
std::mt19937 gen(rd()) ;

std::string directions[4] = {"UP", "DOWN", "LEFT", "RIGHT"} ;





bool searchElement(const std::list<position>& lst, position element) {
    for (position elem : lst) {
        if (elem.x == element.x && elem.y == element.y) {
            return true;
        }
    }
    return false;
}


bool Snake::is_valid(position p){
    //returns true if the position p is not already in the snake and is in the grid
    return (!searchElement(snake_cases, p) && p.is_valid(grid_size_x, grid_size_y)) ;
}

Snake::Snake(int grid_size_x, int grid_size_y) {

    std::uniform_int_distribution<> dis_dir(0, 3) ;

    this->grid_size_x = grid_size_x ;
    this->grid_size_y = grid_size_y ;
    snake_length = 3 ;
    position body_part ;
    position new_body_part ;
    position old_body_part ;
    
    this->snake_color[0] = 0 ;
    this->snake_color[1] = 0 ;
    this->snake_color[2] = 0 ;
    this->snake_color[3] = 255 ;
    this->snake_color_increment[0] = 4 ;
    this->snake_color_increment[1] = 4 ;
    this->snake_color_increment[2] = 4 ;
    this->snake_color_increment[3] = 0 ;//last one is useless

    //generate random position for the head
    
    body_part.x = grid_size_x/2 ;
    body_part.y = grid_size_y/2 ;


    for (int i = 0 ; i < snake_length ; i++) {
        snake_cases.push_front(body_part) ;
        new_body_part = body_part.neighbour(directions[dis_dir(gen)]) ;
        while (!is_valid(new_body_part)) {
            new_body_part = body_part.neighbour(directions[dis_dir(gen)]) ;
        }
        old_body_part = body_part ;
        body_part = new_body_part ; //there will always be a body part and an old one
    }


    current_direction = position::get_direction(old_body_part, body_part) ;

    position food = new_food() ;
    this->food = food ;
} ;

snake_update *  Snake::move(std::string direction) {

        //returns the new position of the head and the tail position to be deleted.
        snake_update * res = (snake_update *)malloc(sizeof(snake_update)) ;
    


        position new_head = snake_cases.front().neighbour(direction) ;
        position old_tail = snake_cases.back() ;
        position old_head = snake_cases.front() ;
        //res[0] = new_head ;
        //res[1] = old_tail ;
        if (new_head.equals(old_tail) || is_valid(new_head)) {
            snake_cases.push_front(new_head) ;
            if (new_head.x == food.x && new_head.y == food.y) {
                snake_length++ ;
                this->food = new_food() ;
                res->eaten = true ;

            } else {
                res->eaten = false ;
                snake_cases.pop_back() ;
            }
            current_direction = position::get_direction(old_head, new_head) ;
            res->new_head = new_head ;
            res->old_tail = old_tail ;
            res->dead = false ;
        } else {
            res->dead = true ;
        }
        return res ;
    

    }

position Snake::new_food() {
    //returns a new food that is not in the snake

    std::uniform_int_distribution<> dis_x(0, this->grid_size_x - 1) ;
    std::uniform_int_distribution<> dis_y(0, this->grid_size_y - 1) ;

    position food ;
    food.x = dis_x(gen) ;
    food.y = dis_y(gen) ;
    while (searchElement(snake_cases, food)) {
        food.x = dis_x(gen) ;
        food.y = dis_y(gen) ;
    }
    return food ;    
}

void Snake::update_snake_color() {
    std::uniform_int_distribution<> dis_color(0, 2) ;
    int limit = 200 ; 
    int color_index = dis_color(gen) ; //the color to be updated
    if (this->snake_color[color_index] == limit && this->snake_color_increment[color_index] > 0 \
        || this->snake_color[color_index] == 0 && this->snake_color_increment[color_index] < 0){
        this->snake_color_increment[color_index] = - this->snake_color_increment[color_index] ;
    } 
    this->snake_color[color_index] += this->snake_color_increment[color_index] ;

}

std::list<position> Snake::get_snake_cases() {
    return snake_cases ;
}
