#include "archestree.h"

/*!
\class TerraUnary archestree.h
\brief Unary operators.

\ingroup ArchesTree 
*/

/*!
\brief Create an unary node.
\param a Sub-tree.
*/
TerraUnary::TerraUnary(TerraNode* a)
{
  TerraUnary::node = a;

  box666 = node->GetBox();
}

/*!
\brief Recusively destroy the sub-tree.
*/
TerraUnary::~TerraUnary()
{
  delete node;
}

/*!
\brief Recursively compute the number of nodes that are traversed when evaluating the tree.
\param p Point.
*/
int TerraUnary::Traversal(const Vector2& p) const
{
  int n = 1;

  if (!box666.Inside(p))
    return n;

  n += node->Traversal(p);

  return n;
}

/*!
\brief Compute the size of a node.
*/
int TerraUnary::Memory() const
{
  return sizeof(TerraUnary) + node->Memory();
}


/*!
\brief Place a node at a given location.
\param node Sub-tree.
\param s Scale.
\param a Rotation angle.
\param t Translation vector
*/
TerraNode* TerraUnary::PutAt(TerraNode* node, const Vector& s, const double& a, const Vector2& t)
{
  return new TerraTranslate(new TerraRotate(new TerraScale(node, s), a), t);
}

/*!
\brief Place a node at a given location.
\param node Sub-tree.
\param a Rotation angle.
\param t Translation vector
*/
TerraNode* TerraUnary::PutAt(TerraNode* node, const double& a, const Vector2& t)
{
  return new TerraTranslate(new TerraRotate(node, a), t);
}