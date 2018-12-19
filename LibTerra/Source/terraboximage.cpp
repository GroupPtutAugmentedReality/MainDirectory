#include "archestree.h"

/*!
\class TerraBoxImage archestree.h
\brief Height field node.
\ingroup ArchesTree
*/

/*!
\brief Create the node.
\param image The image, its alpha channel is used to compute the weight mask.
\param alias Alias.
\param a, b Vectors defining the bounding box of the terrain.
*/
TerraBoxImage::TerraBoxImage(const QString& image, const QString& alias, const Vector& a, const Vector& b) :TerraImages(image, alias)
{
  TerraBoxImage::a = a;
  TerraBoxImage::b = b;
  box666 = Box2(Vector2::Min(a, b), Vector2::Max(a, b));
}

/*!
\brief Create the node.
\param image Image defining the height field.
\param alias Alias.
\param alpha Weight mask image.
\param a, b Vectors defining the bounding box of the terrain.
*/
TerraBoxImage::TerraBoxImage(const QString& image, const QString& alpha, const QString& alias, const Vector& a, const Vector& b) :TerraImages(image, alpha, alias)
{
  TerraBoxImage::a = a;
  TerraBoxImage::b = b;
  box666 = Box2(a, b);
}

/*!
\brief Compute the field at given point.
\param p Point.
*/
ScalarAlpha TerraBoxImage::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double h = GetImage(reference).Value(p);
  double w = GetImage(reference).Value(p);

  return ScalarAlpha(h, w);
}

/*!
\brief Compute the size of a node.
*/
int TerraBoxImage::Memory() const
{
  return sizeof(TerraBoxImage);
}
