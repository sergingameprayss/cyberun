#ifndef __BULLET__ 																																//Guardas de inclusão (!)
#define __BULLET__																																//Guardas de inclusão (!)

#define BULLET_MOVE 10

typedef struct {																																//Definição da estrutura de um pŕojétil (!)
	int x;																															//Local, no eixo x, onde se localiza a bala (!)
	int y;																															//Local, no eixo y, onde se localiza a bala (!)
	char trajectory;																													//Trajetória da bala
	struct bullet *next; 																														//Próxima bala, se houver, formando uma lista encadeada (!)
} bullet;																																		//Definição do nome da estrutura (!)

bullet* bullet_create(int x, int y, char trajectory, bullet *next);												//Protótipo da função de criação de uma bala (!)
void bullet_move(bullet *elements);																												//Protótipo da função de movimentação de uma bala (!)	
void bullet_destroy(bullet *element);	

//void update_bullets(bullet *shots, int left_limit, int right_limit);

#endif																																			//Guardas de inclusão (!)