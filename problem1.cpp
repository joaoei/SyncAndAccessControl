/* 
Descrição:
	Problema do Banheiro Unissex
	
	Um escritório contém um banheiro que pode ser utilizado tanto por homens quanto por mulheres, 
	mas não por ambos ao mesmo tempo. Se um homem estiver no banheiro, outros homens podem entrar, 
	porém eventuais mulheres que desejem utilizar o banheiro devem esperar ele ficar vazio. Se uma 
	mulher estiver no banheiro, outras mulheres podem entrar, porém eventuais homens que desejem 
	utilizar o banheiro devem esperar ele ficar vazio.

Como compilar: 
	g++ -std=c++11 problem1.cpp -o pro1 -lpthread -lrt
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
sem_t mulherr_qtd;

//Contadores de quantidade de pessoas
int contador_mulher = 0;
int contador_homem = 0;

void* entra_mulher (void* arg) {
	sem_wait(&troca_sexo);
	
	sem_wait(&mulher);
	contador_mulher = contador_mulher + 1;
	if (contador_mulher == 1)
		sem_wait(&banheiro_vazio);
	sem_post(&mulher);

	sem_post(&troca_sexo);
	
	sem_wait(&mulher_qtd);
	
	std::cout << "\nMulher entrando no banheiro...\n";
	std::cout << "\nQuantidade de mulheres no banheiro: " << contador_mulher << "\n";
	
	//Calculando tempo randômico no qual a pessoa vai ficar no banheiro
	srand((unsigned)time(0));
	int tempo_banheiro = rand()%(16) + 5;
	sleep(tempo_banheiro);
	
	std::cout << "\nMulher saindo do banheiro após passar " << tempo_banheiro << " segundo(s)";

	sem_post(&mulher_qtd);
	
	sem_wait(&mulher);
	contador_mulher = contador_mulher - 1;
	if (contador_mulher == 0)
		sem_post(&banheiro_vazio);
	sem_post(&mulher);
}

void* entra_homem (void* arg) {
	sem_wait(&troca_sexo);

	sem_wait(&homem);
	contador_homem = contador_homem + 1;
	if (contador_homem == 1)
		sem_wait(&banheiro_vazio);
	sem_post(&homem);
	
	sem_post(&troca_sexo);
	
	sem_wait(&homem_qtd);
	
	std::cout << "\nHomem entrando no banheiro...\n";
	std::cout << "\nQuantidade de homens no banheiro: " << contador_homem << "\n";
	
	//Calculando tempo randômico no qual a pessoa vai ficar no banheiro
	srand((unsigned)time(0));
	int tempo_banheiro = rand()%(16) + 5;
	sleep(tempo_banheiro);
	
	std::cout << "\nHomem saindo do banheiro após passar " << tempo_banheiro << " segundo(s)";

	sem_post(&homem_qtd);
	
	sem_wait(&homem);
	contador_homem = contador_homem - 1;
	if (contador_homem == 0)
		sem_post(&banheiro_vazio);
	sem_post(&homem);
}

int main( int argc, char * argv[] ) {
	//Limite padãro
	unsigned int limite = 5;
	
	//Se algum valor for passado por parâmetro
	if (argc >= 2) {
		unsigned int tam;
		std::stringstream( argv[1] ) >> tam;
		
		//O limite é alterado se valor inserido for maior que 0 
		if (tam > 0) {
			limite = tam;
		} else {
			std::cout << "\nValor inserido é inválido, limite setado como 5\n";
		}
	}

	//Inicializando os semáforos
	sem_init(&banheiro_vazio, 0, 1);
	sem_init(&troca_sexo, 0, 1);
	sem_init(&homem, 0, 1);
	sem_init(&mulher, 0, 1);
	sem_init(&homem_qtd, 0, limite);
	sem_init(&mulher_qtd, 0, limite);
	
	//Array de threads
	pthread_t threads[20];
	int pessoa;

	for (int i = 0; i < 20; i++) {
		srand((unsigned)time(0));
		pessoa = rand()%(2);

		//Cria uma thread de um homem ou de uma mulher, dependendo do número gerado randomicamete
		if (pessoa == 0) {
			pthread_create(&threads[i],NULL,entra_mulher, NULL);			
		} else {
			pthread_create(&threads[i],NULL,entra_homem, NULL);			
		}

		//Tempo para outra thread ser criada
		sleep(rand()%(5) + 1);
	}

	for (int i = 0; i < 20; i++) {
		pthread_join(threads[i],NULL);		
	}

	//Destroindo os semáforos
	sem_destroy(&banheiro_vazio);
	sem_destroy(&homem);
	sem_destroy(&mulher);
	sem_destroy(&homem_qtd);
	sem_destroy(&mulher_qtd);

	return 0;
}
