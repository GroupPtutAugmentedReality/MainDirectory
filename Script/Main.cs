using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Main : MonoBehaviour
{
    public GameObject Poplar_tree;
    public List<GameObject> t;
    private int cpt=0;
    // Start is called before the first frame update
    void Start()
    {
        GameObject clone = Instantiate(Poplar_tree, new Vector3(0, 0, 0), Quaternion.identity) as GameObject;
        t.Add(clone);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnBecameVisible()
    {

        GameObject clone = Instantiate(Poplar_tree, new Vector3(0, 0, 0), Quaternion.identity) as GameObject;
    }
    public void OnMouseDown()
    {
        this.cpt++;
        Random rnd = new Random();
        int x = rnd.Next(1, 10);
        GameObject clone = Instantiate(Poplar_tree, new Vector3(0, 0, 0), Quaternion.identity) as GameObject;
    }
}
