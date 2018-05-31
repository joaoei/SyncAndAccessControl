/**
 * Compilar: g++ problem2.cpp -o pro2 -Wall -lpthread -std=c++11
 * Executar: ./pro2
 */

#include <iostream> // cout
#include <forward_list> // forward_list
#include <thread> // thread
#include <mutex> // mutex
#include <algorithm> // find

std::forward_list<int> f_list; // Lista encadeada simples compartilhada pelas threads
std::mutex mtx; // Bloqueio compartilhado entre as threads

/**
 * Método usado apenas para visualização da lista durante remoções e inserções.
 */
void print_list(std::forward_list<int> &f_list) {
	std::cout << "### Minha lista agora contém:";
	for ( int& x : f_list )
		std::cout << "->[" << x << "]";
	std::cout << ".\n\n";
}

/**
 * Método para inserir valores no final da lista.
 */
void I(int value) {
	mtx.lock();

	auto before_end = f_list.before_begin();

	std::cout << ">>> Navegando ao fim da lista encadeada.\n";
	for(auto it = f_list.begin(); it != f_list.end(); ++it) {
		before_end++;
	}

	std::cout << "+++ Inserindo valor " << value << " ao fim da lista.\n\n";
	f_list.insert_after(before_end, value);

	print_list(f_list);

	mtx.unlock();
}

/**
 * Método para buscar valores na lista.
 * Percorre a lista procurando um valor e verifica sua existência.
 */
void B(int value) {
	bool found = std::find(f_list.begin(), f_list.end(), value) != f_list.end();

	std::cout << "=== Resultado da busca por " << value << " na lista: ";
	
	if (found) {
		std::cout << "encontrado." << "\n\n";
	} else {
		std::cout << "não encontrado." << "\n\n";
	}
}

/*
 * Método para remover valores (se existentes) na lista.
 */
void R(int value) {
	mtx.lock();
	
	std::cout << "--- Removendo valor " << value << " (se existir) da lista.\n\n";
	f_list.remove(value);

	print_list(f_list);

	mtx.unlock();
}

/*
 * Método principal utilizado para criar threads e rodá-las.
 */
int main( int argc, char * argv[] ) {
	  std::thread threads[30]; // Vetor de 30 threads que terá 10 de cada tipo.

	int j = 0; // Variável auxiliar utilizada somente pra inserir/remover/buscar valores de 0 a 9 na lista.

	// Criação de 10 threads para inserir valores de 0 a 9 na lista através da varivável auxiliar j.
	for (int i = 0; i < 10; ++i) {
		threads[i] = std::thread(I, j);
		j++;
	}

	// Criação de 10 threads para remover valores de 0 a 9 na lista através da varivável auxiliar j.
	j = 0;
	for (int i = 10; i < 20; ++i) {
		threads[i] = std::thread(R, j);
		j++;
	}

	// Criação de 10 threads para buscar valores de 0 a 9 na lista através da varivável auxiliar j.
	j = 0;
	for (int i = 20; i < 30; ++i) {
		threads[i] = std::thread(B, j);
		j++;
	}

	// Aguardando finalização das threads.
	for (auto& th : threads) {
		th.join();
	}

	return 0;
}