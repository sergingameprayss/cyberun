#include <stdlib.h>
#include "gun.h"

pistol* pistol_create(){																				//Implementação da função "pistol_create" (!)

	pistol *new_pistol = (pistol*) malloc(sizeof(pistol));												//Aloca a memória na heap para uma instância de pistola (!)
	if (!new_pistol) return NULL;																		//Verifica o sucesso da alocação de memória; retorna NULL em caso de falha (!)
	new_pistol->timer = 0;																				//Inicializa o relógio de disparos em zero (pronto para atirar) (!)
	new_pistol->shots = NULL;	
	new_pistol->bullet_art = NULL;	
	new_pistol->damage = 1; //dano padrao																	//Inicializa a lista de disparos com NULL; ou seja, sem disparos (!)
	return new_pistol;																					//Retorna a nova instância de pistola (!)
}

bullet* pistol_shot(int x, int y, char trajectory, pistol *gun){			//Implementação da função "pistol_shot" (!)
	
	bullet *new_bullet = bullet_create(x, y, trajectory, gun->shots);									//Cria uma nova instância de projétil a ser disparado (!)
	if (!new_bullet) return NULL;																		//Verifica o sucesso da alocação de memória; retorna NULL em caso de falha (!)
	return new_bullet;																					//Retorna uma nova instância de projétil (!)
}

void pistol_destroy(pistol *gun){																	//Implementação da função "pistol_destroy" (!)

	bullet *sentinel;																					//Sentinela para a remoção de projéteis ativos (!)
	for (bullet *index = gun->shots; index != NULL; index = sentinel){								//Para cada projétil na lista de disparos (!)
		sentinel = (bullet*) index->next;																//Armazena o próximo projétil (!)
		bullet_destroy(index);																			//Chama o destrutor do projétil atual (!)
	}
	
	free(gun);																						//Libera a memória da instância de pistola (!)
}

void update_bullets(pistol *gun, int left_limit, int right_limit){
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index = gun->shots; index != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		if (!index->trajectory) index->x += BULLET_MOVE;																																					//Se a trajetória for para a esquerda, atualiza a posição para a esquerda (!)
		else if (index->trajectory == 1) index->x -= BULLET_MOVE;																																			//Se a trajetória for para a direita, atualiza a posição para a esquerda (!)
		
		if ((index->x < left_limit) || (index->x > right_limit)){																																						//Verifica se o projétil saiu das bordas da janela (!)
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis (!)
				previous->next = index->next;																																								//Se não for, salva o próximo projétil (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = (bullet*) previous->next;																																							//Atualiza para o próximo projétil (!)
			}
			else {																																															//Se for o primeiro projétil da lista (!)
				gun->shots = (bullet*) index->next;																																					//Atualiza o projétil no início da lista (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = gun->shots;																																									//Atualiza para o próximo projétil (!)
			}
		}
		else{																																																//Se não saiu da tela (!)
			previous = index;																																												//Atualiza o projétil anterior (para a próxima iteração) (!)
			index = (bullet*) index->next;																																									//Atualiza para o próximo projétil (!)
		}
	}
}