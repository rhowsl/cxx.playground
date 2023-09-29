all:
	rm -rfv ./build/
	mkdir -p build
	mkdir -p build/obj
	g++ -c -std=gnu++2b sequenciador.cc -o build/obj/sequenciador.o
	g++ -c -std=gnu++2b ffi.cc -o build/obj/ffi.o
	g++ -c -std=gnu++2b main.cc -o build/obj/main.o
	g++ build/obj/sequenciador.o build/obj/ffi.o -shared -s -o build/seq_ffi.so
	g++ build/obj/sequenciador.o build/obj/main.o -o build/seq
