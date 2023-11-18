#include <vector>
#include <iostream>
#include <initializer_list>
#include <string_view>
#include <cstdint>
#include <type_traits>
#include <ranges>
#include <unordered_map>
    
using program_arguments = const std::vector<std::string_view>;
using environment_variable = const std::pair<const std::string_view, const std::string_view>;
using environment_variables = const std::unordered_map<std::string_view, std::string_view>;

int bmain(
    program_arguments args = {}, 
    environment_variables env = {}
) {
	// your main here
	
    std::cout << "=== Arguments\n";
    for (auto arg : args) {
        std::cout << "\t" << arg << std::endl;
    }

    std::cout << "=== Environment Variables\n";
    for (auto [k, v] : env) {
        std::cout << "\t" << k << ": " << v << std::endl;
    }

    return 0;
}


int main(int argc, char* argv[], char* envp[]) {
	extern char** environ;
    return bmain(
		// pointer arithmetic moment
        program_arguments { argv + 1, argv + argc },
		// this is the best hashmap initialization iife i've ever written
		([](char** envp) -> environment_variables {
			 std::remove_cv_t<environment_variables> out;

			 if (envp == nullptr) {
				 // no environment vars, we leave
				 // our env hashmap is empty anyway
				 return out;
			 }

			 while (*envp) {
				 std::string_view strv { *envp++ };
				 auto split = strv 
					 | std::views::split('=') 
					 | std::views::take(2)
					 | std::views::pairwise_transform([&strv](auto a, auto b) -> environment_variable {
						 return { std::string_view(a), std::string_view(b.cbegin(), strv.cend()) };
					 });

				 for (auto [k, v] : split) {
					 out.emplace(k, v);
				 }
			 }

			 return out;
		 })(envp ? envp : (environ ? environ : nullptr))
	);
}
