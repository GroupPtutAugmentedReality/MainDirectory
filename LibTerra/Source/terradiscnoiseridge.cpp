#include "archestree.h"

/*!
\class TerraVertexNoiseRidge archestree.h
\brief Ridged noise over a disc.
\ingroup ArchesTree
*/

/*!
\brief Creates a flat disc primitive.
\param p Center.
\param r Radius.
\param lac Lacunarity.
\param H Height.
\param oc Octaves.
\param off Offset;
\param gain Gain.
\param a Amplitude
\param s Frequency of the noise
\param r Radius.
\param q Alpha.
*/
TerraVertexNoiseRidge::TerraVertexNoiseRidge(const Vector& p, double lac, double H, int oc, double off, double gain, double a, double s, const double& r, const double& q) :TerraVertex(p, r, q)
{
  TerraVertexNoiseRidge::lacunarity = lac;
  TerraVertexNoiseRidge::scalefactor = s;
  TerraVertexNoiseRidge::H = H;
  TerraVertexNoiseRidge::oc = oc;
  TerraVertexNoiseRidge::off = off;
  TerraVertexNoiseRidge::gain = gain;
  TerraVertexNoiseRidge::a = a;

  // Box
  box666 = Box2(p, r);
}

/*!
\brief Compute material at a given point.
\param p Point.
*/
ScalarAlpha TerraVertexNoiseRidge::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = SquaredNorm(p - Vector2(c));

  if (d > r*r)
    return ScalarAlpha();

  double w = Alpha(d);

  double z =  AnalyticMusgrave::RidgedMultifractal(p / scalefactor, c[2],a, lacunarity, oc, off, gain,H);


  return ScalarAlpha(z, w);
}

/*!
\brief Compute the size of a node.
*/
int TerraVertexNoiseRidge::Memory() const
{
  return sizeof(TerraVertexNoiseRidge);
}
