#include "archestree.h"

/*!
\class TerraTwist archestree.h
\brief A twisting operator.

\ingroup ArchesTree
*/

/*!
\brief Create a twisting operator.
\param node Sub-tree.
\param t Twist.
*/
TerraTwist::TerraTwist(TerraNode* node, const Twist2& t) :TerraUnary(node), twist(t)
{
  box666 = Box2(node->GetBox(), twist.GetBox());
}

/*!
\brief Evaluate the field at a given point.
\param p Point.
*/
ScalarAlpha TerraTwist::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  Vector2 q = twist.InverseTransform(p);

  return node->ElevationAlpha(q);
}

/*!
\brief Compute the size of a node.
*/
int TerraTwist::Memory() const
{
  return sizeof(TerraTwist) + node->Memory();
}
