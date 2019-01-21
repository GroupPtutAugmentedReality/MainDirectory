using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ptut2 {
    public class huflyufiulgiul : MonoBehaviour
    {
        // Start is called before the first frame update

       private TreeArches arches;
        void Start()
        {
            arches = new TreeArches();

            UnityEngine.Debug.Log("je me contruit");
    }

        // Update is called once per frame
        void Update()
        {

        }

        private void OnBecameVisible()
        {
            arches.bedrock.Add(new TerraBlend(this.arches.bedrock));
            Vector2 n = new Vector2(3, 3);
            Debug.Log(arches.bedrock[0].Traversal(ref n));
            
            Debug.Log("j\'apparait fdp");
        }
    }
}
