#ifndef __ArchesTree__
#define __ArchesTree__

#include <QtGui/QImage>

#include "polygon.h"
#include "turbulence.h"
#include "maya.h"
#include "layers.h"
#include "random.h"
#include "heightfield.h"

#include "fieldtree.h"

class TerraNode;
class VegetationNode;

class ArchesTree {
protected:
  TerraNode * bedrock; //!< Bedrock tree.
  TerraNode* water; //!< Water.
  TerraNode* foam;
  TerraNode* sand;
  VegetationNode* veg_root;
public:
  ArchesTree(TerraNode*, TerraNode* = nullptr, TerraNode* = nullptr, TerraNode* = nullptr, VegetationNode* = nullptr);
  ~ArchesTree();

  // Height
  ScalarAlpha GetBedrock(const Vector2&) const;
  ScalarAlpha GetWater(const Vector2&) const;
  ScalarAlpha GetFoam(const Vector2&) const;
  ScalarAlpha GetSand(const Vector2&) const;

  // Setters
  //void setTime(double time) { water->setTime(time); foam->setTime(time);}

  // Get the box of the tree
  Box2 GetBox() const;

  // Color
  Vector GetColor(const Vector2&) const;

  int Traversal(const Vector2&) const;

  // Vegetation
  void generateVegetationNode(const Box2&);
  void generateVegetationNodeCanyon(const Box2&);

  int GetInstancesTree(Vector& p, Random& ra) const;
  int GetInstancesSmallVeg(Vector& p, Random& ra) const;
  MayaInstanceAll GetVegetation(const Box2&, bool) const;

  // Tools for 2D Mesher 
  void GetTriangles(Vector a, Vector b, Vector c, QVector<Vector> &mgTerrain, QVector<Vector> &triangleWater) const;
  Vector dichotomie(Vector a, Vector b, double va, double vb) const;

  // Mesher 2D 
  MayaGeometryAll createMeshMaterial(const Box2& box, int nb_div_x, int nb_div_y) const;
  MayaGeometry createMeshCircle(const Circle2&, int) const;
  MayaGeometry createMeshCircleBorder(const Circle2& circle, int nb_sub) const;

  void HeightFieldRock(HeightField &hf);
  void HeightFieldWater(HeightField &hf);
  void HeightFieldSand(HeightField &hf);

  LayerStack CreateLayerStack(const Box2&, int) const;
  LayerStack CreateLayerStackRectangular(const Box2&, int) const;

  MayaGeometryAll createMeshMaterialAndTexture(const Box2& box, int nb_div_x, int nb_div_y) const;
  QImage createImageFoam(const Box2& box, int nb_div_x, int nb_div_y) const;

  bool saveJson(QString);
  void writeJson(QJsonObject &);
protected:
  static const double epsilon; //!< Epsilon value for gradient computations.
};

// Generic node
class TerraNode {
protected:
  Box2 box666; //!< Bounding box.
public:
  //! Empty.
  TerraNode() {}
  TerraNode(const Box2&);
  virtual Box2 GetBox() const;
  virtual void Draw(QGraphicsScene&) const;

  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual double Elevation(const Vector2&) const;
  virtual Vector2 Gradient(const Vector2&) const;
  virtual Vector Normal(const Vector2&) const;

  virtual void writeJson(QJsonObject&);
  virtual int Traversal(const Vector2&) const;
  virtual int Memory() const;

  //virtual void setTime(double);

protected:
  static const double epsilon; //!< Epsilon value for gradient computations, set to 1cm.
};

class TerraBinary :public TerraNode {
protected:
  TerraNode * a; //!< Left sub-tree.
  TerraNode * b; //!< Right sub-tree.
public:
  TerraBinary(TerraNode*, TerraNode*);
  ~TerraBinary();
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const = 0;
  virtual int Traversal(const Vector2&) const;
  virtual int Memory() const;
};


class TerraUnary :public TerraNode {
protected:
  TerraNode * node; //!< Sub-tree.
public:
  TerraUnary(TerraNode*);
  ~TerraUnary();
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const = 0;
  virtual int Traversal(const Vector2&) const;
  virtual int Memory() const;

  static TerraNode* PutAt(TerraNode*, const Vector&, const double&, const Vector2&);
  static TerraNode* PutAt(TerraNode*, const double&, const Vector2&);
};

