#include <thread>
#include <utility>

#include "sequenciador.h"
#include "tipos.h"

int main() {
	sequenciador seq;

	seq.definir_passo(0, 0, std::make_pair(tipo_comando::note_on, 1));
	seq.definir_passo(0, 4, std::make_pair(tipo_comando::note_on, 2));
	seq.definir_passo(0, 8, std::make_pair(tipo_comando::note_on, 3));
	seq.definir_passo(0, 12, std::make_pair(tipo_comando::note_on, 4));

	seq.tocar(true);

	while (true) {
		std::this_thread::yield();
	}
}
