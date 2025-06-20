
#include "collision.h"



char simple_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return FALSE;
    if(ax2 < bx1) return FALSE;
    if(ay1 > by2) return FALSE;
    if(ay2 < by1) return FALSE;

    return TRUE;
}

char four_sides_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2){
    char collision = FALSE;

    if(ax1 > bx2) 
        return FALSE;
    
    if(ax2 < bx1) 
        return FALSE;
    

    if(ax1 < bx1)
        collision = 1; //horizontal left-right collide
    else collision = 2; //horizontal right-left collide

    //confere as alturas para saber se a colisão aconteceu
    if(ay1 > by2) 
        return FALSE;
    if(ay2 < by1) 
        return FALSE;

    if(ay1 == by1)
        return collision;
    else if (ay2 < by2)
        collision = 3; //cima para baixo
    else collision =  4; //de baixo para cima ay2 > by2

    return collision;;
}

char complex_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2){
    char collision = FALSE;

    if(ax1 > bx2) 
        return FALSE;
    
    if(ax2 < bx1) 
        return FALSE;
    

    if(ax1 < bx1)
        collision = 1; //horizontal left-right collide
    else collision = 2; //horizontal right-left collide

    //confere as alturas para saber se a colisão aconteceu
    if(ay1 > by2) 
        return FALSE;
    if(ay2 < by1) 
        return FALSE;

    if(ay1 == by1)
        return collision;
    else if (ay2 < by2)
        collision += 2; //retorna 3 se for cima pra baixo pela esquerda e 4 se pela direita
    else collision +=  4; //de baixo para cima ay2 > by2 5 se for pela esquerda e 6 pela direita

    return collision;;
}

