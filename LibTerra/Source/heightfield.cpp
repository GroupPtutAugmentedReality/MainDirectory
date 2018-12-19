// Heightfield
#include "heightfield.h"

#include "triangle.h"
#include "color.h"
#include "palette.h"

/*!
\class HeightField heightfield.h
\brief A height field.

\ingroup Structure
*/

/*!
\brief Create a flat heightfield.
\param box Rectangle domain of the terrain.
\param nx, ny Samples.
\param v Constant elevation.
*/
HeightField::HeightField(const Box2& box, int nx, int ny, const double& v) :ScalarField2(box, nx, ny, v)
{
}

/*!
\brief Create a heightfield.
\param box Rectangle domain of the terrain.
\param nx, ny Samples.
\param v Set of elevation values.
*/
HeightField::HeightField(const Box2& box, int nx, int ny, const QVector<double>& v) : ScalarField2(box, nx, ny, v)
{
}

/*!
\brief Create a heightfield from a scalar field.
\param s Scalar field.
*/
HeightField::HeightField(const ScalarField2& s) :ScalarField2(s)
{
}

/*!
\brief Create a heightfield from an image.
\param box Rectangle domain of the terrain.
\param image Elevation image.
\param a, b Minimum and maximum elevation range.
\param grayscale Boolean set to false if the image is provided in color.
*/
HeightField::HeightField(const Box2& box, const QImage& image, const double& a, const double& b, bool grayscale) :ScalarField2(box, image, a, b, grayscale)
{
}

/*!
\brief Refine the terrain.

This function subdivides the samples and uses a smooth interpolation.
A random elevation may be added to the samples.
\param e Amplitude of the random elevation added to the samples.
\param r %Random number generator.
*/
void HeightField::Subdivide(const double& e, Random& r)
{
  // Extend grid and smooth values
  ScalarField2::Subdivide();

  // Add some random values
  for (int i = 1; i < nx; i += 2)
  {
    for (int j = 1; j < ny; j += 2)
    {
      field[VertexIndex(i, j)] += r.Uniform(-e, e);
    }
  }
}

/*!
\brief Compute the normal for a given position on the terrain.

Note that this function may be expensive to compute.

\param p Point.
\param triangular Boolean, use triangle normals if set to true, bilinear interpolation of normals at vertices otherwize.
*/
Vector HeightField::Normal(const Vector2& p, bool triangular) const
{
  double u, v;
  int i, j;
  CellInteger(p, i, j, u, v);

  // Test position
  if (!InsideCellIndex(i, j))
    return Vector(0.0, 0.0, 1.0);

  if (triangular)
  {
    if (u > v)
    {
      return Triangle(Vertex(i, j), Vertex(i + 1, j), Vertex(i + 1, j + 1)).Normal();
    }
    else
    {
      return Triangle(Vertex(i, j), Vertex(i + 1, j + 1), Vertex(i, j + 1)).Normal();
    }
  }
  else
  {
    return Normalized(Vector::Bilinear(Normal(i, j), Normal(i + 1, j), Normal(i + 1, j + 1), Normal(i, j + 1), u, v));
  }
}

