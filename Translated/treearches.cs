using System;

public class ArchesTree
{
    /*!
	\class ArchesTree archestree.h
	\brief This class implements complex terrains created by combining elements.
	\ingroup ArchesTree
	*/


    protected static const double epsilon = 1e-6;
    protected TerraNode* bedrock; //!< Bedrock tree.
    protected TerraNode* water; //!< Water.
    protected TerraNode* foam;
    protected TerraNode* sand;
    protected VegetationNode* veg_root;


    /*!
	\brief Create terrain structure from an input TerraNode element with Vegetation.
	\param r Bedrock node.
	\param w Water node.
	\param s Sand node.
	\param f Foam node.
	\param v Vegetation Root node.
	*/
    public ArchesTree(TerraNode* r, TerraNode* w, TerraNode* s, TerraNode* f, VegetationNode* v)
    {
        bedrock = r;
        water = w;
        sand = s;
        foam = f;
        veg_root = v;
    }

	public ~ArchesTree()
    {
		/*not supported yet*/
    }

    /*!
	\brief Compute the bedrock height of a given input point.
	\param p Point.
	*/
    ScalarAlpha GetBedrock(/*const*/ ref Vector2 p) /*const*/
    {
		return bedrock->ElevationAlpha(p);
	}

	/*!
	\brief Compute the height of a given input point.
	\param p Point.
	*/
	ScalarAlpha GetWater(/*const*/ ref Vector2 p) /*const*/
	{
		if (water)
            return water->ElevationAlpha(p);
        return ScalarAlpha();
	}

	/*!
	\brief Compute the height of a given input point.
	\param p Point.
	*/
	ScalarAlpha GetFoam(/*const*/ ref Vector2 p) /*const*/
	{
		  if (foam)
            return foam->ElevationAlpha(p);
		return ScalarAlpha();
	}

	/*!
	\brief Compute the height of a given input point.
	\param p Point.
	*/
	ScalarAlpha GetSand(/*const*/ ref Vector2 p) /*const*/
	{
		if (sand) 
			return sand->ElevationAlpha(p);
		return ScalarAlpha();
	}

	/*!
	\brief Return the bounding box of the tree.
	*/
	Box2 GetBox() /*const*/
	{
		return bedrock->GetBox();
	}

	/*!
	\brief Compute the color of a given input point.
	\param p Point.
	*/
	Vector GetColor(/*const*/ ref Vector2 p) /*const*/
	{
		// Colors
		Vector col_sand = Vector(0.7, 0.7, 0.0);
		Vector col_rock = Vector(91. / 255., 59. / 255., 17. / 255.);
		Vector col_water = Vector(30. / 255., 30. / 255., 150. / 255.);

		ScalarAlpha mB = bedrock->ElevationAlpha(p);
		ScalarAlpha mW = water->ElevationAlpha(p);
		if (mB.Value() > mW.Value())
		{
			return col_rock;
		}
		else
		{
			return col_water;
		}
		return col_rock;
	}

	/*!
	\brief Compute the number of nodes that are traversed when evaluating the tree.
	\param p Point.
	*/
	int Traversal(/*const*/ ref Vector2 p) /*const*/
	{
		if (water)
            return bedrock->Traversal(p) + water->Traversal(p);

		return bedrock->Traversal(p);
	}

	/*!
	\brief \warn Todo
	*/
	Vector dichotomie(Vector a, Vector b, double va, double vb) /*const*/
	{
		double epsilon = 0.01;

		Vector start, middle, end;
		if (va > vb)
        {
            start = a; end = b;
        }
		else
        {
            start = b; end = a;
        }

		double l = Norm(start - end);

		ScalarAlpha msaMiddleBedrock;
		ScalarAlpha msaMiddleWater;
		do {
			middle = (start + end) / 2.;
			msaMiddleBedrock = bedrock->ElevationAlpha(middle);
			msaMiddleWater = water->ElevationAlpha(middle);

			if (msaMiddleWater.Value() < msaMiddleBedrock.Value())
				end = middle;
			else
				start = middle;

			l *= 0.5;
		} while (l > epsilon);

		middle[2] = msaMiddleBedrock.Value();

		return middle;
	}

