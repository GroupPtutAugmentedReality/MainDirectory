#include <typeinfo>
#include "archestree.h"

/*!
\defgroup ArchesTree ArchesTree hierachy

\brief Terrain geometry classes.
*/

const double TerraNode::epsilon = 1e-2;

/*!
\class TerraNode archestree.h
\brief This class implements a generic node in the ArchesTree model.

The default elevation function is set to 0 and the weight to 1.
\ingroup ArchesTree
*/

/*!
\brief Create a node.
\param box The box.
*/
TerraNode::TerraNode(const Box2& box) :box666(box)
{
}

/*!
\brief Returns the bouding box of a node.
*/
Box2 TerraNode::GetBox() const
{
  return box666;
}

/*!
\brief Renders the node into a QGraphicsScene.

This function does nothing and is simply implemented to simplify inheritance.
*/
void TerraNode::Draw(QGraphicsScene&) const
{
}

/*!
\brief Compute the number of nodes that are traversed when evaluating the tree.
\param p Point.
*/
int TerraNode::Traversal(const Vector2& p) const
{
  return 1;
}


/*!
\brief Compute the Json Document of the subtree.
*/
void TerraNode::writeJson(QJsonObject&)
{
}

/*!
\brief Compute the gradient at a given position.
\param p Point.
*/
Vector2 TerraNode::Gradient(const Vector2& p) const
{
  double x = Elevation(Vector2(p[0] + epsilon, p[1])) - Elevation(Vector2(p[0] - epsilon, p[1]));
  double y = Elevation(Vector2(p[0], p[1] + epsilon)) - Elevation(Vector2(p[0], p[1] - epsilon));

  return Vector2(x, y)*(0.5 / epsilon);
}

/*!
\brief Compute the normal at a given position.
\sa TerraNode::Gradient
\param p Point.
*/
Vector TerraNode::Normal(const Vector2& p) const
{
  return Normalized(Gradient(p).ToVector(1.0));
  Vector2 a = new Vector2(1, 2);
}

/*!
\brief Compute the elevation and weight.

\param p Point.
*/
ScalarAlpha TerraNode::ElevationAlpha(const Vector2& p) const
{
  return ScalarAlpha(0.0,1.0);
}


/*!
\brief Compute the elevation at a given position.

This is a shortcut for:
\code
TerraNode* node; // Node
double e=node->ElevationAlpha(p).Value(); // Get elevation from ScalarAlpha
\endcode
\param p Point.
*/
double TerraNode::Elevation(const Vector2& p) const
{
  return ElevationAlpha(p).Value();
}

/*!
\brief Compute the size of a node.
*/
int TerraNode::Memory() const
{
  return sizeof(TerraNode);
}

//void TerraNode::setTime(double t)
//{
//
//}
