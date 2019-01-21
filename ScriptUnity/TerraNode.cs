using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace ptut2
{
    class TerraNode : MonoBehaviour
    {
        // voir box 2 c++
        protected Box2 box666 = new Box2();//Classe Box2 Vide !!
        protected const double epsilon = 1e-2;

        //Empty constructor
        public TerraNode() {}


        /*!
        \brief Create a node.
        \param box The box.
        */
        public TerraNode( Box2 box)
        {
            box666 = box;
        }


        /*!
        \brief Returns the bouding box of a node.
        */
       public Box2 GetBox()
        {
            return box666;
        }

        /*!
        \brief Compute the number of nodes that are traversed when evaluating the tree.
        \param p Point.
        */
       public  int Traversal(ref Vector2 p)
        {
            return 1;
        }



        Vector2 Gradient(/*const*/  Vector2 p) /*const*/
        {
            float x =(float)(Elevation((new Vector2((float)(p.x + epsilon),p.y))) - Elevation( (new Vector2( (float)(p.x + epsilon), p.y))));
            float y = (float)(Elevation((new Vector2(p.x, (float)(p.y + epsilon)))) - Elevation((new Vector2(p.x, (float)(p.y + epsilon)))));

            return new Vector2((float)(x * (0.5 / epsilon)),(float)(y * (0.5 / epsilon)));
        }


        /*!
        \brief Compute the normal at a given position.
        \sa TerraNode::Gradient
        \param p Point.
        */


            /**
             * TODO:Vefrifier que la fonction n'est pas nessecaire .
             * 
             * /*
           *//*
       ArrayList Normal(const Vector2 p) //vector transformé en ArrayList
        {
           return Vector(Vector2.Normalize(Gradient(p));
        }
    */
        /*!
        \brief Compute the elevation and weight.

        \param p Point.
        */
        public ScalarAlpha ElevationAlpha(/*const*/ ref Vector2 p) /*const*/
        {
            return new ScalarAlpha(0.0, 1.0);
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
        double Elevation(/*const*/ Vector2 p) /*const*/
        {
            return ElevationAlpha(ref p).getValue();
        }



        /*!
        \brief Compute the size of a node.
        */
        /* int Memory() 
         {
             return sizeof(TerraNode);
         }

         */


        //void TerraNode::setTime(double t)
        //{
        //
        //}

    }
}
