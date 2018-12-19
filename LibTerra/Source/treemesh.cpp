#include "archestree.h"
#include "delaunay.h"

QVector<int> oldMatchBetween(QVector<int> vec_int, QVector<int> vec_ext)
{
  int nb_seg_ext = vec_ext.size() - 1;
  int nb_seg_int = vec_int.size() - 1;

  if (nb_seg_ext <= 0 || nb_seg_int <= 0)
    return QVector<int>();

  int d = 0;
  int step_ext = 1.0 * nb_seg_int; // fraction is simplified
  int step_int = 1.0 * nb_seg_ext; // fraction is simplified

  int last_ext = 0;
  int last_int = 0;

  QVector<int> vec_index;
  while (!(last_ext == nb_seg_ext && last_int == nb_seg_int))
  {
    if (abs(d + step_ext) < abs(d - step_int)) // move fwd on line EXT
    {
      d += step_ext;
      vec_index.append(vec_int[last_int]);
      vec_index.append(vec_ext[last_ext + 1]);
      vec_index.append(vec_ext[last_ext]);
      last_ext++;
    }
    else // move fwd on line INT
    {
      d -= step_int;
      vec_index.append(vec_int[last_int]);
      vec_index.append(vec_int[last_int + 1]);
      vec_index.append(vec_ext[last_ext]);
      last_int++;
    }
  }
  return vec_index;
}

//
int oldGetResolution(Camera c, double gridBlockSize, double TerrainHeight, Vector2 center)
{
  int resMin = 32;//Modifier resolution export du bloc Min
  int resMax = 512;//Modifier resolution export du bloc Max
  double MaxCameraHeight = 20000.0;
  double MinCameraHeight = 10.0;

  double Mindist = 1000.0;
  double Maxdist = 20000.0;

  int res1, res2;
  double alpha1, alpha2;

  Vector2 eye = Vector2(c.Eye()) - gridBlockSize * Normalized(Vector2(c.At()) - Vector2(c.Eye()));

  Vector2 view = Normalized(Vector2(c.At()) - eye);
  Vector2 vect = Normalized(center - eye);

  // Prise en compte de la hauteur
  double height = c.Eye()[2] - TerrainHeight;
  if (height < MinCameraHeight) alpha1 = 1.0;
  else if (height > MaxCameraHeight) alpha1 = 0.0;
  else                             alpha1 = 1.0 - (height / (MaxCameraHeight - MinCameraHeight));
  res1 = resMin + (resMax - resMin)*alpha1;

  // Prise en compte de l'angle de vue
  double angle = Math::RadianToDegree(acos(vect*view));
  if (angle > 30 && alpha1 > 0.7)
    return -1;

  // Prise en compte de la distance
  double dist = sqrt((c.Eye()[0] - center[0])*(c.Eye()[0] - center[0]) + (c.Eye()[1] - center[1])*(c.Eye()[1] - center[1]));
  if (dist < Mindist)           alpha2 = 1.0;
  else if (dist > Maxdist)           alpha2 = 0.0;//return -1;//
  else                             alpha2 = 1. - (dist / (Maxdist - Mindist));
  res2 = resMin + (res1 - resMin)*alpha2;

  //cout <<"Resolution : "<<res2<<endl;
  return res2;
}

int oldGetLvl(int num)
{
  if (num == -1) return -1;
  int res = 0;
  while (num > 1) {
    num /= 2;
    res++;
  }
  return res;
}

MayaGeometryAll ArchesTree::createMeshMaterial(const Box2& box, int nb_div_x, int nb_div_y) const
{
  // Get Triangles
  Array2 a2(box, nb_div_x, nb_div_y);

  QVector<Vector> Bv;
  QVector<Vector> Wv;

  for (int i = 0; i < nb_div_x - 1; i++)
  {
    for (int j = 0; j < nb_div_y - 1; j++)
    {
      Vector a = Vector(a2.ArrayVertex(i, j)[0], a2.ArrayVertex(i, j)[1], 0.);
      Vector b = Vector(a2.ArrayVertex(i + 1, j)[0], a2.ArrayVertex(i + 1, j)[1], 0.);
      Vector c = Vector(a2.ArrayVertex(i, j + 1)[0], a2.ArrayVertex(i, j + 1)[1], 0.);
      Vector d = Vector(a2.ArrayVertex(i + 1, j + 1)[0], a2.ArrayVertex(i + 1, j + 1)[1], 0.);
      GetTriangles(a, b, d, Bv, Wv);
      GetTriangles(a, d, c, Bv, Wv);
    }
  }

  // Create MayaGeometry Terrain
  MayaGeometry mgTerrain("Terrain", MayaMaterial::Normal);
  for (int i = 0; i < Bv.size() / 3; i++)
  {
    mgTerrain.AddTriangle(Bv[i * 3 + 0], Bv[i * 3 + 1], Bv[i * 3 + 2]);
  }
  // Create MayaGeometry Water
  MayaGeometry mgWater("Water", MayaMaterial::NormalWire);
  for (int i = 0; i < Wv.size() / 3; i++)
  {
    mgWater.AddTriangle(Wv[i * 3 + 0], Wv[i * 3 + 1], Wv[i * 3 + 2]);
  }

  // Create scene
  MayaGeometryAll mga;
  mga.Append(mgTerrain);
  mga.Append(mgWater);
  return mga;
}


