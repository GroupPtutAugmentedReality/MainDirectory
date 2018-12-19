#include "archestree.h"

/*!
\class TerraBox archestree.h
\brief Box node.
\ingroup ArchesTree
*/

/*!
\brief Create the node.
\param box The box.
\param z Elevation.
\param r Radius of influnce.
\param alpha Alpha.
*/
TerraBox::TerraBox(const Box2& box, const double& z, const double& r, const double& alpha) :box(box)
{
  TerraBox::alpha = alpha;
  TerraBox::z = z;
  TerraBox::r = r;

  // Box
  box666 = box.Extended(r);
}

/*!
\brief Compute the field at given point.
\param p Point.
*/
ScalarAlpha TerraBox::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = box.R(p);
  return ScalarAlpha(z, alpha*Quadric::SmoothCompact(d, r*r));
}

/*!
\brief Compute the size of a node.
*/
int TerraBox::Memory() const
{
  return sizeof(TerraBox);
}