	/*!
	\brief \warn Get triangles of terrain and water using the Archestree
	*/
	void GetTriangles(Vector a, Vector b, Vector c, ref QVector<Vector> triangleTerrain, ref QVector<Vector> triangleWater) /*const*/
    {

		int count = 0;

		ScalarAlpha maB = bedrock->ElevationAlpha(a);
		ScalarAlpha mbB = bedrock->ElevationAlpha(b);
		ScalarAlpha mcB = bedrock->ElevationAlpha(c);

		ScalarAlpha maW = ScalarAlpha();
		ScalarAlpha mbW = ScalarAlpha();
		ScalarAlpha mcW = ScalarAlpha();

		if (water) {
			maW = water->ElevationAlpha(a);
			mbW = water->ElevationAlpha(b);
			mcW = water->ElevationAlpha(c);
		}

		a[2] = 0.0;
		b[2] = 0.0;
		c[2] = 0.0;

		// Sur la terre
		if (maW.Value() <= maB.Value() && mbW.Value() <= mbB.Value() && mcW.Value() <= mcB.Value())
		{
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
		}
		// Sous l'eau
		else if (maW.Value() > maB.Value() && mbW.Value() > mbB.Value() && mcW.Value() > mcB.Value())
		{
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
			count += 3;
			triangleWater.append(a + Vector(0., 0., maW.Value()));
			triangleWater.append(b + Vector(0., 0., mbW.Value()));
			triangleWater.append(c + Vector(0., 0., mcW.Value()));
		}
		else if (maW.Value() > maB.Value() && mbW.Value() <= mbB.Value() && mcW.Value() <= mcB.Value())
		{
			Vector ab = dichotomie(a + Vector(0., 0., maB.Value()), b + Vector(0., 0., mbB.Value()), maW.Value() - maB.Value(), mbW.Value() - mbB.Value());
			Vector ac = dichotomie(a + Vector(0., 0., maB.Value()), c + Vector(0., 0., mcB.Value()), maW.Value() - maB.Value(), mcW.Value() - mcB.Value());

			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(ab);
			triangleTerrain.append(ac);

			triangleTerrain.append(ab);
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));

			triangleTerrain.append(ab);
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
			triangleTerrain.append(ac);

