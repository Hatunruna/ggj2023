#ifndef RC_ROOT_MODEL_H
#define RC_ROOT_MODEL_H

#include <gf/Model.h>
#include <gf/Random.h>

namespace rc {
  struct GameState;
  struct RootState;

  class RootModel : public gf::Model {
  public:
    RootModel(GameState& state, gf::Random& random);
    void update(gf::Time time) override;

  private:
    void updateLevel(gf::Time time, std::size_t levelIndex);
    void updateRoot(RootState& root);

  private:
    GameState& m_state;
    gf::Random& m_random;
    gf::Time m_time;
  };

}


#endif // RC_ROOT_MODEL_H
