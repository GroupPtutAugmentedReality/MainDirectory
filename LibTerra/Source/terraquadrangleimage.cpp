#include "archestree.h"
#include "quad.h"

/*!
\class TerraQuadrangleImage archestree.h
\brief Image-based height field nodes.
\ingroup ArchesTree
*/

/*!
\brief Create the node.
\param alias The image, its alpha channel is used to compute the weight mask.
\param q %Quadrangle.
\param ha, hb, hc, hd Elevation.
\param beta Importance.
*/
TerraQuadrangleImage::TerraQuadrangleImage(const QString& alias, const Quadrangle& q, const double&ha, const double& hb, const double& hc, const double& hd, const double& beta) :TerraImages(alias, beta)
{
  TerraQuadrangleImage::q = q;

  TerraQuadrangleImage::za = ha;
  TerraQuadrangleImage::zb = hb;
  TerraQuadrangleImage::zc = hc;
  TerraQuadrangleImage::zd = hd;

  box666 = Box2(q.GetBox());
}

/*!
\brief Create the node.
\param image, alias The image, its alpha channel is used to compute the weight mask.
\param q %Quadrangle.
\param ha, hb, hc, hd Elevation.
\param beta Importance.
*/
TerraQuadrangleImage::TerraQuadrangleImage(const QString& image, const QString& alias, const Quadrangle& q, const double&ha, const double& hb, const double& hc, const double& hd, const double& beta) :TerraImages(image, alias, beta)
{
  TerraQuadrangleImage::q = q;

  TerraQuadrangleImage::za = ha;
  TerraQuadrangleImage::zb = hb;
  TerraQuadrangleImage::zc = hc;
  TerraQuadrangleImage::zd = hd;

  box666 = Box2(q.GetBox());
}

/*!
\brief Create the node.
\param image, alpha, alias Image defining the height field.
\param alpha Weight mask image.
\param q %Quadrangle.
\param ha, hb, hc, hd Elevation.
\param beta Importance.
*/
TerraQuadrangleImage::TerraQuadrangleImage(const QString& image, const QString& alpha, const QString& alias, const Quadrangle& q, const double&ha, const double& hb, const double& hc, const double& hd, const double& beta) :TerraImages(image, alpha, alias, beta)
{
  TerraQuadrangleImage::q = q;

  TerraQuadrangleImage::za = ha;
  TerraQuadrangleImage::zb = hb;
  TerraQuadrangleImage::zc = hc;
  TerraQuadrangleImage::zd = hd;

  box666 = Box2(q.GetBox());
}

/*!
\brief Compute material at given point.
\param p Point.
*/
ScalarAlpha TerraQuadrangleImage::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double x;
  double y;

  if (Quadrangle2::InverseBilinear(q.Vertex(0), q.Vertex(1), q.Vertex(3), q.Vertex(2), p, x, y) == 0)
    return ScalarAlpha();

  if (x>1.0 || x<0.0 || y>1.0 || y<0.0)
    return ScalarAlpha();

  double h = Math::BiCubic(x, y, q.Vertex(0)[2], q.Vertex(1)[2], q.Vertex(3)[2], q.Vertex(2)[2]);
  double dh = GetImage(reference).Value(Vector2(x, y)) - 0.5;

  // Depth is modified
  dh *= Math::BiCubic(x, y, za, zb, zd, zc);
  // Water
  double water = 0.0;
  if (dh < 0.0)
  {
    water = fabs(dh);
  }

  h = h + dh;

  double w = GetImage(reference).Value(Vector2(x, y));

  return ScalarAlpha(h, /*0.0, 0.0, 0.0, 0.0, water, 0.0,*/ w/*, beta*/);
}

/*!
\brief Compute the size of a node.
*/
int TerraQuadrangleImage::Memory() const
{
  return sizeof(TerraQuadrangleImage);
}
