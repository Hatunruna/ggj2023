#include "MapState.h"

#include <gf/Direction.h>
#include <gf/Log.h>
#include <gf/Rect.h>

#include "GameHub.h"
#include "Settings.h"

namespace rc {

  namespace {

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

  std::vector<GeneratedMap> createProceduralMap(gf::Random& random) {
    auto generateLevel = [&random](gf::Vector2i mapSize, std::vector<gf::RectI>& stairs, int levelNumber) -> GeneratedMap {
      gf::RectI mapRect = gf::RectI::fromSize(mapSize);

      std::vector<gf::RectI> oldStairs = stairs;

      std::vector<gf::RectI> corridors;

      if (levelNumber == 0 || random.computeBernoulli(0.5)) {
        // 2 horizontal, 1 vertical

        int v0 = random.computeUniformInteger(Corridor0 - CorridorExtent, Corridor0 + CorridorExtent);
        int h1 = random.computeUniformInteger(Corridor1 - CorridorExtent, Corridor1 + CorridorExtent);
        int h2 = random.computeUniformInteger(Corridor2 - CorridorExtent, Corridor2 + CorridorExtent);

        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(v0, 1), gf::vec(CorridorWidth, mapSize.height - 2)));
        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(1, h1), gf::vec(mapSize.width - 2, CorridorWidth)));
        corridors.push_back(gf::RectI::fromPositionSize(gf::vec(1, h2), gf::vec(mapSize.width - 2, CorridorWidth)));
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
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 2),
          random.computeUniformInteger(Corridor2 + CorridorExtent + 2, MapLength - StairsSize - 2)
        );

        gf::RectI stair1 = gf::RectI::fromPositionSize(position1, gf::vec(StairsSize, StairsSize));

        assert(!hasCollision(stair1) && "even");
        stairs.push_back(stair1);

        auto position2 = gf::vec(
          random.computeUniformInteger(Corridor2 + CorridorExtent + 2, MapLength - StairsSize - 2),
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 2)
        );

        gf::RectI stair2 = gf::RectI::fromPositionSize(position2, gf::vec(StairsSize, StairsSize));
        assert(!hasCollision(stair2) && "even");
        stairs.push_back(stair2);

      } else {
        auto position1 = gf::vec(
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 2),
          random.computeUniformInteger(1, Corridor1 - CorridorExtent - StairsSize - 2)
        );

        gf::RectI stair1 = gf::RectI::fromPositionSize(position1, gf::vec(StairsSize, StairsSize));

        assert(!hasCollision(stair1) && "odd");
        stairs.push_back(stair1);

        auto position2 = gf::vec(
          random.computeUniformInteger(Corridor2 + CorridorExtent + 2, MapLength - StairsSize - 2),
          random.computeUniformInteger(Corridor2 + CorridorExtent + 2, MapLength - StairsSize - 2)
        );

        gf::RectI stair2 = gf::RectI::fromPositionSize(position2, gf::vec(StairsSize, StairsSize));
        assert(!hasCollision(stair2) && "odd");
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

      gf::Array2D<MapCell> level(mapSize, { MapCellType::Void });

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
          // assert(level(current).type == MapCellType::Wall);

          gf::Vector2i start = current - dir;

          while (mapRect.contains(current) && isWall(level(current).type)
            && isWall(level(current + gf::perp(dir)).type)
            && isWall(level(current - gf::perp(dir)).type)
            && isWall(level(current + 2 * gf::perp(dir)).type)
            && isWall(level(current - 2 * gf::perp(dir)).type)
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

      if (levelNumber == 0) {
        // assume that the vertical corridor is the first!
        gf::RectI corridor = corridors[0];

        auto liftL = corridor.getTopLeft() - gf::diry(1);
        auto liftR = liftL + gf::dirx(1);

        level(liftL).type = MapCellType::LiftL;
        level(liftR).type = MapCellType::LiftR;
      }

      for (auto & stair : oldStairs) {
        level(stair.getCenter()).type = MapCellType::StairDown;
      }

      for (auto & stair : stairs) {
        level(stair.getCenter()).type = MapCellType::StairUp;
      }

      // Add wall
      for (const auto& cellPosition: level.getPositionRange()) {
        const MapCell& cell = level(cellPosition);

        if (cell.type == MapCellType::Floor) {
          for (const auto& neighborsCellPosition: level.get8NeighborsRange(cellPosition)) {
            MapCell& neighborsCell = level(neighborsCellPosition);
            if (neighborsCell.type == MapCellType::Void) {
              neighborsCell.type = MapCellType::Wall;
            }
          }
        }
      }

      // Add open door at room entries
      for (const auto& room: rooms) {
        // Horizontal doors
        for (int col = room.min.x - 1; col < room.max.x + 1; ++col) {
          for (int row: { room.min.y - 1, room.max.y }) {
            MapCell& cell = level(gf::vec(col, row));

            if (cell.type == MapCellType::Floor) {
              cell.type = MapCellType::Door;
              cell.doorState.isOpen = true;
            }
          }
        }

        // Vertical doors
        for (int col: { room.min.x - 1, room.max.x }) {
          for (int row = room.min.y - 1; row < room.max.y + 1; ++row) {
            MapCell& cell = level(gf::vec(col, row));

            if (cell.type == MapCellType::Floor) {
              cell.type = MapCellType::Door;
              cell.doorState.isOpen = true;
            }
          }
        }
      }

      return { level, rooms };
    };

    std::vector<GeneratedMap> levels;
    std::vector<gf::RectI> stairs;

    for (int i = 0; i < 10; ++i) {
      auto level = generateLevel(MapSize, stairs, i);
      levels.emplace_back(std::move(level));
    }

    return levels;
  }

  std::tuple<gf::Vector2i, gf::Vector2i> computeStartingPositions(const gf::Array2D<MapCell>& cells) {
    gf::Vector2i lisa = gf::vec(0, 0);
    gf::Vector2i ryan = gf::vec(0, 0);

    for (auto position : cells.getPositionRange()) {
      if (cells(position).type == MapCellType::LiftL) {
        lisa = position + gf::diry(1);
      } else if (cells(position).type == MapCellType::LiftR) {
        ryan = position + gf::diry(1);
      }
    }

    assert(lisa != gf::vec(0, 0));
    assert(ryan != gf::vec(0, 0));
    return std::make_tuple(lisa, ryan);
  }

  gf::SquareMap computeMap(const gf::Array2D<MapCell>& cells) {
    const gf::Vector2i mapSize = cells.getSize();
    gf::SquareMap map(mapSize);

    for (int col = 0; col < mapSize.width; ++col) {
      for (int row = 0; row < mapSize.height; ++row) {
        const MapCell& cell = cells(gf::vec(col, row));
        switch (cell.type) {
        case MapCellType::Floor:
        case MapCellType::StairDown:
        case MapCellType::StairUp:
          map.setEmpty(gf::vec(col, row));
          break;

        case MapCellType::Door:
          if (cell.doorState.isOpen) {
            map.setEmpty(gf::vec(col, row));
          }

        default:
          break;
        }

      }
    }

    return map;
  }

  enum class CellStatus : char {
    Unexplored,
    Visited,
  };

  std::vector<gf::Vector2i> computeMultiPath(const gf::SquareMap& map, std::vector<gf::Vector2i> starts, gf::Random& random) {
    std::vector<gf::Vector2i> path = starts;
    std::vector<gf::Vector2i> queue = starts;;

    gf::Array2D<CellStatus> status(map.getSize(), CellStatus::Unexplored);

    for (auto position : starts) {
      status(position) = CellStatus::Visited;
    }

    while (!queue.empty()) {
      std::shuffle(queue.begin(), queue.end(), random.getEngine());

      gf::Vector2i current = queue.back();
      queue.pop_back();
      assert(status(current) == CellStatus::Visited);

      for (auto direction : { gf::Direction::Up, gf::Direction::Left, gf::Direction::Down, gf::Direction::Right }) {
        gf::Vector2i next = current + gf::displacement(direction);

        if (map.isWalkable(next) && status(next) == CellStatus::Unexplored) {
          status(next) = CellStatus::Visited;
          queue.push_back(next);
        }
      }
    }

    return path;
  }

  MapLevel::MapLevel(const GeneratedMap& generated, gf::Random& random)
  : level(generated)
  , map(computeMap(generated.cells))
  {
    std::vector<gf::Vector2i> stairs;
    std::vector<gf::Vector2i> starts;

    for (auto position : generated.cells.getPositionRange()) {
      if (generated.cells(position).type == MapCellType::StairDown) {
        stairs.push_back(position);
      } else if (generated.cells(position).type == MapCellType::LiftL) {
        starts.push_back(position + gf::diry(1));
      } else if (generated.cells(position).type == MapCellType::LiftR) {
        starts.push_back(position + gf::diry(1));
      }
    }

    if (stairs.size() == 2) {
      starts = map.computeRoute(stairs[0], stairs[1], 0.0);

      if (starts.empty()) {
        starts = stairs;
      }
    }

    assert(!starts.empty());
    auto path = computeMultiPath(map, starts, random);
    computeDoorsAndComputers(path);
  }

  void MapLevel::computeDoorsAndComputers(const std::vector<gf::Vector2i>& path) {
    // TODO
  }

}