/*!
\brief Compute the normal at a given sample.

This function uses the weighted sum (area) of the normals of the
triangles sharing the point on the grid. The returned vector is normalized.

\param i,j Integer coordinates of the sample.
*/
Vector HeightField::Normal(int i, int j) const
{
  Vector n;
  if (i == 0)
  {
    if (j == 0)
    {
      // Corner: 0/1
      n = Triangle(Vertex(i, j), Vertex(i + 1, j), Vertex(i + 1, j + 1)).AreaNormal() + Triangle(Vertex(i, j), Vertex(i + 1, j + 1), Vertex(i, j + 1)).AreaNormal();
    }
    else if (j == ny - 1)
    {
      // Corner: 5
      n = Triangle(Vertex(i, j), Vertex(i, j - 1), Vertex(i + 1, j)).AreaNormal();
    }
    else
    {
      // Edge: 0/1/5
      n = Triangle(Vertex(i, j), Vertex(i + 1, j), Vertex(i + 1, j + 1)).AreaNormal() + Triangle(Vertex(i, j), Vertex(i + 1, j + 1), Vertex(i, j + 1)).AreaNormal()
        + Triangle(Vertex(i, j), Vertex(i, j - 1), Vertex(i + 1, j)).AreaNormal();
    }
  }
  else if (i == nx - 1)
  {
    if (j == 0)
    {
      // Corner: 2
      n = Triangle(Vertex(i, j), Vertex(i, j + 1), Vertex(i - 1, j)).AreaNormal();

    }
    else if (j == ny - 1)
    {
      // Corner: 3/4
      n = Triangle(Vertex(i, j), Vertex(i - 1, j - 1), Vertex(i, j - 1)).AreaNormal() + Triangle(Vertex(i, j), Vertex(i - 1, j), Vertex(i - 1, j - 1)).AreaNormal();
    }
    else
    {
      // Edge: 2/3/4
      n = Triangle(Vertex(i, j), Vertex(i, j + 1), Vertex(i - 1, j)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i - 1, j), Vertex(i - 1, j - 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i - 1, j - 1), Vertex(i, j - 1)).AreaNormal();
    }
  }
  else
  {
    if (j == 0)
    {
      // Edge: 0/1/2
      n = Triangle(Vertex(i, j), Vertex(i + 1, j), Vertex(i + 1, j + 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i + 1, j + 1), Vertex(i, j + 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i, j + 1), Vertex(i - 1, j)).AreaNormal();
    }
    else if (j == ny - 1)
    {
      // Edge: 3/4/5
      n = Triangle(Vertex(i, j), Vertex(i - 1, j), Vertex(i - 1, j - 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i - 1, j - 1), Vertex(i, j - 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i, j - 1), Vertex(i + 1, j)).AreaNormal();
    }
    else
    {
      // Face: 0/1/2/3/4/5
      n = Triangle(Vertex(i, j), Vertex(i + 1, j), Vertex(i + 1, j + 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i + 1, j + 1), Vertex(i, j + 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i, j + 1), Vertex(i - 1, j)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i - 1, j), Vertex(i - 1, j - 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i - 1, j - 1), Vertex(i, j - 1)).AreaNormal() +
        Triangle(Vertex(i, j), Vertex(i, j - 1), Vertex(i + 1, j)).AreaNormal();
    }
  }
  return Normalized(n);
}

/*!
\brief Compute the vertex corresponding to a given sample.
\param i,j Integer coordinates of the sample.
*/
Vector HeightField::Vertex(int i, int j) const
{
  return Vector(a[0] + i * (b[0] - a[0]) / (nx - 1), a[1] + j * (b[1] - a[1]) / (ny - 1), at(i, j));
}

/*!
\brief Compute the vertex position on the terrain.
\param p Point.
\param triangular Boolean, use triangular interpolation if set to true, bilinear interpolation otherwize.
*/
Vector HeightField::Vertex(const Vector2& p, bool triangular) const
{
  double u, v;
  int i, j;
  CellInteger(p, i, j, u, v);

  double z = 0.0;
  // Escape
  if (!InsideCellIndex(i, j))
  {
  }
  else
  {
    if (triangular)
    {
      if (u > v)
      {
        z = (1.0 - u)*at(i, j) + (u - v)*at(i + 1, j) + v * at(i + 1, j + 1);
      }
      else
      {
        z = (1.0 - v)*at(i, j) + u * at(i + 1, j + 1) + (v - u)*at(i, j + 1);
      }
    }
    else
    {
      z = Math::Bilinear(at(i, j), at(i + 1, j), at(i + 1, j + 1), at(i, j + 1), u, v);
    }
  }
  return Vector(p[0], p[1], z);
}

#include <QtCore/QStack>

