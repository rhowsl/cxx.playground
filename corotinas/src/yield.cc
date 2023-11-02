#include <iostream>
#include <iomanip>
#include <optional>
#include <coroutine>
#include <format>
#include <cstdint>

struct promessa;

// coroutine_handle<Promise>
struct corotina : public std::coroutine_handle<promessa> {
	using promise_type = promessa;

	~corotina() {
		std::cout << "corotina destruída" << std::endl;
	}
};

enum class trecho_nome {
	primeiro_nome,
	nome_do_meio,
	ultimo_nome
};

struct promessa {
	std::string primeiro_nome = "";
	std::string nome_do_meio = "";
	std::string ultimo_nome = "";
	trecho_nome onde_estamos = trecho_nome::primeiro_nome; 
	bool terminamos = false;

	~promessa() {
		// apenas o destrutor é chamado (porém o objeto não é desalocado)
		std::cout << std::format("destrutor de promessa chamado (ptr: {:#010x})", reinterpret_cast<uintptr_t>(this)) << std::endl;
	}

	corotina get_return_object() {
		std::cout << "get_return_object" << std::endl;

		// Inicialização implícita de uma corotina, usando essa mesma
		// instância de promessa

		return /* corotina */ { corotina::from_promise(*this) };
	};

	std::suspend_never initial_suspend() noexcept {
		std::cout << "initial_suspend" << std::endl;
		return {};
	};

	std::suspend_never final_suspend() noexcept {
		/*
		 * !!!!!!!!!!!!!!!!!!!! IMPORTANTE
		 * Após o término da corotina (ou em um co_return),
		 * final_suspend() é implicitamente chamado, e LOGO APÓS, o
		 * handle da corotina é destruído.
		 *
		 * A corotina não se encontrará mais em um estado válido.
		 * corotina.done() pode retornar qualquer coisa depois daqui,
		 * inclusive false, e comportamento indefinido ocorrerá se ela
		 * tentar ser resumida.
		 *
		 * Como remediamos isso?
		 * Guardamos dentro do final_suspend
		 */
		auto corotina = get_return_object();
		std::cout << "final_suspend; terminamos? " << std::boolalpha << corotina.done() << std::endl;
		terminamos = true;

		return {};
	};

	void unhandled_exception() {
		std::cout << "unhandled_exception" << std::endl;
	}

	void return_void() {
		std::cout << "return_void" << std::endl;
	}

	std::suspend_always yield_value(std::string val) {
		switch (onde_estamos) {
			case trecho_nome::primeiro_nome:
				primeiro_nome = std::move(val);

				// eu não encontrei como se faz pra acessar a
				// promessa de uma corotina DENTRO de seu corpo
				// de execução, então esse estado pode ser
				// gerenciado aqui mesmo
				onde_estamos = trecho_nome::nome_do_meio;
				break;
			case trecho_nome::nome_do_meio:
				nome_do_meio = std::move(val);

				// mesma coisa aqui
				onde_estamos = trecho_nome::ultimo_nome;
				break;
			case trecho_nome::ultimo_nome:
				ultimo_nome = std::move(val);
				// eu poderia forçar o término da corotina aqui
				// e pela ciência, irei

				// final_suspend();

				// e olha, funciona!
				// porém ela nunca vai chegar no co_return por ela mesma, nesse caso
				// sem contar o inferno de deupração que isso deve trazer

				// NOTA: isso previne que o destrutor da promessa seja chamado
				break;
		}

		return {};
	}
};

corotina coro() {
	auto ler_stdin = []() -> std::string {
		std::string texto;
		std::getline(std::cin, texto);

		return texto;
	};

	std::cout << "Digite seu primeiro nome" << std::endl;
	co_yield ler_stdin();

	std::cout << "Digite seu nome do meio" << std::endl;
	co_yield ler_stdin();

	std::cout << "Digite seu último nome" << std::endl;
	co_yield ler_stdin();

	std::cout << "A corotina terminou" << std::endl;
	co_return;
}

int main() {
	std::cout << "Controle em main()" << std::endl;
	auto obj_corotina = coro();

	while (!obj_corotina.promise().terminamos) {
		std::cout << "Controle em main()" << std::endl;
		obj_corotina.resume();
	}

	std::cout << "Controle em main(); corotina.done()? " << std::boolalpha << obj_corotina.done() << std::endl;

	std::cout << std::format("acessando promessa em {:#010x}", reinterpret_cast<uintptr_t>(&obj_corotina.promise())) << std::endl;
	std::cout << "Controle em main(); corotina terminou? " << std::boolalpha << obj_corotina.promise().terminamos << std::endl;

	auto nome_completo = std::format("{} {} {}",
		obj_corotina.promise().primeiro_nome,
		obj_corotina.promise().nome_do_meio,
		obj_corotina.promise().ultimo_nome
	);

	std::cout << "Seu nome completo é " << nome_completo << std::endl;

	std::cout << "Controle em main()" << std::endl;
}
