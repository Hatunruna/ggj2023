cmake_minimum_required(VERSION 3.10)

set(GAME Game)

project(${GAME}
  VERSION 0.1.0
)

include(GNUInstallDirs)

find_package(gf REQUIRED COMPONENTS graphics)

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

set(GAME_DATADIR "${CMAKE_INSTALL_PREFIX}/share/games/${GAME}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(${GAME}
  code/main.cc
  code/bits/GameData.cc
  code/bits/GameHub.cc
  code/bits/GameState.cc
  code/bits/MainScene.cc
  code/bits/StartScene.cc
  code/bits/TitleEntity.cc
)

target_include_directories(${GAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${GAME}
  PRIVATE
    gf::graphics
)

if (WIN32)
  install(
    FILES
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/SDL2d.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/brotlicommon.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/brotlidec.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/bz2d.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/freetyped.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/gf0.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/gfcore0.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/libpng16d.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/pugixml_d.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/zlibd1.dll>
      $<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/Debug/prises.pdb>

      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/SDL2.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/brotlicommon.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/brotlidec.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/bz2.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/freetype.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/gf0.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/gfcore0.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/libpng16.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/pugixml.dll>
      $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/zlib1.dll>
    DESTINATION .
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
