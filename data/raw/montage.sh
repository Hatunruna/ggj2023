#!/bin/bash

DIR=$1
TILESET=$2

montage $DIR/*.png -background none -tile 4x2 -geometry 256x256  png32:${TILESET}
