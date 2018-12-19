#include "archestree.h"

/*!
\class TerraLowest archestree.h
\brief Minimum operator.
*/

/*!
\brief Creation.

\param a, b Sub-trees.
*/
TerraLowest::TerraLowest(TerraNode* a, TerraNode* b) :TerraBinary(a, b)
{
  box666 = Box2(a->GetBox(), b->GetBox());
}

/*!
\brief Recursively destroy the sub-tree.
*/
TerraLowest::~TerraLowest()
{
}

/*!
\brief Compute the fields.
\param p Point.
*/
ScalarAlpha TerraLowest::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  ScalarAlpha xa = a->ElevationAlpha(p);
  ScalarAlpha xb = b->ElevationAlpha(p);

  return ScalarAlpha(Math::Min(xa.Value(), xb.Value()), Math::Min(xa.Alpha(), xb.Alpha()));
}
