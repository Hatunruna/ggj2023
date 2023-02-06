#include "StartScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace rc {

  namespace {

    constexpr gf::Vector2i LayerLayout = gf::vec(16, 17);

    enum class TileType {
      Floor                 = 0,
      WallColumn            = LayerLayout.x *  1 + 0,
      WallRow               = LayerLayout.x *  2 + 0,
      WallCornerLeftDown    = LayerLayout.x *  3 + 0,
      WallCornerRightDown   = LayerLayout.x *  4 + 0,
      WallCornerLeftUp      = LayerLayout.x *  5 + 0,
      WallCornerRightUp     = LayerLayout.x *  6 + 0,
      WallInterLeft         = LayerLayout.x *  7 + 0,
      WallInterUp           = LayerLayout.x *  8 + 0,
      WallInterRight        = LayerLayout.x *  9 + 0,
      WallInterDown         = LayerLayout.x * 10 + 0,
      WallCross             = LayerLayout.x * 11 + 0,
      DoorRowClosed         = LayerLayout.x * 12 + 0,
      DoorRowOpened         = LayerLayout.x * 12 + 1,
      DoorColumnClosed      = LayerLayout.x * 13 + 0,
      DoorColumnOpened      = LayerLayout.x * 13 + 1,
      LiftLeft              = LayerLayout.x * 14 + 0,
      LiftRight             = LayerLayout.x * 14 + 1,
      StairsDown            = LayerLayout.x * 15 + 0,
      StairsUp              = LayerLayout.x * 15 + 1,
      Computer              = LayerLayout.x * 16 + 0,
    };

  }

  StartScene::StartScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_startAction("Start")
  , m_titleEntity(game.resources)
  {
    setClearColor(gf::Color::Black);

    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    m_startAction.addScancodeKeyControl(gf::Scancode::Space);
    addAction(m_startAction);

    addHudEntity(m_titleEntity);
  }

  void StartScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_startAction.isActive()) {
      auto levels = createProceduralMap(m_game.random);

      m_game.state.roots.clear();
      m_game.state.lisa.map.levels.clear();
      m_game.state.lisa.map.layers.clear();
      m_game.state.ryan.map.levels.clear();
      m_game.state.ryan.map.layers.clear();

      bool first = true;

      for (int i = 0; i < levels.size(); ++i) {
        auto & level = levels[i];

        if (first) {
          first = false;

          // determine the position of Lisa and Ryan

          auto [ lisa, ryan ] = computeStartingPositions(level.cells);
          m_game.state.lisa.hero.position = m_game.state.lisa.hero.target = lisa;
          m_game.state.ryan.hero.position = m_game.state.ryan.hero.target = ryan;

          // determine the position of the root

          gf::Vector2i rootStart;

          do {
            rootStart.x = m_game.random.computeUniformInteger(0, MapLength);
            rootStart.y = m_game.random.computeUniformInteger(0, MapLength);
          } while (level.cells(rootStart).type != MapCellType::Floor || gf::manhattanDistance(rootStart, lisa) < MapLength || gf::manhattanDistance(rootStart, ryan) < MapLength);

          RootState root;
          root.tail = root.head = rootStart;
          root.parts.push_back({ rootStart, gf::vec(0, 15) });

          m_game.state.roots.push_back(root);

          // create the map level

          auto mapLevel = MapLevel(level, m_game.random);
          mapLevel.map.setWalkable(rootStart, false);
          m_game.state.lisa.map.levels.push_back(mapLevel);
          m_game.state.ryan.map.levels.push_back(std::move(mapLevel));



        } else {
          auto mapLevel = MapLevel(level, m_game.random);
          m_game.state.lisa.map.levels.push_back(mapLevel);
          m_game.state.ryan.map.levels.push_back(std::move(mapLevel));
        }

        auto createLevelTileLayer = [this](const gf::Array2D<MapCell>& cells, Hero hero) -> gf::TileLayer {
          gf::TileLayer tileLayer = gf::TileLayer::createOrthogonal(MapSize, CellSize);
          PlayerState& playerState = (hero == Hero::Lisa) ? m_game.state.lisa : m_game.state.ryan;
          std::size_t tileSetId = tileLayer.createTilesetId();
          gf::Tileset& tileset = tileLayer.getTileset(tileSetId);
          tileset.setTexture(m_game.resources.getTexture("images/layer.png"));
          tileset.setSmooth();
          tileset.setTileSize(SpriteSize - 2);
          tileset.setSpacing(gf::vec(2, 2));
          tileset.setMargin(gf::vec(1, 1));

          for (const auto& cellPostion: cells.getPositionRange()) {
            const auto& cell = cells(cellPostion);
            switch (cell.type) {
              case MapCellType::Floor:
                tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::Floor));
                break;

              case MapCellType::Wall:
              {
                const MapCellType cellLeft   = (cellPostion.x > 0)                   ? cells(cellPostion + gf::vec(-1,  0)).type : MapCellType::Void;
                const MapCellType cellRight  = (cellPostion.x < MapSize.width - 1)   ? cells(cellPostion + gf::vec( 1,  0)).type : MapCellType::Void;
                const MapCellType cellUp     = (cellPostion.y > 0)                   ? cells(cellPostion + gf::vec( 0, -1)).type : MapCellType::Void;
                const MapCellType cellDown   = (cellPostion.y < MapSize.height - 1)  ? cells(cellPostion + gf::vec( 0,  1)).type : MapCellType::Void;

                bool cellLeftIsWall =
                  cellLeft == MapCellType::Wall
                  || cellLeft == MapCellType::Door
                  || cellLeft == MapCellType::LiftL
                  || cellLeft == MapCellType::LiftR;
                bool cellRightIsWall =
                  cellRight == MapCellType::Wall
                  || cellRight == MapCellType::Door
                  || cellRight == MapCellType::LiftL
                  || cellRight == MapCellType::LiftR;
                bool cellUpIsWall = cellUp == MapCellType::Wall || cellUp == MapCellType::Door;
                bool cellDownIsWall = cellDown == MapCellType::Wall || cellDown == MapCellType::Door;

                // Set line walls (row & col)
                if (cellLeftIsWall && cellRightIsWall && !cellUpIsWall && !cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallRow));
                }
                else if (!cellLeftIsWall && !cellRightIsWall && cellUpIsWall && cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallColumn));
                }
                // Set corner walls
                else if (cellLeftIsWall && !cellRightIsWall && !cellUpIsWall && cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerLeftDown));
                }
                else if (cellLeftIsWall && !cellRightIsWall && cellUpIsWall && !cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerLeftUp));
                }
                else if (!cellLeftIsWall && cellRightIsWall && !cellUpIsWall && cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerRightDown));
                }
                else if (!cellLeftIsWall && cellRightIsWall && cellUpIsWall && !cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerRightUp));
                }
                // Set intersection (T shape)
                else if (!cellLeftIsWall && cellRightIsWall && cellUpIsWall && cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterRight));
                }
                else if (cellLeftIsWall && !cellRightIsWall && cellUpIsWall && cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterLeft));
                }
                else if (cellLeftIsWall && cellRightIsWall && !cellUpIsWall && cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterDown));
                }
                else if (cellLeftIsWall && cellRightIsWall && cellUpIsWall && !cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterUp));
                }
                // Set intersection (Cross shape)
                else if (cellLeftIsWall && cellRightIsWall && cellUpIsWall && cellDownIsWall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCross));
                }
                else {
                  assert(false);
                }

                break;
              }

              case MapCellType::Door:
              {
                const MapCellType cellLeft   = (cellPostion.x > 0)                   ? cells(cellPostion + gf::vec(-1,  0)).type : MapCellType::Void;
                const MapCellType cellRight  = (cellPostion.x < MapSize.width - 1)   ? cells(cellPostion + gf::vec( 1,  0)).type : MapCellType::Void;
                const MapCellType cellUp     = (cellPostion.y > 0)                   ? cells(cellPostion + gf::vec( 0, -1)).type : MapCellType::Void;
                const MapCellType cellDown   = (cellPostion.y < MapSize.height - 1)  ? cells(cellPostion + gf::vec( 0,  1)).type : MapCellType::Void;

                bool cellLeftIsWall = cellLeft == MapCellType::Wall || cellLeft == MapCellType::Door;
                bool cellRightIsWall = cellRight == MapCellType::Wall || cellRight == MapCellType::Door;
                bool cellUpIsWall = cellUp == MapCellType::Wall || cellUp == MapCellType::Door;
                bool cellDownIsWall = cellDown == MapCellType::Wall || cellDown == MapCellType::Door;

                bool cellLeftIsNotWall = cellLeft != MapCellType::Wall;
                bool cellRightIsNotWall = cellRight != MapCellType::Wall;
                bool cellUpIsNotWall = cellUp != MapCellType::Wall;
                bool cellDownIsNotWall = cellDown != MapCellType::Wall;

                if (cellLeftIsWall && cellRightIsWall && cellUpIsNotWall && cellDownIsNotWall) {
                  if (cell.doorState.isOpen) {
                    tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::DoorRowOpened));
                  } else {
                    tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::DoorRowClosed));
                  }
                }
                else if (cellLeftIsNotWall && cellRightIsNotWall && cellUpIsWall && cellDownIsWall) {
                  if (cell.doorState.isOpen) {
                    tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::DoorColumnOpened));
                  } else {
                    tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::DoorColumnClosed));
                  }
                }
                else {
                  assert(false);
                }

                break;
              }

              case MapCellType::LiftL:
                tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::LiftLeft));
                break;

              case MapCellType::LiftR:
                tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::LiftRight));
                break;

              case MapCellType::StairDown:
                tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::StairsDown));
                break;

              case MapCellType::StairUp:
                tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::StairsUp));
                break;

              case MapCellType::Computer:
                tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::Computer));
                break;

              default:
                break;
            }
          }

          return tileLayer;
        };

        m_game.state.lisa.map.layers.push_back(createLevelTileLayer(m_game.state.lisa.map.levels.back().level.cells, Hero::Lisa));
        m_game.state.ryan.map.layers.push_back(createLevelTileLayer(m_game.state.ryan.map.levels.back().level.cells, Hero::Ryan));
      }

      m_game.state.status = GameStatus::Playing;
      m_game.replaceScene(m_game.lift, m_game.blackout, gf::seconds(1.0f));
    }
  }

}
