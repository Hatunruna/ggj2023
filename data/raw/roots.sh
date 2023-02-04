#!/bin/sh

TMP=$(mktemp -d /tmp/XXXXXXXX)

montage_line () {
  if [ -z "$3" ]
  then
    montage "roots/$2/*.png" -background none -tile 15x1 -geometry 256x256  png32:$1
  else
    montage "roots/$2/*.png" "roots/$3/*.png" -background none -tile 15x1 -geometry 256x256  png32:$1
  fi
}

montage_line "$TMP/root_0001.png" "bout_racine/gauche"
montage_line "$TMP/root_0010.png" "bout_racine/bas"
montage_line "$TMP/root_0011.png" "L racine/de_bas_a_gauche" "L racine/de_gauche_en_bas"
montage_line "$TMP/root_0100.png" "bout_racine/droite"
montage_line "$TMP/root_0101.png" "_ racine"
montage_line "$TMP/root_0110.png" "L racine/de_bas_a_droite" "L racine/de_droite_en_bas"
montage_line "$TMP/root_0111.png" "T racine/haut"
montage_line "$TMP/root_1000.png" "bout_racine/haut"
montage_line "$TMP/root_1001.png" "L racine/de_haut_a_gauche" "L racine/de_gauche_en_haut"
montage_line "$TMP/root_1010.png" "I racine"
montage_line "$TMP/root_1011.png" "T racine/droite"
montage_line "$TMP/root_1100.png" "L racine/de_haut_a_droite" "L racine/de_droite_en_haut"
montage_line "$TMP/root_1101.png" "T racine/bas"
montage_line "$TMP/root_1110.png" "T racine/gauche"
montage_line "$TMP/root_1111.png" "+racine"

montage "$TMP/*.png" -background none -tile 1x16 -geometry 3840x256  png32:rootsL.png

montage \
  "roots/sortit_/sortit_racine_03.png" \
  "roots/sortit_/sortit_racine_06.png" \
  "roots/sortitL/sortit_racine_15.png" \
  "roots/sortit_/sortit_racine_01.png" \
  "roots/sortit__/sortit_racine_17.png" \
  "roots/sortitL/sortit_racine_16.png" \
  "roots/sortitT/sortit_racine_12.png" \
  "roots/sortit_/sortit_racine_07.png" \
  "roots/sortitL/sortit_racine_14.png" \
  "roots/sortit__/sortit_racine_18.png" \
  "roots/sortitT/sortit_racine_09.png" \
  "roots/sortitL/sortit_racine_13.png" \
  "roots/sortitT/sortit_racine_11.png" \
  "roots/sortitT/sortit_racine_10.png" \
  "roots/sortit+/sortit_racine_19.png" \
  -background none -tile 1x16 -geometry 256x256  png32:rootsR.png

montage -mode concatenate -tile x1 rootsL.png rootsR.png -background none png32:roots.png

rm -rf "$TMP"
