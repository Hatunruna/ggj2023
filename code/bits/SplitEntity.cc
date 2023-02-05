#include "SplitEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

namespace rc {

  void SplitEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coordinates(target);

    gf::RectangleShape bar(coordinates.getRelativeSize({ 0.005f, 1.0f }));
    bar.setColor(gf::Color::Gray());
    bar.setPosition(coordinates.getRelativePoint({ 0.5f, 0.5f }));
    bar.setAnchor(gf::Anchor::Center);

    target.draw(bar, states);
  }

}