/*!
\brief Compute the stream area at a given point in the terrain.

\param x, y Integer position of the point.
*/
double HeightField::StreamArea(int x, int y) const
{
  double stream = 0.0;

  QStack<QPoint> stack;
  QStack<double> weight;

  stack.push(QPoint(x, y));
  weight.push(1.0);
  int e = 0;
  while (!stack.empty())
  {
    QPoint p = stack.pop();
    double w = weight.pop();
    e++;
    stream += w;

    QVector<QPoint> up = Up(p.x(), p.y());
    if (up.size() == 0)
    {
      // cout<<"SORT"<<endl;
    }
    double sum = 0.0;

    for (int i = 0; i < up.size(); i++)
    {
      stack.push(up.at(i));
      if (SlopeFlow(up.at(i), p) < 1e-4)
      {
        cout << "EEE" << endl;
      }
      double ddd = SlopeFlow(up.at(i), p);
      weight.push(w*SlopeFlow(up.at(i), p));
    }
  }
  return stream * CellArea();
}

#include <algorithm>

/*!
\brief Compute the stream area field of the terrain.

\param e Distribution type of the flow over neighboring cells, set to false (default) for standard average, true pour L<SUP>2</SUP> metric.
*/
ScalarField2 HeightField::StreamArea(bool e) const
{
  ScalarField2 stream(Box2(a, b), nx, ny, 1.0);

  QVector<ScalarPoint2> QEE = GetScalarPoints();
  std::sort(QEE.begin(), QEE.end());

  for (int i = QEE.size() - 1; i >= 0; i--)
  {
    QPoint p = QEE.at(i).Point();

    QPoint q[8];
    double h[8];
    double s[8];
    int n = CheckFlowDirectionsUUU(p, q, h, s);
    if (n > 0)
    {
      double ss = 0.0;
      for (int j = 0; j < n; j++)
      {
        // Use average of squared values
        if (e)
        {
          s[j] *= s[j];
        }
        ss += s[j];
      }
      const double sp = stream(p);
      for (int j = 0; j < n; j++)
      {
        stream(q[j]) += sp * s[j] / ss;
      }
    }
  }
  stream *= stream.CellArea();

  return stream;
}

/*!
\brief Compute the stream area field of the terrain.
\sa ()
*/
ScalarField2 HeightField::StreamAreaSteepest() const
{
  ScalarField2 stream(Box2(a, b), nx, ny, 1.0);

  QVector<ScalarPoint2> QEE = GetScalarPoints();
  std::sort(QEE.begin(), QEE.end());

  for (int i = QEE.size() - 1; i >= 0; i--)
  {
    QPoint p = QEE.at(i).Point();

    QPoint q[8];
    double h[8];
    double s[8];
    int n = CheckFlowDirectionsUUU(p, q, h, s);
    if (n > 0)
    {
      double ss = s[0];
      int k = 0;
      for (int j = 1; j < n; j++)
      {
        if (s[j] > ss)
        {
          k = j;
          ss = s[j];
        }
      }
      const double sp = stream(p);
      stream(q[k]) += sp;
    }
  }
  stream *= stream.CellArea();

  return stream;
}

/*!
\brief Compute the flow directions at a given point.

\param a Point.
\param point Array of neighboring points.
\param height Array of height difference
\param slope Array of slopes.
*/
int HeightField::CheckFlowDirectionsUUU(const QPoint& a, QPoint* point, double* height, double* slope) const
{
  int n = 0;

  double za = at(a);
  for (int i = 0; i < 8; i++)
  {
    QPoint b = a + next[i];
    // Skip if point is not inside the domain
    if (!InsideVertexIndex(b.x(), b.y()))
    {
      continue;
    }

    double step = at(b) - za;
    if (step < 0.0)
    {
      point[n] = b;
      height[n] = -step;
      slope[n] = -step / length[i];
      n++;
    }
  }
  return n;
}

/*!
\brief Compute the maximum slope field of the terrain.

Simply compute the norm of the gradient.

This function may be used to perform hill-slope erosion simulation.

\param a Boolean, use average slope if set to true, and slope otherwize.
\sa (), Slope(), AverageSlope(), GradientNorm()
*/
ScalarField2 HeightField::Slope(bool a) const
{
  if (a == true)
  {
    return AverageSlope();
  }
  else
  {
    return GradientNorm();
  }
}

