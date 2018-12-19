#include "archestree.h"

/*!
\class TerraBinary archestree.h
\brief Binary operators.

\ingroup ArchesTree
*/

/*!
\brief Create a binary node.
\param a,b Sub-trees.
*/
TerraBinary::TerraBinary(TerraNode* a, TerraNode* b):TerraNode(Box2(a->GetBox(), b->GetBox()))
{
  TerraBinary::a = a;
  TerraBinary::b = b;
}

/*!
\brief Recusively destroy the sub-trees.
*/
TerraBinary::~TerraBinary()
{
  delete a;
  delete b;
}

/*!
\brief Recursively compute the number of nodes that are traversed when evaluating the tree.
\param p Point.
*/
int TerraBinary::Traversal(const Vector2& p) const
{
  int n = 1;

  if (!box666.Inside(p))
    return n;

  n += a->Traversal(p) + b->Traversal(p);

  return n;
}

/*!
\brief Compute the size of a node.
*/
int TerraBinary::Memory() const
{
  return sizeof(TerraBinary) + a->Memory() + b->Memory();
}
