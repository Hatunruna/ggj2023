#ifndef RC_SPLIT_ENTITY_H
#define RC_SPLIT_ENTITY_H

#include <gf/Entity.h>

namespace rc {

  class SplitEntity : public gf::Entity {
  public:
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
  };

}

#endif // RC_SPLIT_ENTITY_H
