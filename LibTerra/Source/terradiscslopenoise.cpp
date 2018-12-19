#include "archestree.h"

/*!
\class Terra2013VertexSlopeNoise archestree.h
\brief Slope disc primitive with noise.
\ingroup ArchesTree
*/

/*!
\brief Creation.
\param p Center.
\param q ?
\param r Radius.
\param a Amplitude weighting the alpha function.
\param na Amplitude of the noise.
\param sa Scaling of the noise.
*/
Terra2013VertexSlopeNoise::Terra2013VertexSlopeNoise(const Vector& p, const Vector& q, const double& r, const double& a, const double& sa, const double& na) :TerraVertexSlope(p, q, r, a)
{
  Terra2013VertexSlopeNoise::s = sa;
  Terra2013VertexSlopeNoise::na = na;
}

/*!
\brief Compute the fields.
\param p Point.
*/
ScalarAlpha Terra2013VertexSlopeNoise::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double d = SquaredNorm(p - Vector2(c));
  if (d > r*r)
    return ScalarAlpha();

  double w = Alpha(d);

  double lac = 2.;
  double H = 1.0;
  int oc = 6;
  double off = 1.1;
  double gain = 1.6;

  double x = p[0] - c[0];
  double y = p[1] - c[1];
  double h = c[2] +sx * x +sy * y;
  double mult = 1.5;//1.4*Norm(Vector(n[0],n[1],0));
  double scal = Vector(x, y, 0)*Normalized(Vector(sx, sy, 0));

  Vector tn = Vector(sx, sy, 0) / Vector(0, 0, 1);
  double scal2 = Vector(x, y, 0)*Normalized(tn);

  double z = na*(AnalyticMusgrave::RidgedMultifractal(Vector(scal2*s, scal*s*0.3, 0), 1.0,1.0, lac, oc, off, gain,H) - AnalyticMusgrave::RidgedMultifractal(Vector(0, 0, 0), 1.0,1.0, lac, oc, off, gain,H));
  double hres = h + mult*z + (na * 0.5 * (AnalyticMusgrave::RidgedMultifractal(p *s, 1.0,1.0, lac, oc, off, gain,H) - AnalyticMusgrave::RidgedMultifractal(c *s, 1.0,1.0, lac, oc, off, gain,H)));
  
  return ScalarAlpha(hres, w);
}

/*!
\brief Compute the size of a node.
*/
int Terra2013VertexSlopeNoise::Memory() const
{
  return sizeof(Terra2013VertexSlopeNoise);
}
