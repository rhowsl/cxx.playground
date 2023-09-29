#include <optional>
#include <array>
#include <utility>

enum class tipo_comando {
	note_on,
	note_off
};

typedef std::pair<tipo_comando, std::optional<int>> comando;

constexpr int NO_COLUNAS = 5;
struct coluna {
	std::optional<int> nota_atual = 0;
	int volume = 0;
};

struct linha {
	std::array<comando, NO_COLUNAS> comandos;
};
