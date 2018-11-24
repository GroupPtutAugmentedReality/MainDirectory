using System;
using System.Reflection;

public class TerraNode
{
    /*!
	\defgroup ArchesTree ArchesTree hierachy

	\brief Terrain geometry classes.
	*/
    const double epsilon = 1e-2;

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