MayaGeometryAll ArchesTree::createMeshMaterialAndTexture(const Box2& box, int nb_div_x, int nb_div_y) const
{
  // Get Triangles
  Array2 a2(box, nb_div_x, nb_div_y);

  QVector<Vector> Bv;
  QVector<Vector> Wv;

  for (int i = 0; i < nb_div_x - 1; i++)
  {
    for (int j = 0; j < nb_div_y - 1; j++)
    {
      Vector a = Vector(a2.ArrayVertex(i, j)[0], a2.ArrayVertex(i, j)[1], 0.);
      Vector b = Vector(a2.ArrayVertex(i + 1, j)[0], a2.ArrayVertex(i + 1, j)[1], 0.);
      Vector c = Vector(a2.ArrayVertex(i, j + 1)[0], a2.ArrayVertex(i, j + 1)[1], 0.);
      Vector d = Vector(a2.ArrayVertex(i + 1, j + 1)[0], a2.ArrayVertex(i + 1, j + 1)[1], 0.);
      GetTriangles(a, b, d, Bv, Wv);
      GetTriangles(a, d, c, Bv, Wv);
    }
  }

  // Create MayaGeometry Terrain
  MayaGeometry mgTerrain("Terrain", MayaMaterial::Normal);
  for (int i = 0; i < Bv.size() / 3; i++)
  {
    mgTerrain.AddTriangle(Bv[i * 3 + 0], Bv[i * 3 + 1], Bv[i * 3 + 2]);
  }
  // Create MayaGeometry Water
  MayaGeometry mgWater("Water", MayaMaterial::NormalWire);
  for (int i = 0; i < Wv.size() / 3; i++)
  {
    mgWater.AddTriangle(Wv[i * 3 + 0], Wv[i * 3 + 1], Wv[i * 3 + 2]);
  }

  // Create and project texture
  QImage imFoam = createImageFoam(box, nb_div_x, nb_div_y);
  MayaMaterial material(ShaderTextureUV, Color(0.1, 0.1, 0.1, 1.0), Color(0.1, 0.1, 0.1, 1.0), Color(0.1, 0.1, 0.1, 1.0), 20.0, imFoam);
  material.materialNode = MaterialNode::UVMapping;

  mgTerrain.generatePlannarZ_Mapping(box);
  mgTerrain.SetMaterial(material);

  // Create scene
  MayaGeometryAll mga;
  mga.Append(mgTerrain);
  mga.Append(mgWater);
  return mga;
}

QImage ArchesTree::createImageFoam(const Box2& box, int nb_div_x, int nb_div_y) const
{
  // Get Triangles
  Array2 a2(box, nb_div_x, nb_div_y);
  QImage image(nb_div_x, nb_div_y, QImage::Format_ARGB32);
  image.fill(QColor(0., 0., 0.));

  if (foam) {
    for (int i = 0; i < nb_div_x - 1; i++)
    {
      for (int j = 0; j < nb_div_y - 1; j++)
      {
        double value = Clamp(foam->ElevationAlpha(a2.ArrayVertex(i, j)).Value()*255., 0., 255.);

        //value = value < 28 ? 0 : 255;

        QColor color(value, value, value, 255);
        image.setPixel(i, j, color.rgba());
      }
    }
  }

  return image;
}

MayaGeometry ArchesTree::createMeshCircle(const Circle2& circle, int nb_sub) const
{
  QVector<Vector> vec_normals;

  QVector<Vector> listp;
  QVector<int> listi;
  circle.Subdivide(nb_sub, listp, listi);

  // Modify Point
  for (int i = 0; i < listp.size(); i++)
  {
    listp[i][2] = GetBedrock(Vector2(listp.at(i))).Value();
  }

  QVector<MayaIndexVertexData> vec_index2;
  for (int i = 0; i < listi.size(); i++)
    vec_index2.append(MayaIndexVertexData(listi[i], listi[i]));

  for (int i = 0; i < listp.size(); i++)
    vec_normals.append(bedrock->Normal(Vector2(listp[i])));

  MayaMaterial moTerrain = MayaMaterial::NormalWire;
  MayaGeometry mgTerrain(QString("MeshCircleSubdivided"),
    listp, vec_normals, vec_index2, moTerrain);

  return mgTerrain;
}

