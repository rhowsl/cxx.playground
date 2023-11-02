#include <iostream>
#include <format>
#include <coroutine>
#include <thread>
#include <stacktrace>
#include <format>
#include <optional>
#include <iomanip>

struct coro_promise;

// tipo da corotina, que segura o objeto de promessa
struct coro : public std::coroutine_handle<coro_promise> {
	using promise_type = coro_promise;
};

// objeto de promessa, responsável pelo controle do estado de execução
//
// dos traços de um objeto de promessa, temos:
//
// -- Objeto de retorno? --
// (tipo da corotina) get_return_object()
//
// -- Essas duas não podem lançar exceções --
// (suspend_never ou suspend_always) initial_suspend() noexcept
// (suspend_never ou suspend_always) final_suspend() noexcept
//
// -- Retorno e erro --
// (void) return_void(), chamado quando { co_return; }
//
// (void) unhandled_exception(), chamado quando uma exceção ocorre no contexto da corotina
// 		nota: exceções são propagas de volta para o invocador
//
// (T?) return_value(expr), onde expr vem de { co_return expr; } 
// 		onde T é não-void
//
// uma corotina não pode resumir mais de uma vez

struct coro_promise {
	// sera que pode ser vazio?
	coro get_return_object() {
		std::cout << "get_return_object\n";
		return {};
	}

	// init
	// 	std::suspend_always: execução pregiçosa
	// 	std::suspend_never: execução instantânea?
	std::suspend_never initial_suspend() noexcept {
		std::cout << "initial_suspend\n";

		// retorna um suspend_never em branco
		return {};
	}

	std::suspend_never final_suspend() noexcept {
		std::cout << "final_suspend\n";

		// retorna um suspend_never em branco
		return {};
	}

	void unhandled_exception() {
		std::cout << "erro!!\n";
		return;
	}

	void return_void() {
		void print_thread_id();
		print_thread_id();
		std::cout << "retorno void\n";
		return;
	}
};

void print_thread_id() {
	std::cout << "bom dia, sou a thread " << std::this_thread::get_id() << std::endl;
}

void b() {
	using namespace std::chrono_literals;
	while (true) {
		print_thread_id();
		std::this_thread::sleep_for(2s);
	}
}

coro a() {
	struct awaitable {
		std::optional<std::coroutine_handle<coro_promise>> coro;

		void await_suspend(std::coroutine_handle<coro_promise> corotina) {
			print_thread_id();
			std::cout << "await_suspend\n";

			std::jthread thread_corotina([corotina]() { corotina.resume(); });
			coro = corotina;
		}

		void await_resume() {
			print_thread_id();
			std::cout << "await_resume\n";

			coro.and_then([](std::coroutine_handle<coro_promise> a) {
				std::cout << "pronto? " << std::boolalpha << a.done() << std::endl;
				return std::optional { a };
			});
		}

		bool await_ready() {
			print_thread_id();
			std::cout << "await_ready\n";

			return false;
		}
	};

	co_await awaitable {};
	std::cout << "aaaaa\n";

	co_return;
}

int main() {
	// O controle é transferido...?
	a();
}
