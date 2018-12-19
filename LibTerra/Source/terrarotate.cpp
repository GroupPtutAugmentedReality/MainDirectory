#include "archestree.h"

/*!
\class TerraRotate archestree.h
\brief Rotation operator.

\ingroup ArchesTree
*/

/*!
\brief Create a rotation node.
\param n Sub-tree.
\param a Angle of rotation.
\param c Center.
*/
TerraRotate::TerraRotate(TerraNode* n, const double& a, const Vector2& c) :TerraUnary(n),c(c)
{
  r = Matrix2::Rotation(Math::DegreeToRadian(a));

  box666 = node->GetBox().Translated(-c).Rotated(r).Translated(c);
}

/*!
\brief Recursively destroys the sub-tree.
*/
TerraRotate::~TerraRotate()
{
}

/*!
\brief Evaluate the field at a given point.
\param p Point.
*/
ScalarAlpha TerraRotate::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  return node->ElevationAlpha(c + r * (p - c));
}

/*!
\brief Compute the size of a node.
*/
int TerraRotate::Memory() const
{
  return sizeof(TerraRotate) + node->Memory();
}
