#include "archestree.h"
#include "fieldtree.h"

/*!
\class TerraWarpNoise archestree.h
\brief A warping operator.

\ingroup ArchesTree
*/

/*!
\brief Create a warping operator.
\param node Sub-tree.
\param w Warping.
\param c Center.
\param r Maximum warping distance.
*/
TerraWarpNoise::TerraWarpNoise(TerraNode* node, const Vector2& c, const double& r, const AnalyticFieldFractal& wQXXQ) :TerraUnary(node), warpQXXQ(wQXXQ)
{
  TerraWarpNoise::c = c;

  box666 = node->GetBox().Extended(r);
}

/*!
\brief Evaluate the field at a given point.
\param p Point.
*/
ScalarAlpha TerraWarpNoise::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  Vector2 cp = c - p;

  if (cp != Vector(0.0))
  {
    Normalize(cp);
  }

  Vector2 d = cp * warpQXXQ.Value(p);
  return node->ElevationAlpha(p + d);
}

/*!
\brief Compute the size of a node.
*/
int TerraWarpNoise::Memory() const
{
  return sizeof(TerraWarpNoise) + node->Memory();
}
