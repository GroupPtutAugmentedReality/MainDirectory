#include "archestree.h"

/*!
\class TerraScale archestree.h
\brief Scaling operator.

\ingroup ArchesTree
*/

/*!
\brief Create a scaling node.
\param node Sub-tree.
\param s Scaling vector.
*/
TerraScale::TerraScale(TerraNode* node, const Vector& s) :TerraUnary(node), s(s)
{
  box666 = node->GetBox().Scaled(Vector2(s));
}

/*!
\brief Recursively destroys the sub-tree.
*/
TerraScale::~TerraScale()
{
}

/*!
\copydoc TerraNode::ElevationAlpha
*/
ScalarAlpha TerraScale::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  Vector2 q = p.Scale(Vector2(1.0 / s[0], 1.0 / s[1]));

  ScalarAlpha sa = node->ElevationAlpha(q);

  return  ScalarAlpha(sa.Value() * s[2], sa.Alpha());
}

/*!
\brief Compute the size of a node.
*/
int TerraScale::Memory() const
{
  return sizeof(TerraScale) + node->Memory();
}
