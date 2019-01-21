using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ptut2
{
    class TreeArches : MonoBehaviour
    {
        List<TerraNode> bedrock;
        /*TerraNode* water; //!< Water.
        TerraNode* foam;
        TerraNode* sand;
        VegetationNode* veg_root;
         */

        public TreeArches(ref TerraNode ter)
        {
            bedrock = new List<TerraNode>();
            bedrock.Add(ter);
            /*
                water = w;
                sand = s;
                foam = f;
                veg_root = v;
             */
        }
        public TreeArches()
        {
            bedrock = new List<TerraNode>();
            /*
                water = w;
                sand = s;
                foam = f;
                veg_root = v;
             */
        }

        public ScalarAlpha GetBedrock(ref Vector2 p,int index)
        {
            return bedrock[index].ElevationAlpha(ref p);
        }



        void Start()
        {
            bedrock.Add(new TerraNode(new  Box2(0.0,2.0,0.0,2.0)));
        }

        // Update is called once per frame
        void Update()
        {
            for(int i = 0; i < this.bedrock.Count; i++)
            {
             
                Console.WriteLine(this.bedrock[i].GetBox().x2);
                Console.WriteLine(this.bedrock[i].GetBox().y1);
                Console.WriteLine(this.bedrock[i].GetBox().y2);

                Debug.Log(this.bedrock[i].GetBox().x1);
            }
        }
    }
}
