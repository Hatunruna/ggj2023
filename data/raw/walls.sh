#!/bin/sh

TMP=$(mktemp -d /tmp/XXXXXXXX)

montage_line () {
  montage "walls/$2*.png" -background none -tile 16x1 -geometry 256x256  png32:$1
}

montage_line "$TMP/walls_00.png" "floor"
montage_line "$TMP/walls_01.png" "wall_col"
montage_line "$TMP/walls_02.png" "wall_row"
montage_line "$TMP/walls_03.png" "wall_corner_left_down"
montage_line "$TMP/walls_04.png" "wall_corner_right_down"
montage_line "$TMP/walls_05.png" "wall_corner_up_left"
montage_line "$TMP/walls_06.png" "wall_corner_up_right"
montage_line "$TMP/walls_07.png" "wall_inter_left"
montage_line "$TMP/walls_08.png" "wall_inter_up"
montage_line "$TMP/walls_09.png" "wall_inter_right"
montage_line "$TMP/walls_10.png" "wall_inter_down"
montage_line "$TMP/walls_11.png" "wall_cross"
montage_line "$TMP/walls_12.png" "door_row"
montage_line "$TMP/walls_13.png" "door_col"

montage "$TMP/*.png" -background none -tile 1x16 -geometry 4096x256  png32:walls.png

rm -rf "$TMP"