/*!
\brief Compute the average slope field of the terrain.
*/
ScalarField2 HeightField::AverageSlope() const
{
  ScalarField2 slope(Box2(a, b), nx, ny);
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      slope(i, j) = AverageSlope(i, j);
    }
  }
  return slope;
}

/*!
\brief Compute the slope at a given integer point on the terrain.

This is a convenience function corresponding to the following piece of code:
\code
double s=Norm(heightfield.Gradient(i, j));
\endcode
\param i, j Integer coordinates
*/
double HeightField::Slope(int i, int j) const
{
  return Norm(Gradient(i, j));
}

/*!
\brief Compute the average slope at a given integer point on the terrain.

Simply compute the average slope in 8 directions.
\param i, j Integer coordinates
*/
double HeightField::AverageSlope(int i, int j) const
{
  double s;

  Vector2 c = CellDiagonal();
  double e = Norm(c);

  if (i == 0)
  {
    if (j == 0)
    {
      // Corner
      s = fabs(at(i, j) - at(i + 1, j)) / c[0] + fabs(at(i, j) - at(i + 1, j + 1)) / e + fabs(at(i, j) - at(i, j + 1)) / c[1];
      s /= 3;
    }
    else if (j == ny - 1)
    {
      // Corner
      s = fabs(at(i, j) - at(i + 1, j)) / c[0] + fabs(at(i, j) - at(i + 1, j - 1)) / e + fabs(at(i, j) - at(i, j - 1)) / c[1];
      s /= 3.0;
    }
    else
    {
      // Edge
      s = fabs(at(i, j) - at(i, j - 1)) / c[1] + fabs(at(i, j) - at(i + 1, j - 1)) / e + fabs(at(i, j) - at(i + 1, j)) / c[0] + fabs(at(i, j) - at(i + 1, j + 1)) / e + fabs(at(i, j) - at(i, j + 1)) / c[1];
      s /= 5.0;
    }
  }
  else if (i == nx - 1)
  {
    if (j == 0)
    {
      // Corner
      s = fabs(at(i, j) - at(i - 1, j)) / c[0] + fabs(at(i, j) - at(i - 1, j + 1)) / e + fabs(at(i, j) - at(i, j + 1)) / c[1];
      s /= 3.0;
    }
    else if (j == ny - 1)
    {
      // Corner
      s = fabs(at(i, j) - at(i - 1, j)) / c[0] + fabs(at(i, j) - at(i - 1, j - 1)) / e + fabs(at(i, j) - at(i, j - 1)) / c[1];
      s /= 3.0;
    }
    else
    {
      // Edge
      s = fabs(at(i, j) - at(i, j - 1)) / c[1] + fabs(at(i, j) - at(i - 1, j - 1)) / e + fabs(at(i, j) - at(i - 1, j)) / c[0] + fabs(at(i, j) - at(i - 1, j + 1)) / e + fabs(at(i, j) - at(i, j + 1)) / c[1];
      s /= 5.0;
    }
  }
  else
  {
    if (j == 0)
    {
      // Edge
      s = fabs(at(i, j) - at(i - 1, j)) / c[0] + fabs(at(i, j) - at(i - 1, j + 1)) / e + fabs(at(i, j) - at(i, j + 1)) / c[1] + fabs(at(i, j) - at(i + 1, j + 1)) / e + fabs(at(i, j) - at(i + 1, j)) / c[0];
      s /= 5.0;
    }
    else if (j == ny - 1)
    {
      // Edge
      s = fabs(at(i, j) - at(i - 1, j)) / c[0] + fabs(at(i, j) - at(i - 1, j - 1)) / e + fabs(at(i, j) - at(i, j - 1)) / c[1] + fabs(at(i, j) - at(i + 1, j - 1)) / e + fabs(at(i, j) - at(i + 1, j)) / c[0];
      s /= 5.0;
    }
    else
    {
      // Vertex
      s = fabs(at(i, j) - at(i + 1, j)) / c[0] + fabs(at(i, j) - at(i + 1, j + 1)) / e + fabs(at(i, j) - at(i, j + 1)) / c[1] + fabs(at(i, j) - at(i - 1, j + 1)) / e + fabs(at(i, j) - at(i - 1, j)) / c[0] + fabs(at(i, j) - at(i - 1, j - 1)) / e + fabs(at(i, j) - at(i, j - 1)) / c[1] + fabs(at(i, j) - at(i + 1, j - 1)) / e;
      s /= 8.0;
    }
  }
  return s;
}

