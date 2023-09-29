#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <optional>
#include <functional>

#include "tipos.h"

class sequenciador {
	private:
		std::thread _thread_controle;
		std::chrono::steady_clock _clock;
		std::atomic<int> _posicao_cursor;
		std::array<coluna, NO_COLUNAS> _colunas;

		int _bpm = 150;
		bool _em_loop = false;

	public:
		sequenciador() = default;
		~sequenciador() = default;

		void tocar(bool loop);
		void pausar();
		void parar();

	private:
		void _loop_thread_controle();
};