class TerraRotate : public TerraUnary
{
protected:
  Vector2 c; //!< Rotation center.
  Matrix2 r; //!< Rotation matrix.
public:
  TerraRotate(TerraNode*, const double&, const Vector2& = Vector2(0.0));
  ~TerraRotate();

  ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class TerraTaper : public TerraUnary
{
protected:
  AnalyticFieldNode * a; //!< Tapering field.
public:
  TerraTaper(TerraNode*, AnalyticFieldNode*);
  ~TerraTaper();

  ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class TerraOperator :public TerraNode {
protected:
  QVector<TerraNode*> nodes; //!< Array of nodes that will be combined
public:
  TerraOperator(const QVector<TerraNode*>&);
  TerraOperator(TerraNode*, TerraNode* = nullptr, TerraNode* = nullptr, TerraNode* = nullptr);
  ~TerraOperator();
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const = 0;
  virtual int Memory() const;
};

class TerraPrimitive :public TerraNode {
protected:
public:
  TerraPrimitive();
  ~TerraPrimitive();
};

class TerraGrid :public TerraNode, Array2 {
protected:
  QVector<TerraNode*> gridNodes; //!< Array of nodes that will be combined
  int neighbour;

public:
  TerraGrid(Box2 bb = Box2(), int nxt = 10, int nyt = 10, int neighbourt = 1);
  ~TerraGrid();

  void append(Vector2, TerraNode*);
  void append(int, int, TerraNode*);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
};

class TerraBlend :public TerraOperator {
protected:
public:
  TerraBlend(QVector<TerraNode*>);
  TerraBlend(TerraNode*, TerraNode*, TerraNode* = nullptr, TerraNode* = nullptr);
  //! Empty
  ~TerraBlend() {}
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual void Draw(QGraphicsScene&) const;
  virtual int Traversal(const Vector2&) const;

  virtual void writeJson(QJsonObject&);
};

class TerraBlendGrid :public TerraBlend {
protected:
  QVector<QVector<int> > grid; //!< Array of nodes.
  int nx;
  int ny;
public:
  TerraBlendGrid(QVector<TerraNode*>, int);
  ~TerraBlendGrid();
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
};

class TerraUnion :public TerraOperator {
protected:
public:
  TerraUnion(TerraNode*, TerraNode*);
  TerraUnion(QVector<TerraNode*>);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
};

class TerraLowest :public TerraBinary {
protected:
public:
  TerraLowest(TerraNode*, TerraNode*);
  ~TerraLowest();
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
};

class TerraAdd :public TerraBinary {
protected:
public:
  TerraAdd(TerraNode*, TerraNode*);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
};

class TerraSub :public TerraBinary {
protected:
public:
  TerraSub(TerraNode*, TerraNode*);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
};

class TerraReplace :public TerraBinary {
protected:
public:
  TerraReplace(TerraNode*, TerraNode*);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  static TerraNode* ReplaceArray(const QVector<TerraNode*>&);
};

class Terra2015Replace :public TerraBinary {
protected:
public:
  Terra2015Replace(TerraNode*, TerraNode*);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  static TerraNode* ReplaceArray(const QVector<TerraNode*>&);
};

class TerraPlateau :public TerraUnary {
protected:
  double a, b; //!< Elevations.
  Vector2 c; //!< Center.
  double r; //!< Radius.
  double flat; //!< Flattening coefficient.
public:
  TerraPlateau(TerraNode*, const Vector2&, const double&, const double&, const double&, const double& = 0.25);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  void Draw(QGraphicsScene&) const;
};

class TerraSkeleton :public TerraPrimitive {
protected:
  double r; // Radius.
  double alpha; //!< Amplitude weight.
public:
  TerraSkeleton(const double&, const double&);
  double Alpha(const double&) const;
};

class TerraVertex :public TerraSkeleton {
protected:
  Vector c;     //!< Center, includes elevation.
public:
  TerraVertex(const Vector&, const double&, const double& = 1.0);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
  virtual void Draw(QGraphicsScene&) const;
};


class TerraDisc :public TerraSkeleton {
protected:
  Disc2 disc; //!< Disc.
  double z;     //!< Elevation.
public:
  TerraDisc(const Disc2&, const double&, const double&, const double& = 1.0);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
  void Draw(QGraphicsScene&) const;
};

class TerraVertexSlope :public TerraVertex {
protected:
  double sx,sy; //!< Slope along x and y directions.
public:
  TerraVertexSlope(const Vector&, const Vector&, const double&, const double&);
  TerraVertexSlope(const Vector&, const double&, const double&, const double&, const double&);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class Terra2013VertexSlopeNoise :public TerraVertexSlope {
protected:
  double s; //!< Scaling of the noise.
  double na; //!< Amplitude of the noise.
public:
  Terra2013VertexSlopeNoise(const Vector&, const Vector&, const double&, const double&, const double&, const double&);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class TerraVertexNoise :public TerraVertex {
protected:
  double a; //!< Amplitude of the noise.
  double lambda; //!< Fundamental wavelength.
  double t0; //!< Reference noise at the center, internal parameter to speedup computations.
public:
  TerraVertexNoise(const Vector&, const double&, const double&, const double&, const double& = 1.0);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
private:
  static SimplexTurbulence turbulence; //!< %Turbulence.
};

class TerraVertexNoiseRidge :public TerraVertex {
protected:
  double lacunarity;
  double H;
  int oc;
  double off;
  double gain;
  double scalefactor;
  double a;
public:
  TerraVertexNoiseRidge(const Vector&, double, double, int, double, double, double, double, const double&, const double& = 1.0);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class TerraSegmentNoise :public TerraSkeleton, public Segment2 {
protected:
  double a; //!< Amplitude of the noise.
  double lambda; //!< Fundamental wavelength.
  double t0; //!< Reference noise at the center, internal parameter to speedup computations.
public:
  TerraSegmentNoise(const Segment2&, const double&, const double&, const double&, const double&);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
private:
  static SimplexTurbulence turbulence; //!< %Turbulence.
};

class TerraBox :public TerraPrimitive {
protected:
  Box2 box; //!< Skeleton.
  double z; //!< Constant elevation.
  double alpha;
  double r;
public:
  TerraBox(const Box2&, const double&, const double&, const double&);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class TerraImages :virtual public TerraPrimitive {
protected:
  int reference; //!< Index storing the reference to the field.
  double beta; //!< Importance sampling.
public:
  TerraImages(const QString&, const double& = 0.5);
  TerraImages(const QString&, const QString&, const double& = 0.5);
  TerraImages(const QString&, const QString&, const QString&, const double& = 0.5);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const = 0;
public:
  static void StoreImage(const QImage&, const QString&);
  static void StoreImage(const QImage&, const QImage&, const QString&);
  static void StoreImage(const QString&, const QString&);
  static void StoreImage(const QString&, const QString&, const QString&);
protected:
  static bool ExistsInStorage(const QString&);
  static int IndexInStorage(const QString&);
  static QVector<ScalarField2> images; //!< Set of images that will be shared by the instances of TerraImages.
  static QVector<QString> aliases; //!< Identifier as names of the images.

protected:
  double At(int, int) const;
  double AtW(int, int) const;
  const ScalarField2& GetImage(int) const;
};

/*!
\brief Get the reference to the k-th image in the atlas.

This internal function is only provided to simplifiy notations in the code.

\param k Index.
*/
inline const ScalarField2& TerraImages::GetImage(int k) const
{
  return images.at(k);
}

/*!
\brief Return the height of the terrain at a given integer position.
\param i, j Point location.
*/
inline double TerraImages::At(int i, int j) const
{
  return GetImage(reference).Value(i, j);
}

/*!
\brief Return the weight at a given integer position.
\param i, j Point location.
*/
inline double TerraImages::AtW(int i, int j) const
{
  return GetImage(reference).Value(i, j);
}

class TerraBoxImage :public TerraImages {
protected:
  Vector a, b; //!< Location of the primitive
public:
  TerraBoxImage(const QString&, const QString&, const Vector&, const Vector&);
  TerraBoxImage(const QString&, const QString&, const QString&, const Vector&, const Vector&);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class TerraQuadrangleImage :public TerraImages {
protected:
  Quadrangle q; //!< Location of quad where the image maps.
  double za, zb, zc, zd; //!< Scaling of the elevation at the corners.
public:
  TerraQuadrangleImage(const QString&, const QString&, const Quadrangle&, const double&, const double&, const double&, const double&, const double& = 0.5);
  TerraQuadrangleImage(const QString&, const QString&, const QString&, const Quadrangle&, const double&, const double&, const double&, const double&, const double& = 0.5);
  TerraQuadrangleImage(const QString&, const Quadrangle&, const double&, const double&, const double&, const double&, const double& = 0.5);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class TerraCubic :virtual public TerraPrimitive {
protected:
public:
  Vector a, b; //!< End vertices of the curve.
  Cubic x, y, z; //!< Cubic curve in the plane.
  Quartic xyz; //!< Quartic constant coefficients used to speed-up ray-tracing.
public:
  TerraCubic(const Vector&, const Vector&, const Vector&, const Vector&);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const = 0;
  virtual int Memory() const;
};

class TerraDune :public TerraCubic {
protected:
  double height;
  double size_interior;
  double size_exterior;
public:
  TerraDune(const Vector&, const Vector&, const Vector&, const Vector&, double h, double si, double se);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};


class TerraCrater : public TerraPrimitive
{
protected:
  Vector2 m_center; //!< Center.
  double m_radius; //!< Radius.
  double m_radius_external; //!< Radius of a point based footprint (outside == not defined)
  double m_depth;
  double m_uplift;

  AnalyticFieldDisc m_f_weight;
  AnalyticFieldFractal m_noise_distanceQXXQ;
  AnalyticFieldFractal m_noiseQXXQ;
public:
  TerraCrater(const Vector2&, const double&, const double&, const double&, const double&);
  ~TerraCrater();

  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
protected:
  double XValue(const Vector2&) const;
  double ProfileCrater(const double&) const;
  double NoiseCrater(const double&) const;
};

class TerraCubicImage :public TerraCubic, public TerraImages {
protected:
  double r1, r2; //!< Offset radius.
  double dz1, dz2;

public:
  TerraCubicImage(const Vector&, const Vector&, const Vector&, const Vector&, const double&, const double&, const QString&, const QString&, const QString&, const double& = 0.5);
  TerraCubicImage(const Vector&, const Vector&, const Vector&, const Vector&, const double&, const double&, const QString&, const double& = 0.5);
  TerraCubicImage(const Vector&, const Vector&, const Vector&, const Vector&, const double&, const double&, const double&, const double&, const QString&, const QString&, const QString&, const double& = 0.5);
  TerraCubicImage(const Vector&, const Vector&, const Vector&, const Vector&, const double&, const double&, const double&, const double&, const QString&, const double& = 0.5);
  virtual ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;

protected:
  double GetHeightFromImage(const double&, const double&) const;
  double GetWeightFromImage(const double&, const double&) const;
};

class VegetationSample
{
protected:
  double tree1; //!< Tree1 density.
  double tree2; //!< Tree2 density.
  double tree3; //!< Tree3 density.
public:
  //! Empty
  VegetationSample() {}
  VegetationSample(const double&, const double& = 0, const double& = 0);
  //! Empty
  ~VegetationSample() {}
  double Tree() const;     //!< Returns the Tree density (tree1 + tree2 + tree3)
  double Tree1() const;		//!< Returns the Tree1 density
  double Tree2() const;		//!< Returns the Tree2 density
  double Tree3() const;		//!< Returns the Tree3 density
};
inline VegetationSample::VegetationSample(const double& t1, const double& t2, const double& t3)
{
  tree1 = t1;
  tree2 = t2;
  tree3 = t3;
}
inline double VegetationSample::Tree() const
{
  return tree1 + tree2 + tree3;
}
inline double VegetationSample::Tree1() const
{
  return tree1;
}
inline double VegetationSample::Tree2() const
{
  return tree2;
}
inline double VegetationSample::Tree3() const
{
  return tree3;
}

class VegetationSampleAlpha :public VegetationSample
{
protected:
  double alpha; //!< Alpha coefficient defining how Vegetation should be combined or blended.
  double importance;  //!< Importance, which is used for importance sampling of the terrain.
public:
  //! Empty
  VegetationSampleAlpha() {}
  VegetationSampleAlpha(const double&, const double&, const double&, const double& = 0, const double& = 0);
  //! Empty
  ~VegetationSampleAlpha() {}
  double Alpha() const;
  double Importance() const;
};
inline VegetationSampleAlpha::VegetationSampleAlpha(const double& a, const double& i, const double& t1, const double& t2, const double& t3) :VegetationSample(t1, t2, t3)
{
  alpha = a;
  importance = i;
}
inline double VegetationSampleAlpha::Alpha() const
{
  return alpha;
}
inline double VegetationSampleAlpha::Importance() const
{
  return importance;
}

// Generic node
class VegetationNode {
protected:
  Box2 box888; //!< Bounding box
public:
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const = 0; //!< Returns the weight and height at a given position, pure virtual method
  virtual Box2 GetBox() const;
};


class VegetationAll :public VegetationNode {
protected:
  QVector<VegetationNode*> nodes; //!< Array of nodes that will be combined
public:
  VegetationAll(QVector<VegetationNode*>);
  VegetationAll(VegetationNode*, VegetationNode*);
  ~VegetationAll();
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const = 0;
};

class VegetationBlend :public VegetationAll {
protected:
public:
  VegetationBlend(QVector<VegetationNode*>);
  VegetationBlend(VegetationNode*, VegetationNode*);
  ~VegetationBlend();
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const;
};

class VegetationMax :public VegetationAll {
protected:
public:
  VegetationMax(QVector<VegetationNode*>);
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const;
};

class VegetationAdd :public VegetationAll {
protected:
public:
  VegetationAdd(VegetationNode*, VegetationNode*);
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const;
};

class VegetationReplace :public VegetationAll {
protected:
public:
  VegetationReplace(VegetationNode*, VegetationNode*);
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const;
};

class VegetationCompact :public VegetationNode {
protected:
public:
  double r; //!< Radius of the disc.
  double alpha; //!< Amplitude weight.
public:
  VegetationCompact(const double&, const double& = 1.0);
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const = 0;
protected:
  double Value(const double&) const;
};

class VegetationDisc :public VegetationCompact {
protected:
public:
  Vector2 c; //!< Center
  double tree1; //!< Tree1 Density
  double tree2; //!< Tree2 Density
  double tree3; //!< Tree3 Density
public:
  VegetationDisc(const Vector2&, const double&, const double&, const double&, const double& = 0, const double& = 0);
  virtual VegetationSampleAlpha WeightDensity(const Vector&) const;
};

class TerraScale : public TerraUnary
{
protected:
  Vector s; //!< Scaling vector.
public:
  TerraScale(TerraNode*, const Vector&);
  ~TerraScale();

  ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};


class TerraTranslate : public TerraUnary
{
protected:
  Vector2 t; //!< Translation vector.
public:
  TerraTranslate(TerraNode*, const Vector2&);
  ~TerraTranslate();

  ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

#include "fieldtree.h"
class TerraWarpNoise :public TerraUnary
{
protected:
  AnalyticFieldFractal warpQXXQ; //!< Warping.   ERIC TODO SEARCH / REMOVE ALL :: QXXQ -> rien
  Vector2 c; //!< Center.
public:
  TerraWarpNoise(TerraNode*, const Vector2&, const double&, const AnalyticFieldFractal&);
  ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

#include "twist.h"

class TerraTwist :public TerraUnary
{
protected:
  Twist2 twist; //!< Warping.
public:
  TerraTwist(TerraNode*, const Twist2&);
  ScalarAlpha ElevationAlpha(const Vector2&) const;
  virtual int Memory() const;
};

class Terra2015ReplaceAlpha : public TerraBinary
{
protected:
public:
  Terra2015ReplaceAlpha(TerraNode*, TerraNode*);
  ~Terra2015ReplaceAlpha();

  ScalarAlpha ElevationAlpha(const Vector2&) const;

private:
  ScalarAlpha WeightedReplace(const ScalarAlpha&, const ScalarAlpha&) const;
};

class Terra2015ReplaceMaxAlpha : public TerraBinary
{
protected:
public:
  Terra2015ReplaceMaxAlpha(TerraNode*, TerraNode*);
  ~Terra2015ReplaceMaxAlpha();

  ScalarAlpha ElevationAlpha(const Vector2&) const;

private:
  ScalarAlpha WeightedReplace(const ScalarAlpha&, const ScalarAlpha&) const;
};

#endif

