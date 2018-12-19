#include "archestree.h"
#include "cubic.h"

/*!
\class TerraCubic archestree.h
\brief Generate cubic curve skeletal element.
\ingroup ArchesTree
*/

/*!
\brief Create a generate cubic curve skeletal element.
\param a,b Points.
\param ta, tb Tangents.
*/
TerraCubic::TerraCubic(const Vector& a, const Vector& b, const Vector& ta, const Vector& tb)
{
  TerraCubic::a = a;
  TerraCubic::b = b;

  x = Cubic::Hermite(a[0], b[0], ta[0], tb[0]);
  y = Cubic::Hermite(a[1], b[1], ta[1], tb[1]);
  z = Cubic::Hermite(a[2], b[2], ta[2], tb[2]);

  box666 = CubicCurve2(x, y).GetBox();

  // Quartic constant coefficients
  xyz[4] = 3.0*(x[3] * x[3] + y[3] * y[3]);
  xyz[3] = 5.0*(x[3] * x[2] + y[3] * y[2]);
  xyz[2] = 4.0*(x[3] * x[1] + y[3] * y[1]) + 2.0*(x[2] * x[2] + y[2] * y[2]);
  xyz[1] = 3.0*(x[2] * x[1] + y[2] * y[1]);
  xyz[0] = x[1] * x[1] + y[1] * y[1];
}

/*!
\brief Compute the size of a node.
*/
int TerraCubic::Memory() const
{
  return sizeof(TerraCubic);
}
