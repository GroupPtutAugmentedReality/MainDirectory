using System;
using terraoperator;


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