			count += 3;
			triangleWater.append(a + Vector(0., 0., maW.Value()));
			triangleWater.append(ab);
			triangleWater.append(ac);
		}
		else if (maW.Value() <= maB.Value() && mbW.Value() > mbB.Value() && mcW.Value() <= mcB.Value())
		{
			Vector ba = dichotomie(b + Vector(0., 0., mbB.Value()), a + Vector(0., 0., maB.Value()), mbW.Value() - mbB.Value(), maW.Value() - maB.Value());
			Vector bc = dichotomie(b + Vector(0., 0., mbB.Value()), c + Vector(0., 0., mcB.Value()), mbW.Value() - mbB.Value(), mcW.Value() - mcB.Value());

			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(bc);
			triangleTerrain.append(ba);

			triangleTerrain.append(bc);
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));

			triangleTerrain.append(bc);
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(ba);

			count += 3;
			triangleWater.append(b + Vector(0., 0., mbW.Value()));
			triangleWater.append(bc);
			triangleWater.append(ba);
		}
		else if (maW.Value() <= maB.Value() && mbW.Value() <= mbB.Value() && mcW.Value() > mcB.Value())
		{
			Vector ca = dichotomie(c + Vector(0., 0., mcB.Value()), a + Vector(0., 0., maB.Value()), mcW.Value() - mcB.Value(), maW.Value() - maB.Value());
			Vector cb = dichotomie(c + Vector(0., 0., mcB.Value()), b + Vector(0., 0., mbB.Value()), mcW.Value() - mcB.Value(), mbW.Value() - mbB.Value());

			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
			triangleTerrain.append(ca);
			triangleTerrain.append(cb);

			triangleTerrain.append(ca);
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));

			triangleTerrain.append(ca);
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(cb);

			count += 3;
			triangleWater.append(c + Vector(0., 0., mcW.Value()));
			triangleWater.append(ca);
			triangleWater.append(cb);
		}
		else if (maW.Value() <= maB.Value() && mbW.Value() > mbB.Value() && mcW.Value() > mcB.Value())
		{
			Vector ab = dichotomie(a + Vector(0., 0., maB.Value()), b + Vector(0., 0., mbB.Value()), maW.Value() - maB.Value(), mbW.Value() - mbB.Value());
			Vector ac = dichotomie(a + Vector(0., 0., maB.Value()), c + Vector(0., 0., mcB.Value()), maW.Value() - maB.Value(), mcW.Value() - mcB.Value());

			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(ab);
			triangleTerrain.append(ac);

			triangleTerrain.append(ab);
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));

			triangleTerrain.append(ab);
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
			triangleTerrain.append(ac);

			count += 3;
			count += 3;
			triangleWater.append(ab);
			triangleWater.append(b + Vector(0., 0., mbW.Value()));
			triangleWater.append(c + Vector(0., 0., mcW.Value()));

			triangleWater.append(ab);
			triangleWater.append(c + Vector(0., 0., mcW.Value()));
			triangleWater.append(ac);
		}
		else if (maW.Value() > maB.Value() && mbW.Value() <= mbB.Value() && mcW.Value() > mcB.Value())
		{
			Vector ba = dichotomie(b + Vector(0., 0., mbB.Value()), a + Vector(0., 0., maB.Value()), mbW.Value() - mbB.Value(), maW.Value() - maB.Value());
			Vector bc = dichotomie(b + Vector(0., 0., mbB.Value()), c + Vector(0., 0., mcB.Value()), mbW.Value() - mbB.Value(), mcW.Value() - mcB.Value());

			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(bc);
			triangleTerrain.append(ba);

			triangleTerrain.append(bc);
			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));

			triangleTerrain.append(bc);
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(ba);

			count += 3;
			count += 3;
			triangleWater.append(bc);
			triangleWater.append(c + Vector(0., 0., mcW.Value()));
			triangleWater.append(a + Vector(0., 0., maW.Value()));

			triangleWater.append(bc);
			triangleWater.append(a + Vector(0., 0., maW.Value()));
			triangleWater.append(ba);

		}
		else if (maW.Value() > maB.Value() && mbW.Value() > mbB.Value() && mcW.Value() <= mcB.Value())
		{
			Vector ca = dichotomie(c + Vector(0., 0., mcB.Value()), a + Vector(0., 0., maB.Value()), mcW.Value() - mcB.Value(), maW.Value() - maB.Value());
			Vector cb = dichotomie(c + Vector(0., 0., mcB.Value()), b + Vector(0., 0., mbB.Value()), mcW.Value() - mcB.Value(), mbW.Value() - mbB.Value());

			triangleTerrain.append(c + Vector(0., 0., mcB.Value()));
			triangleTerrain.append(ca);
			triangleTerrain.append(cb);

			triangleTerrain.append(ca);
			triangleTerrain.append(a + Vector(0., 0., maB.Value()));
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));

			triangleTerrain.append(ca);
			triangleTerrain.append(b + Vector(0., 0., mbB.Value()));
			triangleTerrain.append(cb);

			count += 3;
			count += 3;
			triangleWater.append(ca);
			triangleWater.append(a + Vector(0., 0., maW.Value()));
			triangleWater.append(b + Vector(0., 0., mbW.Value()));

			triangleWater.append(ca);
			triangleWater.append(b + Vector(0., 0., mbW.Value()));
			triangleWater.append(cb);
		}
		if (count > 0)
		{
			int n = triangleWater.size();
			{
				double z1 = triangleWater[n - 1][2];
				double z2 = triangleWater[n - 2][2];
				double z3 = triangleWater[n - 3][2];
			}

  }
}

	/*!
	\brief Get tree instances.
	*/
	int ArchesTree::GetInstancesTree(Vector& p, Random& ra) const
	{
  //ScalarAlpha msa = root->ElevationAlpha(p);
  //// Update p.z information
  //p[2] = msa.GetValue();
  //double water = msa.Get(ScalarAlpha::Water);

  //VegetationSampleAlpha vsa = veg_root->WeightDensity(p);

  //int r = ra.Uniform(100) + 1;    // Nb Aléatoire

  //int tree1 = vsa.Tree1()*100.0; // Densité de type1
  ///*int tree2 = vsa.Tree2()*100.0; // Densité de type2
  //int tree3 = vsa.Tree3()*100.0; // Densité de type3
  //*/
  //// Instances hors de l'eau
  //if (r <= tree1 /*&& r<=tree2*/ && water == 0) {
  //  /*if (r%2==0) */return 1;
  //  /*else        return 2;*/
  //}
  ///*if (r<=tree1 && r> tree2 && water == 0) return 1;
  //if (r> tree1 && r<=tree2 && water == 0) return 2;

  //// Instances dans l'eau
  //if (r<=tree3 && water != 0)            return 3;*/

  return 0;
}

/*!
\brief \warn
*/
int ArchesTree::GetInstancesSmallVeg(Vector& p, Random& ra) const
{
  //ScalarAlpha msa = root->ElevationAlpha(p);
  //// Update p.z information
  //p[2] = msa.GetValue();
  //double water = msa.Get(ScalarAlpha::Water);

  //VegetationSampleAlpha vsa = veg_root->WeightDensity(p);

  //int r = ra.Uniform(100) + 1;    // Nb Aléatoire

  //int tree2 = vsa.Tree2()*100.0; // Densité de type2
  //int tree3 = vsa.Tree3()*100.0; // Densité de type3

  //// Instances hors de l'eau
  //if (r <= tree2 && water == 0) return 2;

  //// Instances dans l'eau
  //if (r <= tree3 && water != 0) return 3;

  return 0;
}

