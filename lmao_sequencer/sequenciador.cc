#include "sequenciador.h"

sequenciador::sequenciador() {
	_thread_controle = std::thread(&sequenciador::loop_thread_controle, this);
}

sequenciador::~sequenciador() {
	_killswitch = true;
	_thread_controle.join();
}

void sequenciador::tocar(bool loop) {
	_em_loop = loop;
	_pausado = false;
}

void sequenciador::pausar() {
	_pausado = true;
}

void sequenciador::parar() {
	_posicao_cursor = 0;
	_pausado = true;
}

void sequenciador::definir_passo(int canal, int passo, comando cmd) {
	_passos[passo][canal] = cmd;
}

void sequenciador::loop_thread_controle() {
	while (true) {
		if (_killswitch) {
			_killswitch = false;
			break;
		}

		if (_pausado)
			continue;

		auto& linha_atual = _passos.at(_posicao_cursor);
		unsigned int canal = 0;

		std::stringstream saida;
		saida << std::format("T {} | ", _posicao_cursor.load());

		for (const auto& cmd : linha_atual) {
			cmd
				.and_then([this, canal, &saida](const comando& c) {
					saida << std::format("CH {}: E {}, V {} | ", canal, (int)c.first, c.second.value_or(-1));
					return std::optional<comando> { std::nullopt };
				});

			canal++;
		}

		std::cout << saida.str() << std::endl;

		if (_posicao_cursor + 1 >= _passos.size()) {
			if (_em_loop) {
				_posicao_cursor = 0;
			} else {
				_pausado = true;
				continue;
			}
		} else {
			_posicao_cursor++;
		}

		double intervalo_compasso = (1.0 / (2.0 * (double)_bpm)) * 60;
		uint64_t intervalo_compasso_ns = std::round(intervalo_compasso * 1000000000);

		std::this_thread::sleep_for(std::chrono::nanoseconds(intervalo_compasso_ns));
	}
}
