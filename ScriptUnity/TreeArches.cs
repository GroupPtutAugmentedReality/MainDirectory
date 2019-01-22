using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ptut2
{
    class TreeArches : MonoBehaviour
    {
		//variables Unity (position Box)
		double x1;
		double y1;
		double x2;
		double y2;
		//

        List<TerraNode> bedrock;
        /*TerraNode* water; //!< Water.
        TerraNode* foam;
        TerraNode* sand;
        VegetationNode* veg_root;
         */

        public TreeArches(List<TerraNode> ter)
        {
			bedrock = ter;
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
			//On calcule les coordonnees x1,y1 et x2,y2 de la box de l'objet
			x1 = this.gameObject.transform.position.x - this.gameObject.GetComponent<BoxCollider> ().size.x;
			y1 = this.gameObject.transform.position.z + this.gameObject.GetComponent<BoxCollider> ().size.z;
			x2 = this.gameObject.transform.position.x + this.gameObject.GetComponent<BoxCollider> ().size.x;
			y2 = this.gameObject.transform.position.z - this.gameObject.GetComponent<BoxCollider> ().size.z;
			Debug.Log ("x1 : " + x1); 
			Debug.Log ("y1 : " + y1);
			Debug.Log ("x2 : " + x2);
			Debug.Log ("y2 : " + y2);
            bedrock.Add(new TerraNode(new  Box2(x1,x2,y1,y2)));
        }

        // Update is called once per frame
        void Update()
        {	
			x1 = this.gameObject.transform.position.x - this.gameObject.GetComponent<BoxCollider> ().size.x;
			y1 = this.gameObject.transform.position.z + this.gameObject.GetComponent<BoxCollider> ().size.z;
			x2 = this.gameObject.transform.position.x + this.gameObject.GetComponent<BoxCollider> ().size.x;
			y2 = this.gameObject.transform.position.z - this.gameObject.GetComponent<BoxCollider> ().size.z;
			bedrock [0].GetBox ().setBox2 (x1, x2, y2, y2);
        }

		void OnMouseDown(){
			Debug.Log("x1 : " + bedrock [0].GetBox ().x1);
			Debug.Log("y1 : " + bedrock [0].GetBox ().y1);
			Debug.Log("x2 : " + bedrock [0].GetBox ().x2);
			Debug.Log("y2 : " + bedrock [0].GetBox ().y2);
			Debug.Log ("POS X : " + this.gameObject.transform.position.x);
			Debug.Log ("POS Z : " + this.gameObject.transform.position.z);
		}
    }
}
