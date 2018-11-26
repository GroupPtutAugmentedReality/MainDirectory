using System;

public class TerraOperator : TerraNode
{
    /*!
	\class TerraOperator archestree.h
	\brief Generic n-ary operators in the tree.

	Combination nodes should derive from this class.
	\ingroup ArchesTree
	*/

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
			for (int i = 1; i<nodes.size(); i++)
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
		for (int i = 0; i<nodes.size(); i++)
		{
			n += nodes.at(i)->Memory();
		}
		return n;
	}

}
