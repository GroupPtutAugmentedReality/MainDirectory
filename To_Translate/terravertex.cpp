#include "archestree.h"

/*!
\class TerraVertex archestree.h
\brief A disc primitive.
\ingroup ArchesTree
*/

/*!
\brief Create a disc primitive.
\param c Center.
\param r Radius.
\param a Alpha.
*/
TerraVertex::TerraVertex(const Vector& c, const double& r, const double& a) :TerraSkeleton(r, a)
{
  TerraVertex::c = c;
  box666 = Box2(c, r);
}

/*!
\brief Compute the fields.
\param p Point.
*/
ScalarAlpha TerraVertex::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = SquaredNorm(p - Vector2(c));

  return ScalarAlpha(c[2], Alpha(d));
}

/*!
\brief Compute the size of a node.
*/
int TerraVertex::Memory() const
{
  return sizeof(TerraVertex);
}

/*!
\brief Renders the node into a QGraphicsScene.
\param scene The scene.
*/
void TerraVertex::Draw(QGraphicsScene& scene) const
{
  QPen pen(QColor(200, 200, 200), 0.5);
  Circle2(c, r).Draw(scene, pen);
}