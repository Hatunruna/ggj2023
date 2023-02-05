#ifndef RC_ROOT_MODEL_H
#define RC_ROOT_MODEL_H

#include <optional>
#include <tuple>

#include <gf/Model.h>
#include <gf/Random.h>
#include <gf/Vector.h>

#include "Hero.h"

namespace rc {
  struct GameState;
  struct RootState;

  class RootModel : public gf::Model {
  public:
    RootModel(GameState& state, gf::Random& random);
    void update(gf::Time time) override;

  private:
    std::optional<std::tuple<gf::Vector2i, std::size_t>> findNextStep(std::size_t levelIndex, Hero target);
    void updateLevel(std::size_t levelIndex, gf::Vector2i next);
    void updateRoot(RootState& root);

  private:
    GameState& m_state;
    gf::Random& m_random;
    gf::Time m_time;
  };

}


#endif // RC_ROOT_MODEL_H
