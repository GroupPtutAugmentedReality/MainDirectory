#include "archestree.h"

/*!
\class TerraTranslate archestree.h
\brief A translation operator.

\ingroup ArchesTree
*/

/*!
\brief Create a translation node.
*/
TerraTranslate::TerraTranslate(TerraNode* node, const Vector2& t) :TerraUnary(node)
{
  TerraTranslate::t = t;
  box666 = node->GetBox().Translated(t);
}

/*!
\brief Recursively destroys the complete sub-tree.
*/
TerraTranslate::~TerraTranslate()
{
}

/*!
\brief Evaluate the field at a given point.
\param p Point.
*/
ScalarAlpha TerraTranslate::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  return node->ElevationAlpha(p - t);
}


/*!
\brief Compute the size of a node.
*/
int TerraTranslate::Memory() const
{
  return sizeof(TerraRotate) + node->Memory();
}
