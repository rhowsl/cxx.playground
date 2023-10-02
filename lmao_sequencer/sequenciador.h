#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <optional>
#include <functional>
#include <format>
#include <cmath>

#include "tipos.h"

constexpr const int NO_PASSOS = 16;

#include "tipos.h"

class sequenciador {
	private:
		std::thread _thread_controle;
		std::chrono::steady_clock _clock;
		std::array<coluna, NO_COLUNAS> _colunas;
		std::array<std::array<std::optional<comando>, NO_COLUNAS>, NO_PASSOS> _passos;
		std::atomic<unsigned int> _posicao_cursor = 0;

		std::atomic<bool> _em_loop = false;
		std::atomic<bool> _killswitch = false;
		std::atomic<bool> _pausado = true;

		// greenhouse - theriocide (arc,regn)
		double _bpm = 83 * 2;

	public:
		sequenciador();
		~sequenciador();

		void tocar(bool loop);
		void pausar();
		void parar();
		void definir_passo(int canal, int passo, comando comando);

	private:
		void loop_thread_controle();

	private:
		void _loop_thread_controle();
};
