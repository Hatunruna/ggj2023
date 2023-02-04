#include "StartScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace xy {

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
          tileset.setTexture(m_game.resources.getTexture("images/floor.png"));
          tileset.setSmooth();
          tileset.setTileSize(SpriteSize);

          for (const auto& cellPostion: cells.getPositionRange()) {
            const auto& cell = cells(cellPostion);
            switch (cell.type) {
              case MapCellType::Floor:
                tileLayer.setTile(cellPostion, tileSetId, 0);
                break;

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
