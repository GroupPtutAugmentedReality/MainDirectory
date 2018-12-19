#include "archestree.h"

/*!
\class TerraSub archestree.h
\brief A combination node that adds the thicknesses of the different materials.
\ingroup ArchesTree
*/

/*!
\brief Create the node.
\param a, b Argument sub-trees.
*/
TerraSub::TerraSub(TerraNode* a, TerraNode* b) :TerraBinary(a, b)
{
}

/*!
\copydoc TerraNode::ElevationAlpha
*/
ScalarAlpha TerraSub::ElevationAlpha(const Vector2& p) const
{
  double w = 0.0;
  double hr = 0.0;
  double hw = 0.0;
  double hs = 0.0;
  if (!box666.Inside(p))
    return ScalarAlpha();

  ScalarAlpha msaa = a->ElevationAlpha(p);
  ScalarAlpha msab = b->ElevationAlpha(p);
  double wa = msaa.Alpha();
  double wb = msab.Alpha();
  w = wa;

  double hra = msaa.Value();
  double hrb = msab.Value();
  hr = hra - wb * hrb;

  return ScalarAlpha(hr, w);
}
