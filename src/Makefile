
OBJECT=../index.*
CXX=emcc

ALL:
	$(CXX) main.cpp -Wall -Wextra -O3 -DNDEBUG      \
                    -std=c++17                      \
                    -s WASM=1                       \
                    -s DISABLE_EXCEPTION_CATCHING=0 \
                    -s USE_SDL=2                    \
                    -s USE_SDL_NET=2                \
                    -s USE_SDL_IMAGE=2              \
                    -s USE_SDL_TTF=2                \
                    --preload-file ../asset         \
                    -s SDL2_IMAGE_FORMATS='["png"]' \
                    --emrun                         \
                    -o ../index.html

.PHONY: run clean
run:
	sh -c 'cd ..; emrun --port 7000 --no_browser .'

.PHONY: debug_compile
debug_compile:
	$(CXX) main.cpp -Wall -Wextra                   \
                    -std=c++17                      \
                    -s WASM=1                       \
                    -s DISABLE_EXCEPTION_CATCHING=0 \
                    -s USE_SDL=2                    \
                    -s USE_SDL_NET=2                \
                    -s USE_SDL_IMAGE=2              \
                    -s USE_SDL_TTF=2                \
                    -s ASSERTIONS=1                 \
                    -s SDL2_IMAGE_FORMATS='["png"]' \
                    --preload-file ../asset         \
                    --emrun                         \
                    -o ../index.html

.PHONY: debug
debug: debug_compile run

clean:
	rm -rf $(OBJECT)
