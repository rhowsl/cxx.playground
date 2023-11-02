#include <thread>
#include <utility>

#include "sequenciador.h"
#include "tipos.h"

int main() {
	sequenciador seq;

	// Canal 1
	seq.definir_passo(0, 0, std::make_pair(tipo_comando::note_on, 1));
	seq.definir_passo(0, 4, std::make_pair(tipo_comando::note_on, 2));
	seq.definir_passo(0, 8, std::make_pair(tipo_comando::note_on, 3));
	seq.definir_passo(0, 12, std::make_pair(tipo_comando::note_on, 4));

	// Canal 2
	seq.definir_passo(1, 0, std::make_pair(tipo_comando::note_on, 1));
	seq.definir_passo(1, 2, std::make_pair(tipo_comando::note_on, 2));
	seq.definir_passo(1, 4, std::make_pair(tipo_comando::note_on, 3));
	seq.definir_passo(1, 6, std::make_pair(tipo_comando::note_on, 4));
	seq.definir_passo(1, 8, std::make_pair(tipo_comando::note_on, 5));
	seq.definir_passo(1, 10, std::make_pair(tipo_comando::note_on, 6));
	seq.definir_passo(1, 12, std::make_pair(tipo_comando::note_on, 7));
	seq.definir_passo(1, 14, std::make_pair(tipo_comando::note_on, 8));
	
	seq.tocar(true);

	while (true) {
		std::this_thread::yield();
	}
}
