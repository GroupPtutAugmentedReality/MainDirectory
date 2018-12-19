#include "archestree.h"
#include "circle.h"
#include "cubic.h"

/*!
\class TerraPlateau archestree.h
\brief A plateau operator.

\ingroup ArchesTree
*/

/*!
\brief Create a plateau operator.
\param node Argument node.
\param c Center.
\param r Radius.
\param a,b Elevation range.
\param f Flatness coefficient.
*/
TerraPlateau::TerraPlateau(TerraNode* node, const Vector2& c, const double& r, const double& a, const double& b, const double& f) :TerraUnary(node)
{
  TerraPlateau::a = a;
  TerraPlateau::b = b;
  TerraPlateau::c = c;
  TerraPlateau::r = r;
  TerraPlateau::flat = f;
}

/*!
\brief Evaluate the materials at a given point in the plane.
\param p Point.
*/
ScalarAlpha TerraPlateau::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha();

  ScalarAlpha msaa = node->ElevationAlpha(p);

  // Elevation
  double h = msaa.Value();

  if (h<a && h>b)
    return msaa;

  // Do not warp elevation if outisde or region of influence
  if (!Circle2(c, r).Inside(p))
    return msaa;

  // Compute vertical warping

  // Local coordinate x in [0,1]
  double x = Linear::Step(h, a, b);

  // Center and falloff function
  double u = 4.0*Math::Sqr(x - 0.5);
  double e = (1.0 - u)*(1.0 - u)*(1.0 - u);

  // New elevation, which involves flattening coefficient
  double he = h - flat * e;

  // Now blend old elevation with new
  double d = SquaredNorm(p - c);

  double alpha = Cubic::SmoothCompact(d, r*r);

  double z = (1.0 - alpha)*h + alpha * he;

  return ScalarAlpha(z, msaa.Alpha());
}


/*!
\brief Renders the node into a QGraphicsScene.
\param scene The scene.
*/
void TerraPlateau::Draw(QGraphicsScene& scene) const
{
  QPen pen(QColor(200, 200, 200), 0.5);
  Circle2(c, r).Draw(scene, pen);
}