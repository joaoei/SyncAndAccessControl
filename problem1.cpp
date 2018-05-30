/* COMPILAR COM 
*	g++ -std=c++11 problem1.cpp -o pro1 -lpthread -lrt
*/

#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sstream>

//Semáforos
sem_t banheiro_vazio;
sem_t troca_sexo;
sem_t homem;
sem_t mulher;
sem_t homem_qtd;
sem_t mulher_qtd;

int contador_mulher = 0;
int contafor_homem = 0;

void* entra_mulher (void* arg) {
	sem_wait(&troca_sexo);
	
	sem_wait(&mulher);
	if (contador_mulher == 0)
		sem_wait(&banheiro_vazio);
	sem_post(&mulher);

	sem_post(&troca_sexo);
	
	sem_wait(&mulher_qtd);
	
	contador_mulher = contador_mulher + 1;
	std::cout << "\nMulher entrando no banheiro...\n";
	std::cout << "\nQuantidade de mulheres no banheiro: " << contador_mulher << "\n";
	
	//Calculando tempo aleatório no qual a pessoa vai ficar no banheiro
	srand((unsigned)time(0));
	int tempo_banheiro = rand()%(16) + 5;
	sleep(tempo_banheiro);
	
	std::cout << "\nMulher saindo do banheiro após passar " << tempo_banheiro << " segundo(s)";
	contador_mulher = contador_mulher - 1;

	sem_post(&mulher_qtd);
	
	sem_wait(&mulher);
	if (contador_mulher == 0)
		sem_post(&banheiro_vazio);
	sem_post(&mulher);
}

void* entra_homem (void* arg) {
	sem_wait(&troca_sexo);

	sem_wait(&homem);
	if (contafor_homem == 0)
		sem_wait(&banheiro_vazio);
	sem_post(&homem);
	
	sem_post(&troca_sexo);
	
	sem_wait(&homem_qtd);
	
	contafor_homem = contafor_homem + 1;
	std::cout << "\nHomem entrando no banheiro...\n";
	std::cout << "\nQuantidade de homens no banheiro: " << contafor_homem << "\n";
	
	//Calculando tempo aleatório no qual a pessoa vai ficar no banheiro
	srand((unsigned)time(0));
	int tempo_banheiro = rand()%(16) + 5;
	sleep(tempo_banheiro);
	
	std::cout << "\nHomem saindo do banheiro após passar " << tempo_banheiro << " segundo(s)";
	contafor_homem = contafor_homem - 1;

	sem_post(&homem_qtd);
	
	sem_wait(&homem);
	if (contafor_homem == 0)
		sem_post(&banheiro_vazio);
	sem_post(&homem);
}

int main( int argc, char * argv[] ) {
	unsigned int limite = 5;
	if (argc >= 2) {
		unsigned int tam;
		std::stringstream( argv[1] ) >> tam;
		if (tam > 0)
			limite = tam;
	}

	int contador_mulher = 0;
	int contafor_homem = 0;

	sem_init(&banheiro_vazio, 0, 1);
	sem_init(&troca_sexo, 0, 1);
	sem_init(&homem, 0, 1);
	sem_init(&mulher, 0, 1);
	sem_init(&homem_qtd, 0, limite);
	sem_init(&mulher_qtd, 0, limite);
	
	pthread_t threads[20];
	int pessoa;
	for (int i = 0; i < 20; i++) {
		srand((unsigned)time(0));
		pessoa = rand()%(2);
		if (pessoa == 0) {
			pthread_create(&threads[i],NULL,entra_mulher, NULL);			
		} else {
			pthread_create(&threads[i],NULL,entra_homem, NULL);			
		}

		sleep(rand()%(5) + 1);
	}

	for (int i = 0; i < 20; i++) {
		pthread_join(threads[i],NULL);		
	}

	sem_destroy(&banheiro_vazio);
	sem_destroy(&homem);
	sem_destroy(&mulher);
	sem_destroy(&homem_qtd);
	sem_destroy(&mulher_qtd);

	return 0;
}
