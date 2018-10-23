
OBJECT=../index.*
CXX=emcc

ALL:
	$(CXX) main.cpp -Wall -Wextra -O2               \
                    -std=c++17                      \
                    -s WASM=1                       \
                    -s DISABLE_EXCEPTION_CATCHING=0 \
                    -s USE_SDL=2                    \
                    -s USE_SDL_NET=2                \
                    -s USE_SDL_IMAGE=2              \
                    --preload-file ../asset         \
                    -s SDL2_IMAGE_FORMATS='["png"]' \
                    --emrun                         \
                    -o ../index.html

.PHONY: run clean
run: ALL
	sh -c 'cd ..; emrun --port 7000 --no_browser .'

clean:
	rm -rf $(OBJECT)
