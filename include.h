#ifndef SNAKE_H
#define SNAKE_H

#include <list>
#include <string>
#include <random>
#include <iostream>
#include <cmath>


typedef int color[4] ;

struct position
{
    int x ;
    int y ;

    position neighbour(std::string direction) {
        if (direction == "UP") {
            return position {x, y-1} ;
        }
        else if (direction == "DOWN") {
            return position {x, y+1} ;
        }
        else if (direction == "LEFT") {
            return position {x-1, y} ;
        }
        else if (direction == "RIGHT") {
            return position {x+1, y} ;
        }
        else {
            printf("Error: invalid direction\n");
            return position {x, y} ;
        }
    }  

    bool is_valid(int grid_size_x, int grid_size_y) {
        if (x < 0 || x >= grid_size_x || y < 0 || y >= grid_size_y) {
            return false ;
        }
        else {
            return true ;
        }
    }

    static std::string get_direction(position p, position q){
        //Computes the direction from p to q
        if (p.x == q.x) {
            if (p.y < q.y) {
                return "DOWN" ;
            }
            else {
                return "UP" ;
            }
        }
        else {
            if (p.x < q.x) {
                return "RIGHT" ;
            }
            else {
                return "LEFT" ;
            }
        }
    }

    void print()
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl ;
    }

    bool equals(position q) {
        return (x == q.x && y == q.y) ;
    }
} ;


struct snake_update {
    bool eaten ;
    bool dead ;
    position new_head ;
    position old_tail ;
} ;


bool searchElement(const std::list<position>& lst, position element) ;


class Snake {
    
    private :

        int grid_size_x ; //rows
        int grid_size_y ; //columns
        std::list<position> snake_cases ;

    public :
        int snake_length ; 
        position food ;
        std::string current_direction ;
        Snake(int grid_size_x, int grid_size_y);
        snake_update * move(std::string direction);
        std::string get_current_direction();
        void change_direction(std::string direction);
        std::list<position> get_snake_cases();
        bool is_valid(position p);
        position new_food();
        color snake_color ; 
        color snake_color_increment ;
        void update_snake_color();

};

#endif // SNAKE_H