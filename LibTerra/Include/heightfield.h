#ifndef __HeightField__
#define __HeightField__

#include <QtCore/QString>

#include "scalarfield.h"
#include "color.h"
#include "mayageometry.h"

class HeightField :public ScalarField2
{
protected:
public:
  HeightField(const Box2&, const QImage&, const double&, const double&, bool = true);
  HeightField(const Box2&, int, int, const double& = 0.0);
  HeightField(const Box2&, int, int, const QVector<double>&);
  HeightField(const ScalarField2&);

  //! Empty
  HeightField() {}
  //! Empty
  ~HeightField() {}

  double GetHeight(const Vector2&) const;

  // Vertices
  Vector Vertex(const Vector2&, bool = true) const;
  Vector Vertex(int, int) const;

  // Normals and slope
  Vector Normal(const Vector2&, bool = true) const;
  Vector Normal(int, int) const;
  double Slope(int, int) const;
  double AverageSlope(int, int) const;

  void Subdivide(const double&, Random& = Random::R239);

  double StreamArea(int, int) const;

  ScalarField2 StreamArea(bool = false) const;
  ScalarField2 StreamAreaSteepest() const;

  ScalarField2 AverageSlope() const;
  ScalarField2 Slope(bool = false) const;

  ScalarField2 WetnessIndex(const double& = 1.0, bool = false) const;
  ScalarField2 StreamPower() const;
  ScalarField2 Accessibility(const double&, int = 16) const;
  ScalarField2 Light(const Vector&) const;

  int FillDepressions();
  int FillDepressions(const double&);
  int FillDepressions(ScalarField2&) const;
  int FillDepressions(const double&, ScalarField2&) const;

  void CompleteBreach();

  // Ray intersection
  bool Intersect(const Ray&, double&, Vector&, const Box&, const double&, const double&) const;
  bool Intersect(const Ray&, double&, Vector&, const Box&, const double&) const;
  bool IntersectBox(const Ray&, double&, Vector&, Vector&, const Box&) const;

  double Shade(const Vector&, const QVector<Vector>&, const QVector<double>&, const double&) const;

  // Box
  Box GetBox() const;

  void Scale(const Vector&);
  void Unity();

  void Draw(QGraphicsScene&, const QPen& = QPen(), const QBrush& = QBrush()) const;

  QImage ImageShadeNormal(const QColor&) const;
  QImage ImageShadeSkyRadianceScaling(const QColor&) const;
  QImage ImageShadeMitsuba() const;
  QImage ImageShadeSky(const QColor&) const;
  QImage TextureIsoLines(const QColor& = QColor(80, 60, 60), const QColor& = QColor(40, 20, 20), const double& height = 100.0, int n = 4) const;
  QImage ImageShadeRadianceScaling(const Color& = Color(1.0, 1.0, 1.0), const Color& = Color(142.0 / 255.0, 92.0 / 255.0, 31.0 / 255.0)) const;

  ScalarField2 RadianceScaling(const double & pos, const double & factor) const;

  // Shading
  Color ShadeRelief(const double&, const double & = 0.0, const double & = 300.0) const;

  QVector<QPoint> Up(int, int) const;
  QVector<QPoint> Down(int, int) const;

  // Flow
  double SlopeFlow(const QPoint&, const QPoint&) const;
  int CheckFlowDirectionsUUU(const QPoint&, QPoint*, double*, double*) const;

  MayaGeometry CreateHeightField(const QString& = QString("HeightField")) const;
  MayaGeometry CreateHeightFieldBorder(const QString& = QString("HeightFieldSide"), const double& = 0.0) const;
};

/*!
\brief Compute the height at a given position.
\param p Point.
*/
inline double HeightField::GetHeight(const Vector2& p) const
{
  return Value(p);
}

class AnalyticField2 {
protected:
public:
  //! Empty.
  AnalyticField2() {}
  virtual double Height(const Vector2&) const = 0;
  virtual Vector2 Gradient(const Vector2&) const;
  virtual Vector Normal(const Vector2&) const;
  HeightField CreateHeightField(const Box2&, int, int) const;

  // Slope
  double Slope(const Vector2&) const;
  double AverageSlope(const Vector2&, int = 8) const;
  double Slope(const Vector2&, const Vector2&) const;

protected:
  double Ridge(const double&, const double&) const;
  double MusgraveRidge(const double&) const;
protected:
  static double Epsilon; //!< Epsilon value used for gradient computation.
};

class HeightFieldNext8 :public Array2
{
protected:
  QVector<unsigned char> flow; //!< Array of directions
public:
  HeightFieldNext8(const HeightField&, bool);
  //! Empty
  HeightFieldNext8() {}
  //! Empty
  ~HeightFieldNext8() {}

  unsigned char At(int, int) const;
  unsigned char At(const QPoint&) const;

  QPoint FlowTo(const QPoint&) const;
  QPoint FlowTo(const QPoint&, int) const;
  int N(int, int) const;
  int N(const QPoint&) const;
protected:
  unsigned char Flow(const HeightField&, const QPoint&, bool) const;
};

/*!
\brief Return the flowing directions, compacted into one byte.
\param i,j Integer coordinates of the sample.
*/
inline unsigned char HeightFieldNext8::At(int i, int j) const
{
  return flow.at(VertexIndex(i, j));
}

/*!
\brief Return the flowing directions, compacted into one byte.
\param p Point.
*/
inline unsigned char HeightFieldNext8::At(const QPoint& p) const
{
  return flow.at(VertexIndex(p));
}

#endif
