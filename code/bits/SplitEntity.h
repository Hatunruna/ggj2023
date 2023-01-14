#ifndef XY_SPLIT_ENTITY_H
#define XY_SPLIT_ENTITY_H

#include <gf/Entity.h>

namespace xy {

  class SplitEntity : public gf::Entity {
  public:
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
  };

}

#endif // XY_SPLIT_ENTITY_H
