#!/bin/bash

GAME_ROOT=$(git rev-parse --show-toplevel)

cd ${GAME_ROOT}
git checkout gh-pages
git checkout master -- src/ asset/
cd src
make clean
make
cd ..
echo "Now, please add and commit to your gh-pages"