/*!
\brief Compute the wetness index field of the terrain.

The algorithm works as follows: first compute the stream area
for every terrain sample, and then compute the wetness index.

Note that the wetness index is defined as ln ( A / s ) where
s is the slope, however the definition does not work well
for small slopes, therefore the implementation defines the wetness
index as ln ( A / ( 1+ c s ) )

\param c Coefficent for slope factor, if set to 0 the function will compute the logarithm of the drainage area.
\param a Boolean, use average slope if set to true, and slope otherwize.
\sa (), Slope(), AverageSlope()
*/
ScalarField2 HeightField::WetnessIndex(const double& c, bool a) const
{
  // Initialize wetness index field using stream area
  ScalarField2 wetness = StreamArea();
  ScalarField2 slope = Slope(a);

  // Modify with slope
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      wetness(i, j) = log(wetness(i, j) / (1.0 + c * slope(i, j)));
    }
  }
  return wetness;
}


/*!
\brief Compute the stream power field of the terrain.

The algorithm works as follows: first compute the stream area
for every terrain sample, and then compute the stream power index.

\sa ()
*/
ScalarField2 HeightField::StreamPower() const
{
  // Initialize wetness index field using stream area
  ScalarField2 power = StreamArea();
  ScalarField2 slope = Slope();

  // Modify with slope
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      power(i, j) = sqrt(power(i, j))*slope(i, j);
    }
  }
  return power;
}

/*!
\brief Get the bounding box of the height field.

Note that although this function has the same name as Array2::GetBox(),
it computes the minimum and maximum elevation of the terrain.

\sa Array2::GetBox()
*/
Box HeightField::GetBox() const
{
  double za, zb;
  GetRange(za, zb);
  return Array2::GetBox().ToBox(za, zb);
}

/*!
\brief Compute the intersection between a ray and the surface border of the heightfield.

\param ray The ray.
\param t Distance along the ray.
\param box %Box where intersection will be computed.
\param p Returned intersection point.
\param n Returned normal.
*/
bool HeightField::IntersectBox(const Ray& ray, double& t, Vector& p, Vector& n, const Box& box) const
{
  double ta, tb;
  Vector na, nb;

  // Check the intersection with the bounding box
  if (!box.Intersect(ray, ta, tb, na, nb)) return false;

  t = ta + 0.0001;
  n = na;
  if (ta < 0.0)
  {
    t = tb - 0.0001;
    n = nb;
    return false;
  }

  p = ray(t);
  double z = GetHeight(p);
  double h = p[2] - z;
  if (z > p[2])
  {
    return true;
  }

  return false;
}


/*!
\brief Compute the intersection between a ray and the surface of the heightfield.

The algorithm uses a ray marching approach, therefore this function may require
many iterations and the resulting intersection may not be accurate.

\param ray The ray.
\param t Returned distance along the ray.
\param box %Box where intersection will be computed.
\param q Returned intersection point.
\param k Lipschitz constant.
\param length Maximum distance along the ray.
*/
bool HeightField::Intersect(const Ray& ray, double& t, Vector& q, const Box& box, const double& k, const double& length) const
{
  double ta, tb;

  // Check the intersection with the bounding box
  if (!box.Intersect(ray, ta, tb)) return false;

  if (ta<-1000000000 || tb>+1000000000) return false;

  tb = Math::Min(tb, length);

  t = Math::Max(ta + 0.0001, 0.0);

  // Ray marching
  while (t < tb)
  {
    // Point along the ray
    Vector p = ray(t);

    // Heightfield elevation
    double z = GetHeight(p);
    double h = p[2] - z;
    if (h < 0.0001)
    {
      q = Vector(p[0], p[1], z);
      return true;
    }
    else
    {
      t += Math::Max(h / k, 0.0001);
    }
  }
  return false;
}

