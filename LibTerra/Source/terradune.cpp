#include "archestree.h"
#include "cubic.h"

/*!
\class TerraDune archestree.h
\brief Dune primitive.
\ingroup ArchesTree
*/

/*!
\brief Create a sand dune.
*/
TerraDune::TerraDune(const Vector& a, const Vector& b, const Vector& ta, const Vector& tb, double h, double si, double se) :TerraCubic(a, b, ta, tb)
{
  double r = Math::Max(si, se);
  box666.Extend(r);

  height = h;
  size_interior = si;
  size_exterior = se;
}

/*!
\copydoc TerraNode::ElevationAlpha
*/
ScalarAlpha TerraDune::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  double u;
  double v;
  double dd = CubicCurve2(x, y).UV(p, u, v);

  double height_u = 0;
  double size_ext_u = 0;
  double size_int_u = 0;

  if (u <= 0.5)
  {
    height_u = Math::Lerp(0, height, Math::SymetricSqr(u * 2));
    size_ext_u = Math::Lerp(0, size_exterior, Math::SymetricSqr(u * 2));
    size_int_u = Math::Lerp(0, size_interior, Math::SymetricSqr(u * 2));
  }
  else //if(u > 0.5)
  {
    height_u = Math::Lerp(height, 0, Math::Sqr((u * 2) - 1));
    size_ext_u = Math::Lerp(size_exterior, 0, Math::Sqr((u * 2) - 1));
    size_int_u = Math::Lerp(size_interior, 0, Math::Sqr((u * 2) - 1));
  }

  double perc = 0.60;
  double size_ext_u_w = perc * size_ext_u;
  double size_int_u_w = perc * size_int_u;

  double sand;
  double weight = 1.0;
  if (v == 0)
    sand = height_u;
  else if (v > 0)
  {
    if (v > size_ext_u)
      sand = 0;
    else
      sand = Math::Lerp(height_u, 0, Math::Sqr(v / size_ext_u));

    if (v >= size_ext_u)
      weight = 0.0;
    else if (v <= size_ext_u_w)
      weight = 1.0;
    else
      weight = Math::Lerp(1.0, 0.0, Cubic::Smooth((v - size_ext_u_w) / (size_ext_u - size_ext_u_w)));
  }
  else
  {
    if (-v > size_int_u)
      sand = 0;
    else
      sand = Math::Lerp(height_u, 0, Math::SymetricSqr(-v / size_int_u));

    if (-v >= size_int_u)
      weight = 0.0;
    else if (-v <= size_int_u_w)
      weight = 1.0;
    else
      weight = Math::Lerp(1.0, 0.0, Cubic::Smooth((-v - size_int_u_w) / (size_int_u - size_int_u_w)));
  }

  sand = abs(sand);
  //double rock = z(u);

  return ScalarAlpha(sand, weight);
}


/*!
\brief Compute the size of a node.
*/
int TerraDune::Memory() const
{
  return sizeof(TerraDune);
}
