#include "archestree.h"

SimplexTurbulence TerraVertexNoise::turbulence(0.5, 0.5, 4);

/*!
\class TerraVertexNoise archestree.h
\brief Procedural noise over a disc.
\ingroup ArchesTree
*/

/*!
\brief Create a noise disc primitive.
\param c Center.
\param a Amplitude of the noise.
\param lambda Fundamental wavelength.
\param r Radius.
\param al Alpha.
*/
TerraVertexNoise::TerraVertexNoise(const Vector& c, const double& a, const double& lambda, const double& r, const double& al) :TerraVertex(c, r, al), a(a), lambda(lambda)
{
  t0 = turbulence.At(c / lambda);
}

/*!
\brief Compute material at a given point.
\param p Point.
*/
ScalarAlpha TerraVertexNoise::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = SquaredNorm(p - Vector2(c));

  if (d > r*r)
    return ScalarAlpha();

  double w = Alpha(d);
  double z = c[2] + a * (turbulence.At(p / lambda) - t0);

  return ScalarAlpha(z, w);
}

/*!
\brief Compute the size of the node.
*/
int TerraVertexNoise::Memory() const
{
  return sizeof(TerraVertexNoise);
}
