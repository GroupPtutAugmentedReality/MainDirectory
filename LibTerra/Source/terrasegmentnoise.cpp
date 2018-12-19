#include "archestree.h"
#include "quadric.h"

SimplexTurbulence TerraSegmentNoise::turbulence(0.5, 0.5, 4);

/*!
\class TerraSegmentNoise archestree.h
\brief Noise elevation around a segment skeleton.
\ingroup ArchesTree
*/

/*!
\brief Create the primitive.
\param s %Segment.
\param a Amplitude of the noise.
\param lambda Fundamental wavelength.
\param r Radius.
\param al Alpha.
*/
TerraSegmentNoise::TerraSegmentNoise(const Segment2& s, const double& a, const double& lambda, const double& r, const double& al) :TerraSkeleton(r, al), Segment2(s), a(a), lambda(lambda)
{
  t0 = turbulence.At(0.5*(a + b) / lambda);

  // Box
  box666 = Segment2::GetBox().Extended(r);
}


/*!
\brief Compute material at given point.
\param p Point.
*/
ScalarAlpha TerraSegmentNoise::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = Segment2::R(p);
  if (d > r*r)
    return ScalarAlpha();

  double w = Alpha(d);

/*
double u = (p - Segment2::a)*Normalized(b - a);
*/
double z = /*(1.0 - u)*Segment::a + u * Segment2::b[2] +*/ a * (turbulence.At(p / lambda) - t0);

  return ScalarAlpha(z, w);
}

/*!
\brief Compute the size of a node.
*/
int TerraSegmentNoise::Memory() const
{
  return sizeof(TerraSegmentNoise);
}
