#include "MapState.h"

#include <stack>

#include <gf/Log.h>
#include <gf/Rect.h>

namespace xy {

  namespace {
    constexpr int SpaceWidth = 21;
    constexpr int CorridorWidth = 2;

    constexpr int MapLength = SpaceWidth * 3 + CorridorWidth * 2 + 2;
    constexpr gf::Vector2i MapSize = gf::vec(MapLength, MapLength);

    constexpr int Corridor0 = MapLength / 2;
    constexpr int Corridor1 = 1 + SpaceWidth;
    constexpr int Corridor2 = Corridor1 + CorridorWidth + SpaceWidth;
    constexpr int CorridorExtent = 4;

    constexpr int StairsSize = 3;

    constexpr int RoomsCount = 16;
    constexpr int MaxRetries = 100;

    gf::Vector2i computeRoomSize(gf::Random& random) {
      switch (random.computeUniformInteger(0, 9)) {
        case 0:
          return gf::vec(12, 16);
        case 1:
          return gf::vec(16, 12);
        case 2:
          return gf::vec(9, 12);
        case 3:
          return gf::vec(12, 9);
        case 4:
        case 5:
          return gf::vec(10, 10);
        case 6:
        case 7:
          return gf::vec(14, 14);
        case 8:
          return gf::vec(6, 8);
        default:
          return gf::vec(8, 6);
      }

      return gf::vec(6, 8);
    }

  }