/*!
\brief Compute the intersection between a ray and the surface of the heightfield.

The algorithm uses a ray marching approach, therefore this function may require
many iterations and the resulting intersection may not be accurate.

\param ray The ray.
\param t Returned distance along the ray.
\param box %Box where intersection will be computed.
\param q Returned intersection point.
\param k Lipschitz constant.
*/
bool HeightField::Intersect(const Ray& ray, double& t, Vector& q, const Box& box, const double& k) const
{
  double ta, tb;

  // Check the intersection with the bounding box
  if (!box.Intersect(ray, ta, tb)) return false;

  if (ta<-1000000000 || tb>+1000000000) return false;

  t = Math::Max(ta + 0.0001, 0.0);

  // Ray marching
  while (t < tb)
  {
    // Point along the ray
    Vector p = ray(t);

    // Heightfield elevation
    double z = GetHeight(p);
    double h = p[2] - z;
    if (h < 0.0001)
    {
      q = Vector(p[0], p[1], z);
      return true;
    }
    else
    {
      t += Math::Max(h / k, 0.0001);
    }
  }
  return false;
}

/*!
\brief Shade a vector of the terrain.
\param p Terrain coordinates.
\param lights Set of lights.
\param intensity Set of light intensities.
\param sum Sum.
*/
double HeightField::Shade(const Vector& p, const QVector<Vector>& lights, const QVector<double>& intensity, const double& sum) const
{
  Vector q = Vector2(p).ToVector(Value(p) + 0.001);
  Vector n = Normal(p);

  double s = 0.0;
  Box box = GetBox();

  // Maximum distance along the ray
  double length = Norm(box.Diagonal());

  double k = K();

  for (int i = 0; i < lights.size(); i++)
  {
    Ray ray(q, Normalized(lights.at(i) - q));

    // There will be an intersection 
    if (ray.Direction() * n < 0.0) continue;

    Vector qq;
    double tq;
    if (!Intersect(ray, tq, qq, box, k))
    {
      double t = n * ray.Direction();
      t = 0.5*(1 + t);

      s += t * intensity.at(i);
    }
  }
  s = s / sum;
  return s;
}

/*!
\brief Draw a heightfield.
\param scene Graphics scene.
\param pen The pen.
\param brush The brush.
*/
void HeightField::Draw(QGraphicsScene& scene, const QPen& pen, const QBrush& brush) const
{
  double a, b;
  GetRange(a, b);

  QPen invisible = pen;
  invisible.setStyle(Qt::NoPen);

  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      double z = Linear::Step(at(VertexIndex(i, j)), a, b);

      QBrush fill(Color(z, z, z).GetQt());
      Box2 box = Box2(ArrayVertex(i, j) - CellDiagonal(), ArrayVertex(i, j) + CellDiagonal());
      box.Draw(scene, invisible, fill);
    }
  }
}

/*!
\brief Scale the height field.

The domain is scaled using the x and y components of the scaling vector,
whereas the heights of the heightfield are scaled using the z component.
\param s Scaling factor.
*/
void HeightField::Scale(const Vector& s)
{
  // Box
  ScalarField2::Scale(Vector2(s));

  // Heights
  for (int i = 0; i < field.size(); i++)
  {
    field[i] *= s[2];
  }
}

/*!
\brief Scale the heightfield so that the compact support should fit within unit box.

Centers the terrain and scales it.
*/
void HeightField::Unity()
{
  Translate(-Box2::Center());

  // Get largest dimension
  double s = NormInfinity(0.5*Box2::Diagonal());

  Scale(Vector(1.0 / s));
}

