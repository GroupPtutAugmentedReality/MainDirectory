// Heightfield

#include "heightfieldshader.h"

/*!
\class HeightFieldShader heightfieldshader.h
\brief Relief shading class.

\ingroup Structure
*/

/*!
\brief Create a shader
*/
HeightFieldShader::HeightFieldShader(const HeightField& field) :HeightField(field)
{
}

/*!
\brief Create a texture image representing the shaded terrain.
*/
QImage HeightFieldShader::Relief(const double& altmin, const double& altmax) const
{
  // Shading
  QImage shading(nx, ny, QImage::Format_ARGB32);

  // Height field
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      // Height shading: color is a linear interpolation of height colors
      Color cz = ShadeRelief(at(i, j), altmin, altmax);

      Vector n = Normal(i, j);
      double s = n * Normalized(Vector(1.0, 0.5, 2.5));
      s = 0.5*(1.0 + s);
      s *= s;
      s *= s;

      // Normalized direction
      double t = Vector2(n)*Normalized(Vector2(1, 1));
      t = 0.5*(1.0 + t);

      // Normal shading: color is a combination of cool and cold colors according to the orientation
      Color cn = s * Color::Lerp(t, Color(0.65, 0.75, 0.85), Color(1.00, 0.95, 0.80));

      Color c = 0.25*Color(0.975, 0.975, 0.975) + 0.25*cz + 0.50*cn;

      shading.setPixel(i, j, c.GetQt().rgb());
    }
  }
  return shading;
}