/*!
\brief \warn
*/
void ArchesTree::generateVegetationNode(const Box2& box)
{

    QVector<VegetationNode*> VNS1, VNS2;

    QVector<Vector2> tile;
    tile.append(Vector2(0.06, 0.06)); tile.append(Vector2(0.39, 0.18)); tile.append(Vector2(0.74, 0.05));
    tile.append(Vector2(0.24, 0.37)); tile.append(Vector2(0.58, 0.38)); tile.append(Vector2(0.92, 0.22));
    tile.append(Vector2(0.06, 0.56)); tile.append(Vector2(0.39, 0.48)); tile.append(Vector2(0.74, 0.55));
    tile.append(Vector2(0.24, 0.87)); tile.append(Vector2(0.58, 0.72)); tile.append(Vector2(0.92, 0.82));

    Vector2 bx = box[0];
    Vector2 by = box[1];

    SimplexTurbulence tutu3;

    double tileSize1 = 50.0; // meter
    for (int i = bx[0] - tileSize1 - int(bx[0]) % int(tileSize1); i < by[0] + tileSize1; i += tileSize1)
    {
        for (int j = bx[1] - tileSize1 - int(bx[1]) % int(tileSize1); j < by[1] + tileSize1; j += tileSize1)
        {
            for (int k = 0; k < tile.size(); k++)
            {
                Vector pos = Vector(i + tile[k][0] * tileSize1, j + tile[k][1] * tileSize1, 0.0);
                double tub = tutu3.At(pos / 2.);
                if (tub < 0.5)
                {
                    VNS1.append(new VegetationDisc(pos, tileSize1 * 4., 1.0, 0.2, 0.0, 0.0));
                }
                else
                {
                    VNS1.append(new VegetationDisc(pos, tileSize1 * 4., 1.0, 0.8, 0.0, 0.0));
                }
            }
        }
    }

    double tileSize2 = 20.0;  // meter
    for (int i = bx[0] - tileSize2 - int(bx[0]) % int(tileSize2); i < by[0] + tileSize2; i += tileSize2)
    {
        for (int j = bx[1] - tileSize2 - int(bx[1]) % int(tileSize2); j < by[1] + tileSize2; j += tileSize2)
        {
            for (int k = 0; k < tile.size(); k++)
            {
                Vector2 pos = Vector2(i + tile[k][0] * tileSize2, j + tile[k][1] * tileSize2);
                ScalarAlpha msa = water->ElevationAlpha(pos);
                if (msa.Value() > 0.)
                {
                    VNS2.append(new VegetationDisc(pos, tileSize2 * 10., 1.0, 0.0, 0.2, 0.7));
                }
            }
        }
    }
    veg_root = new VegetationBlend(new VegetationBlend(VNS1), new VegetationBlend(VNS2));
}

void ArchesTree::generateVegetationNodeCanyon(const Box2& box)
{

    QVector<VegetationNode*> VNS1, VNS2;

    QVector<Vector2> tile;
    tile.append(Vector2(0.06, 0.06)); tile.append(Vector2(0.39, 0.18)); tile.append(Vector2(0.74, 0.05));
    tile.append(Vector2(0.24, 0.37)); tile.append(Vector2(0.58, 0.38)); tile.append(Vector2(0.92, 0.22));
    tile.append(Vector2(0.06, 0.56)); tile.append(Vector2(0.39, 0.48)); tile.append(Vector2(0.74, 0.55));
    tile.append(Vector2(0.24, 0.87)); tile.append(Vector2(0.58, 0.72)); tile.append(Vector2(0.92, 0.82));

    Vector2 bx = box[0];
    Vector2 by = box[1];

    SimplexTurbulence tutu3;

    double tileSize1 = 500.0; // meter
    for (int i = bx[0] - tileSize1 - int(bx[0]) % int(tileSize1); i < by[0] + tileSize1; i += tileSize1)
    {
        for (int j = bx[1] - tileSize1 - int(bx[1]) % int(tileSize1); j < by[1] + tileSize1; j += tileSize1)
        {
            for (int k = 0; k < tile.size(); k++)
            {
                Vector pos = Vector(i + tile[k][0] * tileSize1, j + tile[k][1] * tileSize1, 0.0);
                double tub = tutu3.At(pos / 2.);
                if (tub < 0.5)
                {
                    VNS1.append(new VegetationDisc(pos, tileSize1 * 4., 1.0, 0.002, 0.1, 0.0));
                }
                else
                {
                    VNS1.append(new VegetationDisc(pos, tileSize1 * 4., 1.0, 0.01, 0.1, 0.0));
                }
            }
        }
    }

    double tileSize2 = 500.0;  // meter
    for (int i = bx[0] - tileSize2 - int(bx[0]) % int(tileSize2); i < by[0] + tileSize2; i += tileSize2)
    {
        for (int j = bx[1] - tileSize2 - int(bx[1]) % int(tileSize2); j < by[1] + tileSize2; j += tileSize2)
        {
            for (int k = 0; k < tile.size(); k++)
            {
                Vector pos = Vector(i + tile[k][0] * tileSize2, j + tile[k][1] * tileSize2, 0.0);
                ScalarAlpha msa = water->ElevationAlpha(pos);
                if (msa.Value() > 0.)
                {
                    VNS2.append(new VegetationDisc(pos, tileSize2 * 10., 1.0, 0.0, 0.01, 0.05));
                }
            }
        }
    }
    veg_root = new VegetationBlend(new VegetationBlend(VNS1), new VegetationBlend(VNS2));
}

