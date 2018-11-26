using System;

public class TerraNode
{
    /*!
	\defgroup ArchesTree ArchesTree hierachy

	\brief Terrain geometry classes.
	*/

    protected TerraNode* bedrock; //!< Bedrock tree.
    protected TerraNode* water; //!< Water.
    protected TerraNode* foam;
    protected TerraNode* sand;
    protected VegetationNode* veg_root;
    protected static const double epsilon = 1e-2;

    /*!
	\class TerraNode archestree.h
	\brief This class implements a generic node in the ArchesTree model.

	The default elevation function is set to 0 and the weight to 1.
	\ingroup ArchesTree
	*/

    /*!
    \brief Create a node.
    \param box The box.
    */

    public TerraNode(/*const*/ ref Box2 box)
    {
        box666 = box;
    }

    /*!
	\brief Returns the bouding box of a node.
	*/
    public Box2 GetBox() /*const*/
    {
        return box666;
    }

    /*!
	\brief Compute the number of nodes that are traversed when evaluating the tree.
	\param p Point.
	*/

    int Traversal(/*const*/ ref Vector2 p) /*const*/
    {
        return 1;
    }



    /*!
	\brief Compute the gradient at a given position.
	\param p Point.
	*/
    Vector2 Gradient(/*const*/ ref Vector2 p) /*const*/
    {
        double x = Elevation(Vector2(p[0] + epsilon, p[1])) - Elevation(Vector2(p[0] - epsilon, p[1]));
        double y = Elevation(Vector2(p[0], p[1] + epsilon)) - Elevation(Vector2(p[0], p[1] - epsilon));

        return Vector2(x, y) * (0.5 / epsilon);
    }


    /*!
	\brief Compute the normal at a given position.
	\sa TerraNode::Gradient
	\param p Point.
	*/
    Vector Normal(/*const*/ ref Vector2 p) /*const*/
    {
        return Normalized(Gradient(p).ToVector(1.0));
    }


    /*!
    \brief Compute the elevation and weight.

    \param p Point.
    */
    ScalarAlpha ElevationAlpha(/*const*/ ref Vector2 p) /*const*/
    {
        return ScalarAlpha(0.0, 1.0);
    }


    /*!
	\brief Compute the elevation at a given position.

	This is a shortcut for:
	\code
	TerraNode* node; // Node
	double e=node->ElevationAlpha(p).Value(); // Get elevation from ScalarAlpha
	\endcode
	\param p Point.
	*/
    double Elevation(/*const*/ ref Vector2 p) /*const*/
    {
        return ElevationAlpha(p).Value();
    }



    /*!
	\brief Compute the size of a node.
	*/
    int Memory() /*const*/
    {
        return sizeof(TerraNode);
    }

}

public class TerraOperator : TerraNode
{
    /*!
	\class TerraOperator archestree.h
	\brief Generic n-ary operators in the tree.

	Combination nodes should derive from this class.
	\ingroup ArchesTree
	*/
    protected QVector<TerraNode*> nodes; //!< Array of nodes that will be combined



    /*!
    \brief Create a generic operator.
    \param nodes Sub-tree nodes.
    */
    public TerraOperator(/*const*/ ref QVector<TerraNode*> nodes)
    {
        TerraOperator::nodes = nodes;
        if (nodes.size() != 0)
        {
            box666 = nodes.at(0)->GetBox();
            for (int i = 1; i < nodes.size(); i++)
            {
                box666 = Box2(box666, nodes.at(i)->GetBox());
            }
        }
    }

    /*!
	\brief Create a node with several children nodes.

	Note that the first two argments should not be 0.

	\param a, b, c, d Argument children.
	*/


    public TerraOperator(TerraNode* a, TerraNode* b, TerraNode* c, TerraNode* d)
    {
        nodes.append(a);

        if (b != nullptr) nodes.append(b);
        if (c != nullptr) nodes.append(c);
        if (d != nullptr) nodes.append(d);

        box666 = nodes.at(0)->GetBox();
        for (int i = 1; i < nodes.size(); i++)
        {
            box666 = Box2(box666, nodes.at(i)->GetBox());
        }
    }

    /*!
	\brief Recursively destroys the complete sub-tree.
	*/

    public ~TerraOperator()
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            delete nodes.at(i);
        }
    }

    /*!
	\brief Compute the size of a node.
	*/

    public int Memory() /*const*/
    {
        int n = sizeof(TerraBlendGrid);
        for (int i = 0; i < nodes.size(); i++)
        {
            n += nodes.at(i)->Memory();
        }
        return n;
    }

}


public class TerraBlend : TeraOperator
{
    /*!
	\class TerraBlend archestree.h
	\brief The blending node in the ArchesTree model.

	The blending node computes the weighted average of the children nodes.

	Whenever many primitives need to be blended together, it may be more
	efficient to use the ArchesBlendGrid node that uses a loose grid to speed up computations.

	\sa ArchesBlendGrid
	\ingroup ArchesTree
	*/

    /*!
    \brief Create a blending node.
    \param v Argument children.
    */
    public TerraBlendTerraBlend(QVector<TerraNode*> v)
    {
        TerraOperator(v);
    }

    /*!
	\brief Create a blending node.
	\param a, b, c, d Argument children.
	*/

    public TerraBlend(TerraNode* a, TerraNode* b, TerraNode* c, TerraNode* d) 
    {
        TerraOperator(a, b, c, d);
    }


    /*!
	\brief Compute the weight and the height of a given point in the plane.
	\param p Point.
	*/
    public ScalarAlpha ElevationAlpha(/*const*/ ref Vector2 p) /*const*/
	{
		double w = 0.0;
		double hr = 0.0;

		if (!box666.Inside(p))
			return ScalarAlpha();

		double wi, hri;
		for (int i = 0; i<nodes.size(); i++)
		{
			ScalarAlpha sss = nodes.at(i)->ElevationAlpha(p);
			wi = sss.Alpha();
			w += wi;

			hri = sss.Value();
			hr += wi* hri;
		}

		if (w == 0.0)
		{
			return ScalarAlpha();
		}

		hr /= w;
		//w /= nodes.size();

		return ScalarAlpha(hr, w);
	}


    /*!
	\brief Draw the sub-tree.
	*/
    public void Draw(ref QGraphicsScene gs) /*const*/
	{
		for (int i = 0; i<nodes.size(); i++)
		{
			nodes.at(i)->Draw(gs);
		}
	}

    /*!
	\brief Recursively compute the number of nodes that are traversed when evaluating the tree.
	\param p Point.
	*/
    int Traversal(/*const*/ ref Vector2 p) /*const*/
    {
		int n = 1;

		if (!box666.Inside(p))
			return n;

		for (int i = 0; i<nodes.size(); i++)
		{
			n += nodes.at(i)->Traversal(p);
		}
		return n;
	}

    /*!
	\brief Compute the Json Document of the subtree.
	*/
    void writeJson(ref QJsonObject json)
    {

        QJsonArray levelArray;

        for (int i = 0; i < nodes.size(); i++)
        {
            QJsonObject levelObject;
            nodes.at(i)->writeJson(levelObject);
            levelArray.append(levelObject);
        }

        json["Blend"] = levelArray;
    }
}
