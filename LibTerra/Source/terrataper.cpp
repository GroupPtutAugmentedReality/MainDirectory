#include "archestree.h"

/*!
\class TerraTaper archestree.h
\brief Vertical tapering operator.

\ingroup ArchesTree
*/

/*!
\brief Create a scaling node.
\param node Sub-tree.
\param a SubTree defining the tapering function.
*/
TerraTaper::TerraTaper(TerraNode* node, AnalyticFieldNode* a) :TerraUnary(node)
{
  box666 = node->GetBox();
}

/*!
\brief Recursively destroys the sub-tree.
*/
TerraTaper::~TerraTaper()
{
  delete a;
}

/*!
\brief Evaluate the field at a given point.
\param p Point.
*/
ScalarAlpha TerraTaper::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  ScalarAlpha xa = node->ElevationAlpha(p);
  double t = a->Value(p);
  return ScalarAlpha(xa.Value()*t, xa.Alpha());
}

/*!
\brief Compute the size of a node.
*/
int TerraTaper::Memory() const
{
  return sizeof(TerraTaper) + node->Memory() + a->Memory();
}
