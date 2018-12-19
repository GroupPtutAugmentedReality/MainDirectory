#include "archestree.h"

/*!
\class TerraDisc archestree.h
\brief A disc primitive.
\ingroup ArchesTree
*/

/*!
\brief Create a disc primitive.
\param d Disc.
\param r Radius.
\param z Elevation.
\param a Alpha.
*/
TerraDisc::TerraDisc(const Disc2& d, const double& z, const double& r, const double& a) :TerraSkeleton(r, a), disc(d)
{
  TerraDisc::z = z;
  box666 = d.GetBox().Extended(r);
}

/*!
\brief Compute the fields.
\param p Point.
*/
ScalarAlpha TerraDisc::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = disc.R(p);

  return ScalarAlpha(z, Alpha(d));
}

/*!
\brief Compute the size of a node.
*/
int TerraDisc::Memory() const
{
  return sizeof(TerraDisc);
}

/*!
\brief Renders the node into a QGraphicsScene.
\param scene The scene.
*/
void TerraDisc::Draw(QGraphicsScene& scene) const
{
  disc.Draw(scene);
  disc.Draw(scene);
  disc.Extended(r).Draw(scene);
}