/*!
\brief Compute the cells that are uphill of input point.

\param x, y Integer coordinates of the point.
*/
QVector<QPoint> HeightField::Up(int x, int y) const
{
  QPoint p = QPoint(x, y);
  // Elevation
  double z = at(p);

  QVector<QPoint> up;

  for (int i = 0; i < 8; i++)
  {
    QPoint q = p + next[i];
    if (InsideVertexIndex(q.x(), q.y()))
    {
      if (at(q) > z + 0.0001)
      {
        up.append(q);
      }
    }
  }
  return up;
}

/*!
\brief Compute the cells that are uphill of input point.

\param x, y Integer coordinates of the point.
*/
QVector<QPoint> HeightField::Down(int x, int y) const
{
  QPoint p = QPoint(x, y);
  // Elevation
  double z = at(p);

  QVector<QPoint> down;

  for (int i = 0; i < 8; i++)
  {
    QPoint q = p + next[i];
    if (InsideVertexIndex(q.x(), q.y()))
    {
      if (at(q) > z)
      {
        down.append(q);
      }
    }
  }
  return down;
}

/*!
\brief Compute the realtive slope flow between two points.
\param a Origin.
\param b Target point, should be in the 1-neighborhood.
*/
double HeightField::SlopeFlow(const QPoint& a, const QPoint& b) const
{
  double zab = at(b) - at(a);
  if (zab > 0.0001)
  {
    return 0.0;
  }
  QPoint ab = b - a;
  double sab = zab / Norm(Vector2(ab.x(), ab.y()));

  double s = 0.0;

  for (int i = 0; i < 8; i++)
  {
    QPoint q = a + next[i];

    // Skip if point is not inside the domain
    if (!InsideVertexIndex(q.x(), q.y()))
      continue;

    double step = at(q) - at(a);
    if (step < 0.0)
    {
      s += -step / length[i];
    }
  }
  return -sab / s;
}

