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

            UnityEngine.Debug.Log("je me construirt tz mmrre");
    }

        // Update is called once per frame
        void Update()
        {

        }

        private void OnBecameVisible()
        {
            Debug.Log("j\'apparait fdp");
        }
    }
}