MayaGeometry ArchesTree::createMeshCircleBorder(const Circle2& circle, int nb_sub) const
{

  MayaMaterial moTerrain = MayaMaterial::NormalWire;

  int n = (nb_sub) * 6;
  MayaGeometry border = MayaGeometry::CreateCylinder(circle.Center().ToVector(1.), circle.Center().ToVector(-0.1), circle.Radius(), n, false, true);
  border.SetMaterial(moTerrain);
  border.Transform(FrameScaled::Rotation(Vector(0., 0., Math::HalfPi)));

  for (int i = 0; i < n; i++)
  {
    Vector p = border.GetVertex(i);
    p[2] = GetBedrock(Vector2(p)).Value();
    border.SetVertex(i, p);
  }

  return border;
}

/*!
\brief Create a LayerStack.
\param box The box.
\param n Discretization.
*/
LayerStack ArchesTree::CreateLayerStack(const Box2& box, int n) const
{
  LayerStack layerstack(box.Cubic(), n, n);

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      Vector2 p = layerstack.ArrayVertex(i, j);
      //int n = Traversal(p);
      //cout << n << endl;

      ScalarAlpha scBedrock = bedrock->ElevationAlpha(p);
      
      layerstack.GetBedrock()(i, j) = scBedrock.Value();
      if (water != nullptr) layerstack.GetWater()(i, j) = water->ElevationAlpha(p).Value();
      layerstack.GetAlpha()(i, j) = scBedrock.Alpha();
    }
  }
  return layerstack;
}

/*!
\brief Create a LayerStack.
\param box The box.
\param n Discretization (width)
*/
LayerStack ArchesTree::CreateLayerStackRectangular(const Box2& box, int n) const
{
  double ratio = box.Diagonal()[1] / box.Diagonal()[0];
  int height = int(n * ratio);

  LayerStack layerstack(box, n, height);

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < height; j++)
    {
      Vector2 p = layerstack.ArrayVertex(i, j);
      //int n = Traversal(p);
      //cout << n << endl;

      ScalarAlpha scBedrock = bedrock->ElevationAlpha(p);

      layerstack.GetBedrock()(i, j) = scBedrock.Value();
      if (water != nullptr) layerstack.GetWater()(i, j) = water->ElevationAlpha(p).Value();
      layerstack.GetAlpha()(i, j) = scBedrock.Alpha();
    }
  }
  return layerstack;
}


/*!
\brief Create a Rock Heightfield.
\param hf Heightfield.
*/
void ArchesTree::HeightFieldRock(HeightField &hf)
{
  int ni = hf.GetSizeX();
  int nj = hf.GetSizeY();

  for (int i = 0; i < ni; i++)
  {
    for (int j = 0; j < nj; j++)
    {
      Vector2 p = hf.ArrayVertex(i, j);

      hf(i, j) = bedrock->ElevationAlpha(p).Value();
    }
  }
}

/*!
\brief Create a Water Heightfield.
\param hf Heightfield.
*/
void ArchesTree::HeightFieldWater(HeightField &hf)
{
  int ni = hf.GetSizeX();
  int nj = hf.GetSizeY();

  for (int i = 0; i < ni; i++)
  {
    for (int j = 0; j < nj; j++)
    {
      Vector2 p = hf.ArrayVertex(i, j);

      hf(i, j) = water->ElevationAlpha(p).Value();
    }
  }
}


/*!
\brief Create a Sand Heightfield.
\param hf Heightfield.
*/
void ArchesTree::HeightFieldSand(HeightField &hf)
{
  if (sand) {
    int ni = hf.GetSizeX();
    int nj = hf.GetSizeY();

    for (int i = 0; i < ni; i++)
    {
      for (int j = 0; j < nj; j++)
      {
        Vector2 p = hf.ArrayVertex(i, j);

        hf(i, j) = sand->ElevationAlpha(p).Value();
      }
    }
  }
}


bool ArchesTree::saveJson(QString name)
{
  QFile saveFile(name);

  if (!saveFile.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save file.");
    return false;
  }

  QJsonObject jsonObject;
  writeJson(jsonObject);
  QJsonDocument saveDoc(jsonObject);
  saveFile.write(saveDoc.toJson());

  return true;
}


void ArchesTree::writeJson(QJsonObject & json)
{
  QJsonArray levelArray;
  QJsonObject levelObject;

  water->writeJson(levelObject);
  levelArray.append(levelObject);

  json["tree_water"] = levelArray;

  Box2 b = water->GetBox();
  json["box_X1"] = b[0][0];
  json["box_Y1"] = b[0][1];
  json["box_X2"] = b[1][0];
  json["box_Y2"] = b[1][1];

}