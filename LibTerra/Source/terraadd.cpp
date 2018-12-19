#include "archestree.h"

/*!
\class TerraAdd archestree.h
\brief A combination node that adds the thicknesses of the different materials.
\ingroup ArchesTree
*/

/*!
\brief Create the node.
\param a, b Argument sub-trees.
*/
TerraAdd::TerraAdd(TerraNode* a, TerraNode* b) :TerraBinary(a, b)
{
}

/*!
\brief Evaluate the field at a given point.
\param p Point.
*/
ScalarAlpha TerraAdd::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  ScalarAlpha msaa = a->ElevationAlpha(p);
  ScalarAlpha msab = b->ElevationAlpha(p);
  double wa = msaa.Alpha();
  double wb = msab.Alpha();

  double hra = msaa.Value();
  double hrb = msab.Value();
  double hr = hra + wb * hrb;

  return ScalarAlpha(hr, wa);
}