/*!
\brief \warn Create a MayaInstanceAll (Vegetation)
*/
MayaInstanceAll ArchesTree::GetVegetation(const Box2& box, bool use_cam) const
{
  Random randomTree;
Random randomScale;

MayaInstanceAll mia;
MayaInstanceSet* mis;

int minx = min(box[0][0], box[1][0]);
int maxx = max(box[0][0], box[1][0]);
int miny = min(box[0][1], box[1][1]);
int maxy = max(box[0][1], box[1][1]);

// Vegetation
Vector p(0, 0, 0);
MayaMaterial moTree2 = MayaMaterial(ShaderPhong, Color(0.3, 0.6, 0.3, 1.0), Color(0.2, 0.6, 0.2, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);

int pas = 5;
bool test;
  for (int x = minx; x <= maxx; x += pas)
  {
    for (int y = miny; y <= maxy; y += pas)
    {
      test = true;
      p[0] = x + randomTree.Uniform(pas* 2) - pas;
      p[1] = y + randomTree.Uniform(pas* 2) - pas;

      // Test d'orientation de la pente à faire

       // Ajout de l'instance
      if (test) {
        Matrix m = Matrix::Rotation(Vector(0.0, 0.0, randomTree.Uniform(2.0 * Math::Pi)));
int t = GetInstancesTree(p, randomTree);
        switch (t) {
        case 1:
          mis = new MayaInstanceSet("AmurCork_Fall_V1", FrameScaled(m, p, Vector(0.5 + randomScale.Uniform(-0.05, 0.05))));
          mia.Append(* mis);
          break;
        case 2:
          mis = new MayaInstanceSet("AmurCork_Fall_V1", FrameScaled(m, p, Vector(0.5 + randomScale.Uniform(-0.05, 0.05))));
          mia.Append(* mis);
          break;
        case 3:
          mis = new MayaInstanceSet("Willow_Spring_V3", FrameScaled(m, p - Vector(0., 0., 0.4), Vector(0.5 + randomScale.Uniform(-0.05, 2.0))), moTree2);
          mia.Append(* mis);
          break;
        }
      }
    }
  }

  pas = 2;
  for (int x = minx; x <= maxx; x += pas)
  {
    for (int y = miny; y <= maxy; y += pas)
    {
      test = true;
      p[0] = x + randomTree.Uniform(pas* 2) - pas;
      p[1] = y + randomTree.Uniform(pas* 2) - pas;

      // Ajout de l'instance
      if (test)
      {
        Matrix m = Matrix::Rotation(Vector(0.0, 0.0, randomTree.Uniform(2.0 * Math::Pi)));
int t = GetInstancesSmallVeg(p, randomTree);
        switch (t) {
        case 2:
          mis = new MayaInstanceSet("AZERTY2", FrameScaled(m, p, Vector(1.5, 1.5, 0.5 + randomScale.Uniform(-0.1, 1.0))), moTree2);
          mia.Append(* mis);
          break;
        case 3:
          mis = new MayaInstanceSet("nenu", FrameScaled(m, p - Vector(0., 0., 0.4), Vector(0.5 + randomScale.Uniform(-0.05, 2.0))), moTree2);
          mia.Append(* mis);
          break;
        }
      }
    }
  }

  return mia;
}
}