  std::vector<gf::Array2D<MapCell>> createProceduralMap(gf::Random& random) {
    auto generateLevel = [&random](gf::Vector2i mapSize, std::vector<gf::RectI>& stairs, int levelNumber) -> gf::Array2D<MapCell> {
      gf::RectI mapRect = gf::RectI::fromSize(mapSize);

      std::vector<gf::RectI> oldStairs = stairs;

      std::vector<gf::RectI> corridors;

      if (random.computeBernoulli(0.5)) {
        // 2 horizontal, 1 vertical

        int h1 = random.computeUniformInteger(Corridor1 - CorridorExtent, Corridor1 + CorridorExtent);
        int h2 = random.computeUniformInteger(Corridor2 - CorridorExtent, Corridor2 + CorridorExtent);
        int v0 = random.computeUniformInteger(Corridor0 - CorridorExtent, Corridor0 + CorridorExtent);

        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(1, h1), gf::vec(mapSize.width - 2, CorridorWidth)));
        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(1, h2), gf::vec(mapSize.width - 2, CorridorWidth)));
        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(v0, 1), gf::vec(CorridorWidth, mapSize.height - 2)));
      } else {
        // 1 horizontal, 2 vertical

        int v1 = random.computeUniformInteger(Corridor1 - CorridorExtent, Corridor1 + CorridorExtent);
        int v2 = random.computeUniformInteger(Corridor2 - CorridorExtent, Corridor2 + CorridorExtent);
        int h0 = random.computeUniformInteger(Corridor0 - CorridorExtent, Corridor0 + CorridorExtent);

        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(v1, 1), gf::vec(CorridorWidth, mapSize.height - 2)));
        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(v2, 1), gf::vec(CorridorWidth, mapSize.height - 2)));
        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(1, h0), gf::vec(mapSize.width - 2, CorridorWidth)));
      }

      std::vector<gf::RectI> rooms = stairs;

      auto hasCollision = [&rooms, &corridors](const gf::RectI& room) {
        return std::any_of(corridors.begin(), corridors.end(), [&room](const gf::RectI& other) { return room.intersects(other.grow(1)); })
            || std::any_of(rooms.begin(), rooms.end(), [&room](const gf::RectI& other) { return room.intersects(other.grow(1)); });
      };

      stairs.clear();

      if (levelNumber % 2 == 0) {
        auto position1 = gf::vec(
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 1),
          random.computeUniformInteger(Corridor2 + CorridorExtent + 1, MapLength - StairsSize - 2)
        );

        gf::RectI stair1 = gf::RectI::fromPositionSize(position1, gf::vec(StairsSize, StairsSize));

        assert(!hasCollision(stair1));
        stairs.push_back(stair1);

        auto position2 = gf::vec(
          random.computeUniformInteger(Corridor2 + CorridorExtent + 1, MapLength - StairsSize - 2),
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 1)
        );

        gf::RectI stair2 = gf::RectI::fromPositionSize(position2, gf::vec(StairsSize, StairsSize));
        assert(!hasCollision(stair2));
        stairs.push_back(stair2);

      } else {
        auto position1 = gf::vec(
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 1),
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 1)
        );

        gf::RectI stair1 = gf::RectI::fromPositionSize(position1, gf::vec(StairsSize, StairsSize));

        assert(!hasCollision(stair1));
        stairs.push_back(stair1);

        auto position2 = gf::vec(
          random.computeUniformInteger(Corridor2 + CorridorExtent + 1, MapLength - StairsSize - 2),
          random.computeUniformInteger(Corridor2 + CorridorExtent + 1, MapLength - StairsSize - 2)
        );

        gf::RectI stair2 = gf::RectI::fromPositionSize(position2, gf::vec(StairsSize, StairsSize));
        assert(!hasCollision(stair2));
        stairs.push_back(stair2);

      }

      rooms.insert(rooms.end(), stairs.begin(), stairs.end());

      for (int i = 0; i < RoomsCount; ++i) {
        gf::RectI room;
        int retries = 0;

        do {
          auto size = computeRoomSize(random);

          auto position = gf::vec(
            random.computeUniformInteger(1, mapSize.width - size.width - 2),
            random.computeUniformInteger(1, mapSize.height - size.height - 2)
          );

          room = gf::RectI::fromPositionSize(position, size);
          ++retries;
        } while (hasCollision(room) && retries < MaxRetries);

        if (retries < MaxRetries) {
          rooms.push_back(room);
        }
      }

      gf::Array2D<MapCell> level(mapSize, { MapCellType::Wall });

      auto createSpace = [&level](gf::RectI space) {
        for (int x = space.min.x; x < space.max.x; ++x) {
          for (int y = space.min.y; y < space.max.y; ++y) {
            level(gf::vec(x, y)).type = MapCellType::Floor;
          }
        }
      };

      for (auto & space : corridors) {
        createSpace(space);
      }

      for (auto & space : rooms) {
        createSpace(space);
      }

      // Create corridor between rooms

      for (const auto & room : rooms) {

        for (gf::Vector2i dir : { gf::vec(0, 1), gf::vec(0, -1), gf::vec(1, 0), gf::vec(-1, 0) }) {

          auto current = room.getCenter();
          assert(level(current).type == MapCellType::Floor);

          current += dir;

          while (level(current).type == MapCellType::Floor) {
            current += dir;
          }

          assert(mapRect.contains(current));
          assert(level(current).type == MapCellType::Wall);

          gf::Vector2i start = current - dir;

          while (mapRect.contains(current) && level(current).type == MapCellType::Wall
            && level(current + gf::perp(dir)).type == MapCellType::Wall
            && level(current - gf::perp(dir)).type == MapCellType::Wall
            && level(current + 2 * gf::perp(dir)).type == MapCellType::Wall
            && level(current - 2 * gf::perp(dir)).type == MapCellType::Wall
          ) {
            current += dir;
          }

          if (mapRect.contains(current) && level(current).type == MapCellType::Floor) {
            gf::Vector2i stop = current + gf::abs(gf::perp(dir));
            gf::RectI corridor = gf::RectI::fromMinMax(gf::min(start, stop), gf::max(start, stop));

            corridors.push_back(corridor);
            createSpace(corridor);
          }
        }

      }

      for (auto & stair : oldStairs) {
        level(stair.getCenter()).type = MapCellType::StairDown;
      }

      for (auto & stair : stairs) {
        level(stair.getCenter()).type = MapCellType::StairUp;
      }

      return level;
    };

    std::vector<gf::Array2D<MapCell>> levels;
    std::vector<gf::RectI> stairs;

    for (int i = 0; i < 10; ++i) {
      auto level = generateLevel(MapSize, stairs, i);
      levels.emplace_back(std::move(level));
    }

    return levels;
  }

  MapLevel::MapLevel(const gf::Array2D<MapCell>& generated)
  : map(generated.getSize())
  , cells(generated)
  {
    const gf::Vector2i mapSize = generated.getSize();

    for (int col = 0; col < mapSize.width; ++col) {
      for (int row = 0; row < mapSize.height; ++row) {
        const MapCell& cell = cells(gf::vec(col, row));
        switch (cell.type) {
        case MapCellType::Floor:
        case MapCellType::StairDown:
        case MapCellType::StairUp:
          map.setEmpty(gf::vec(col, row));
          break;

        default:
          break;
        }

        // DEBUG
        map.setEmpty(gf::vec(col, row));

      }
    }

    enum class CellStatus : char {
      Unexplored,
      Visited,
    };

    gf::Array2D<CellStatus> status(mapSize, CellStatus::Unexplored);
    std::stack<gf::Vector2i> stack;
    std::vector<gf::Vector2i> path;



  }

}
