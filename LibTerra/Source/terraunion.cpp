#include "archestree.h"

/*!
\class TerraUnion archestree.h
\brief Create a height field with the maximum elevation of sub-trees.
\ingroup ArchesTree
*/

/*!
\brief Create a maximum node.
\param a,b Sub-trees.
*/
TerraUnion::TerraUnion(TerraNode*a, TerraNode*b) :TerraOperator(a, b)
{
}

/*!
\brief Create a maximum node.
\param a Set of sub-trees.
*/
TerraUnion::TerraUnion(QVector<TerraNode*> a) : TerraOperator(a)
{
}

/*!
\brief Evaluate the field at a given point.
\param p Point.
*/
ScalarAlpha TerraUnion::ElevationAlpha(const Vector2& p) const
{
  double a = 0.0;
  double x = 0.0;

  if (!box666.Inside(p))
    return ScalarAlpha();

  if (nodes.size() > 0)
  {
    ScalarAlpha xa = nodes.at(0)->ElevationAlpha(p);
    x = xa.Value();
    a = xa.Alpha();
  }

  for (int i = 1; i < nodes.size(); i++)
  {
    ScalarAlpha xa = nodes.at(i)->ElevationAlpha(p);
    if (xa.Alpha() > 0.0)
    {
      x = Math::Max(x, xa.Value());
      a = Math::Max(a, xa.Alpha());
    }
  }

  return ScalarAlpha(x, a);
}
