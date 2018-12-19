#include "archestree.h"
#include "cubic.h"

/*!
\class TerraCubicImage archestree.h
\brief Image-based primitive along a curve.
\ingroup ArchesTree
*/

/*!
\brief Create an image-based primitive along a curve.
\param a,b Control points.
\param ta, tb Control tangents.
\param r Radius.
\param z Elevation.
*/
TerraCubicImage::TerraCubicImage(const Vector& a, const Vector& b, const Vector& ta, const Vector& tb, const double& r, const double& z, const QString& image, const QString& alpha, const QString& alias, const double& beta) :TerraCubic(a, b, ta, tb), TerraImages(image, alpha, alias, beta)
{
  TerraCubicImage::r1 = r;
  TerraCubicImage::r2 = r;
  TerraCubicImage::dz1 = z;
  TerraCubicImage::dz2 = z;

  // Box offseted with range of profile curves
  box666 = CubicCurve2(x, y).GetBox().Extended(r);

}

TerraCubicImage::TerraCubicImage(const Vector& a, const Vector& b, const Vector& ta, const Vector& tb, const double& r, const double& z, const QString& alias, const double& beta) :TerraCubic(a, b, ta, tb), TerraImages(alias, beta)
{
  TerraCubicImage::r1 = r;
  TerraCubicImage::r2 = r;
  TerraCubicImage::dz1 = z;
  TerraCubicImage::dz2 = z;

  // Box offseted with range of profile curves
  box666 = CubicCurve2(x, y).GetBox().Extended(r);
}


TerraCubicImage::TerraCubicImage(const Vector& a, const Vector& b, const Vector& ta, const Vector& tb, const double& r1, const double& r2, const double& z1, const double& z2, const QString& image, const QString& alpha, const QString& alias, const double& beta) :TerraCubic(a, b, ta, tb), TerraImages(image, alpha, alias, beta)
{
  TerraCubicImage::r1 = r1;
  TerraCubicImage::r2 = r2;
  TerraCubicImage::dz1 = z1;
  TerraCubicImage::dz2 = z2;

  double r = Math::Max(r1, r2);
  // Box offseted with range of profile curves
  box666 = CubicCurve2(x, y).GetBox().Extended(r);
}

TerraCubicImage::TerraCubicImage(const Vector& a, const Vector& b, const Vector& ta, const Vector& tb, const double& r1, const double& r2, const double& z1, const double& z2, const QString& alias, const double& beta) :TerraCubic(a, b, ta, tb), TerraImages(alias, beta)
{
  TerraCubicImage::r1 = r1;
  TerraCubicImage::r2 = r2;
  TerraCubicImage::dz1 = z1;
  TerraCubicImage::dz2 = z2;

  // Box offseted with range of profile curves
  box666 = CubicCurve2(x, y).GetBox().Extended(Math::Max(r1, r2));
}

/*!
\brief
*/
double TerraCubicImage::GetHeightFromImage(const double& x, const double& y) const
{
  double u = Math::Clamp(x, 0.0, 1.0);
  double v = Math::Clamp(0.5*(1.0 + y), 0.0, 1.0);

  double dz = dz2 * u + dz1 * (1 - u);

  double h = GetImage(reference).Value(Vector2(u, v));

  return dz * h;
}

/*!
\brief
*/
double TerraCubicImage::GetWeightFromImage(const double& x, const double& y) const
{
  double u = Math::Clamp(x, 0.0, 1.0);
  double v = Math::Clamp(0.5*(1.0 + y), 0.0, 1.0);

  double h = GetImage(reference).Value(Vector2(u, v));

  return h;
}

/*!
\copydoc TerraNode::ElevationAlpha
*/
ScalarAlpha TerraCubicImage::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  // W
  double u, v;

  double dd = CubicCurve2(x, y).UV(p, u, v);

  double r = r2 * u + r1 * (1 - u);
  double w = GetWeightFromImage(u, v / r);

  double uu;
  Vector2 n = CubicCurve2(x, y).Normal(p, uu);

  double d = n * n;

  if (d > r*r) w = 0.0;
  if ((uu <= 0.0000000001) || (uu >= 1.0 - 0.0000000001))
  {
    w = 0.0;
  }

  // Z
  double h = GetHeightFromImage(u, v / r);

  //double hc=z(u);
  //h=h+hc;
  /*
  double water = 0.0;
  if (h < 0 && w != 0.0)
  {
    water = fabs(h);
  }*/

  double hc = z(u);
  h = h + hc;

  return ScalarAlpha(h, w);
}

/*!
\brief Compute the size of a node.
*/
int TerraCubicImage::Memory() const
{
  return sizeof(TerraCubicImage);
}
