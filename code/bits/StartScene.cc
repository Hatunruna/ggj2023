#include "StartScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace xy {

  namespace {

    constexpr gf::Vector2i LayerLayout = gf::vec(16, 16);

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

      m_game.state.lisa.map.levels.clear();
      m_game.state.ryan.map.levels.clear();

      bool first = true;

      for (int i = 0; i < levels.size(); ++i) {
        auto & level = levels[i];

        if (first) {
          first = false;

          auto [ lisa, ryan ] = computeStartingPositions(level);
          m_game.state.lisa.hero.position = m_game.state.lisa.hero.target = lisa;
          m_game.state.ryan.hero.position = m_game.state.ryan.hero.target = ryan;

          auto mapLevel = MapLevel(level, m_game.random);
          m_game.state.lisa.map.levels.push_back(mapLevel);
          m_game.state.ryan.map.levels.push_back(std::move(mapLevel));

          RootState root;
          root.tail = root.head = ryan + gf::diry(5);
          root.parts.push_back(root.tail);

          m_game.state.roots.push_back(root);

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

                // Set line walls (row & col)
                if (cellLeft == MapCellType::Wall && cellRight == MapCellType::Wall && cellUp != MapCellType::Wall && cellDown != MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallRow));
                }
                else if (cellLeft != MapCellType::Wall && cellRight != MapCellType::Wall && cellUp == MapCellType::Wall && cellDown == MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallColumn));
                }
                // Set corner walls
                else if (cellLeft == MapCellType::Wall && cellRight != MapCellType::Wall && cellUp != MapCellType::Wall && cellDown == MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerLeftDown));
                }
                else if (cellLeft == MapCellType::Wall && cellRight != MapCellType::Wall && cellUp == MapCellType::Wall && cellDown != MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerLeftUp));
                }
                else if (cellLeft != MapCellType::Wall && cellRight == MapCellType::Wall && cellUp != MapCellType::Wall && cellDown == MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerRightDown));
                }
                else if (cellLeft != MapCellType::Wall && cellRight == MapCellType::Wall && cellUp == MapCellType::Wall && cellDown != MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCornerRightUp));
                }
                // Set intersection (T shape)
                else if (cellLeft != MapCellType::Wall && cellRight == MapCellType::Wall && cellUp == MapCellType::Wall && cellDown == MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterRight));
                }
                else if (cellLeft == MapCellType::Wall && cellRight != MapCellType::Wall && cellUp == MapCellType::Wall && cellDown == MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterLeft));
                }
                else if (cellLeft == MapCellType::Wall && cellRight == MapCellType::Wall && cellUp != MapCellType::Wall && cellDown == MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterDown));
                }
                else if (cellLeft == MapCellType::Wall && cellRight == MapCellType::Wall && cellUp == MapCellType::Wall && cellDown != MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallInterUp));
                }
                // Set intersection (Cross shape)
                else if (cellLeft == MapCellType::Wall && cellRight == MapCellType::Wall && cellUp == MapCellType::Wall && cellDown == MapCellType::Wall) {
                  tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallCross));
                }
                else {
                  // Handle doors
                  // TODO: remove this
                  // if (cellRight == MapCellType::Wall || cellLeft == MapCellType::Wall) {
                  //   tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallRow));
                  // } else if (cellUp == MapCellType::Wall || cellDown == MapCellType::Wall) {
                  //   tileLayer.setTile(cellPostion, tileSetId, static_cast<int>(TileType::WallColumn));
                  // } else {
                  //   assert(false);
                  // }
                }

                break;
              }

              default:
                break;
            }
          }

          return std::move(tileLayer);
        };

        m_game.state.lisa.map.layers.emplace_back(std::move(createLevelTileLayer(level, Hero::Lisa)));
        m_game.state.ryan.map.layers.emplace_back(std::move(createLevelTileLayer(level, Hero::Ryan)));
      }

      // TODO: set position from generated map

      m_game.replaceScene(m_game.main); //, m_game.blackout, gf::seconds(TransitionDelay));
    }
  }

}