/*!
\brief Create the geometry of the border of the terrain.

\param name The name of the object.
\param z The height of the base of the block.
*/
MayaGeometry HeightField::CreateHeightFieldBorder(const QString& name, const double& z) const
{
  int n = 2 * nx + 2 * ny - 4;

  QVector<Vector> vertex;
  vertex.reserve(2 * n);
  QVector<Vector> normal;
  normal.resize(4);

  QVector<MayaIndexVertexData> indexes;
  indexes.reserve(6 * 2 * n);

  // Base contour
  for (int i = 0; i < nx - 1; i++)
  {
    Vector p = Vertex(i, 0);
    Vector q = p;
    p[2] = z;
    vertex.append(p);
    vertex.append(q);
  }

  for (int i = 0; i < ny - 1; i++)
  {
    Vector p = Vertex(nx - 1, i);
    Vector q = p;
    p[2] = z;
    vertex.append(p);
    vertex.append(q);
  }

  for (int i = nx - 1; i > 0; i--)
  {
    Vector p = Vertex(i, ny - 1);
    Vector q = p;
    p[2] = z;
    vertex.append(p);
    vertex.append(q);
  }

  for (int i = ny - 1; i > 0; i--)
  {
    Vector p = Vertex(0, i);
    Vector q = p;
    p[2] = z;
    vertex.append(p);
    vertex.append(q);
  }

  // Compute normals
  normal[0] = Vector(0.0, 1.0, 0.0);
  normal[1] = Vector(-1.0, 0.0, 0.0);
  normal[2] = Vector(0.0, -1.0, 0.0);
  normal[3] = Vector(1.0, 0.0, 0.0);

  // Compute indexes : loop over all vertices
  for (int i = 0; i < 2 * n; i += 2)
  {
    int side = 3;
    if ((i / 2 >= 0) && (i / 2 < nx - 1)) { side = 0; }
    else if ((i / 2 >= nx - 1) && (i / 2 < nx + ny - 2)) { side = 1; }
    else if ((i / 2 >= nx + ny - 2) && (i / 2 < 2 * nx + ny - 3)) { side = 2; }

    MayaIndexVertexData mi1(i, side);
    MayaIndexVertexData mi2(i + 1, side);
    MayaIndexVertexData mi3((i + 3) % (2 * n), side);
    MayaIndexVertexData mi4((i + 2) % (2 * n), side);
    //Triangle 1
    indexes.append(mi1);
    indexes.append(mi2);
    indexes.append(mi3);

    //Triangle 2
    indexes.append(mi1);
    indexes.append(mi3);
    indexes.append(mi4);
  }

  MayaMaterial mo = MayaMaterial(ShaderGooch, Color(0.3, 0.3, 0.3, 1.0), Color(0.5, 0.4, 0.2, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);
  MayaGeometry mg(name, vertex, normal, indexes, mo);

  return mg;
}


/*!
\brief Create the geometry of the heightfield.

\param name The name of the object.
*/
MayaGeometry HeightField::CreateHeightField(const QString& name) const
{
  QVector<Vector> vertex;
  vertex.resize(nx*ny);
  QVector<Vector> normal;
  normal.resize(nx*ny);
  QVector<Vector> color;
  color.resize(nx*ny);
  QVector<MayaIndexVertexData> indexes;
  indexes.reserve(12 * (nx - 1)*(ny - 1));

  // Height of vertices
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      vertex[i*ny + j] = Vertex(i, j);
    }
  }

  // Compute normals
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      normal[i*ny + j] = Normal(i, j);
    }
  }

  // Compute VertexColor
  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      //color[i*ny+j]=Color(0.5,0.5,0.5);
    }
  }

  // Compute indexes
  for (int i = 0; i < nx - 1; i++)
  {
    for (int j = 0; j < ny - 1; j++)
    {
      MayaIndexVertexData mi1(i*ny + j, i*ny + j, i*ny + j);
      MayaIndexVertexData mi2((i + 1)*ny + j, (i + 1)*ny + j, (i + 1)*ny + j);
      MayaIndexVertexData mi3((i + 1)*ny + j + 1, (i + 1)*ny + j + 1, (i + 1)*ny + j + 1);
      MayaIndexVertexData mi4(i*ny + j + 1, i*ny + j + 1, i*ny + j + 1);
      //Triangle 1
      indexes.append(mi1);
      indexes.append(mi2);
      indexes.append(mi3);

      //Triangle 2
      indexes.append(mi1);
      indexes.append(mi3);
      indexes.append(mi4);
    }
  }

  MayaMaterial mo = MayaMaterial::Normal;// MayaMaterial(ShaderNormalWireframe, Color(0.3, 0.3, 0.3, 1.0), Color(0.5, 0.4, 0.2, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);
  MayaGeometry mg(name, vertex, normal, color, indexes, mo);

  return mg;
}

#include "hemisphere.h"

/*!
\brief Compute the accessibility.

\param r Radius.
\param n Number of rays.
*/
ScalarField2 HeightField::Accessibility(const double& r, int n) const
{
  Random random;

  double epsilon = 0.01f;

  // Lipschitz
  double k = K();

  Box box = GetBox();

  ScalarField2 a(Box2(a, b), nx, ny, 1.0);

  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      Vector p = Vertex(i, j) + Vector(0.0, 0.0, epsilon);
      Vector normal = Normal(i, j);

      int hit = 0;
      for (int k = 0; k < n; k++)
      {
        Vector direction = HemiSphere::RandomDirection(normal, random);
        Ray ray = Ray(p, direction);
        double t;
        Vector q;
        if (Intersect(ray, t, q, box, k, r))
        {
          hit++;
        }
      }
      a(i, j) = 1.0 - double(hit) / double(n);
    }
  }
  return a;
}

/*!
\brief Compute the direct lighting.

\param u Light direction.
*/
ScalarField2 HeightField::Light(const Vector& u) const
{
  ScalarField2 a(Box2(a, b), nx, ny, 1.0);

  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      double light = Normal(i, j) * u;
      light = 0.5*(1 + light);

      a(i, j) = light;
    }
  }
  return a;
}
