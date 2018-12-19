#include "archestree.h"

/*!
\class TerraVertexSlope archestree.h
\brief A slanted (oblique) disc primitive.
\ingroup ArchesTree
*/

/*!
\brief Create a disc primitive.
\param c Center.
\param n Normal, should be unit.
\param r Radius.
\param a Amplitude weighting the alpha function.
*/
TerraVertexSlope::TerraVertexSlope(const Vector& c, const Vector& n, const double& r, const double& a) :TerraVertexSlope(c, -n[0] / n[2], -n[1] / n[2], r, a)
{
}

/*!
\brief Create a disc primitive.
\param c Center.
\param sx,sy Slopes.
\param r Radius.
\param a Amplitude weighting the alpha function.
*/
TerraVertexSlope::TerraVertexSlope(const Vector& c, const double& sx,const double& sy, const double& r, const double& a) :TerraVertex(c, r, a),sx(sx),sy(sy)
{
}

/*!
\brief Compute the weight and the height of at a given point.
\param p Point.
*/
ScalarAlpha TerraVertexSlope::ElevationAlpha(const Vector2& p) const
{
  // Box
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = SquaredNorm(p - Vector2(c));
  if (d > r*r)
    return ScalarAlpha();

  double w = 0.0;
  double x = p[0] - c[0];
  double y = p[1] - c[1];
  double h = c[2] + sx * x + sy * y;

  w = Alpha(d);
  return ScalarAlpha(h, w);
}


/*!
\brief Compute the size of a node.
*/
int TerraVertexSlope::Memory() const
{
  return sizeof(TerraVertexSlope);
}
