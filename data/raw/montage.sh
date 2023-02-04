#!/bin/bash

DIR=$1
TILESET=$2

rm -f "${FILE}"
montage $DIR/*.png -background none -tile 4x2 -geometry 256x256  png32:${TILESET}
