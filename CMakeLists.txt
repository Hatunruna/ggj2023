cmake_minimum_required(VERSION 3.18...3.21)

set(GAME "root-company")

project(${GAME}
  VERSION 0.1.0
)

include(GNUInstallDirs)

find_package(gf REQUIRED COMPONENTS graphics)
find_package(SFML REQUIRED COMPONENTS audio)

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

set(GAME_DATADIR "${CMAKE_INSTALL_PREFIX}/share/games/${GAME}" CACHE PATH "Game Data Dir")
set(GAME_RELATIVE_DATADIR "./share/games/${GAME}" CACHE STRING "Game Relative Data Dir")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(${GAME}
  code/main.cc
  code/bits/AudioManager.cc
  code/bits/CommonScene.cc
  code/bits/DialogEntity.cc
  code/bits/GameHub.cc
  code/bits/GameOverEntity.cc
  code/bits/GameOverScene.cc
  code/bits/GameState.cc
  code/bits/Hero.cc
  code/bits/HeroEntity.cc
  code/bits/HeroHudEntity.cc
  code/bits/LiftEntity.cc
  code/bits/LiftScene.cc
  code/bits/LightEntity.cc
  code/bits/MainScene.cc
  code/bits/MapEntity.cc
  code/bits/MapState.cc
  code/bits/RootEntity.cc
  code/bits/RootModel.cc
  code/bits/SplitEntity.cc
  code/bits/StartScene.cc
  code/bits/TitleEntity.cc
  code/bits/VictoryEntity.cc
  code/bits/VictoryScene.cc
)

target_include_directories(${GAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${GAME}
  PRIVATE
    gf::graphics
    sfml-audio
)

if (WIN32)
  add_custom_command(TARGET ${GAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${GAME}> $<TARGET_FILE_DIR:${GAME}>
    COMMAND_EXPAND_LISTS
  )

  install(
    FILES
      $<TARGET_FILE_DIR:${GAME}>/SDL2d.dll
      $<TARGET_FILE_DIR:${GAME}>/brotlicommon.dll
      $<TARGET_FILE_DIR:${GAME}>/brotlidec.dll
      $<TARGET_FILE_DIR:${GAME}>/bz2d.dll
      $<TARGET_FILE_DIR:${GAME}>/freetyped.dll
      $<TARGET_FILE_DIR:${GAME}>/gf0.dll
      $<TARGET_FILE_DIR:${GAME}>/gfcore0.dll
      $<TARGET_FILE_DIR:${GAME}>/libpng16d.dll
      $<TARGET_FILE_DIR:${GAME}>/pugixml_d.dll
      $<TARGET_FILE_DIR:${GAME}>/zlibd1.dll
      $<TARGET_FILE_DIR:${GAME}>/prises.pdb
    DESTINATION .
    CONFIGURATIONS Debug
  )

  install(
    FILES
      $<TARGET_FILE_DIR:${GAME}>/SDL2.dll
      $<TARGET_FILE_DIR:${GAME}>/brotlicommon.dll
      $<TARGET_FILE_DIR:${GAME}>/brotlidec.dll
      $<TARGET_FILE_DIR:${GAME}>/bz2.dll
      $<TARGET_FILE_DIR:${GAME}>/freetype.dll
      $<TARGET_FILE_DIR:${GAME}>/gf0.dll
      $<TARGET_FILE_DIR:${GAME}>/gfcore0.dll
      $<TARGET_FILE_DIR:${GAME}>/libpng16.dll
      $<TARGET_FILE_DIR:${GAME}>/pugixml.dll
      $<TARGET_FILE_DIR:${GAME}>/zlib1.dll
    DESTINATION .
    CONFIGURATIONS Release
  )

  install(TARGETS ${GAME} DESTINATION .)
else ()
  install(TARGETS ${GAME} RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif ()

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/${GAME}"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

# Package rules
if (WIN32)
  set(CPACK_GENERATOR "ZIP")
  include(CPack)
endif()
