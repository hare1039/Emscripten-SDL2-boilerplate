
OBJECT=index.* build
EMPORT=7000

.PHONY: emrun emrun-debug
em:
	cd src &&\
	make

em-debug:
	cd src &&\
	make debug

emrun: em
	emrun --port $(EMPORT) --no_browser .

emrun-debug: em-debug
	emrun --port $(EMPORT) --no_browser .


# cmake part

.PHONY: cmakerun cmakerun-debug
cmake:
	mkdir -p build &&\
	cd build       &&\
	cmake .. -DCMAKE_BUILD_TYPE=Release &&\
	cmake --build .

cmake-debug:
	mkdir -p build &&\
	cd build       &&\
	cmake .. -DCMAKE_BUILD_TYPE=Debug &&\
	cmake --build .

cmakerun: cmake
	./build/src/pikachu

cmakerun-debug: cmake-debug
	./build/src/pikachu

clean:
	rm -rf $(OBJECT)
