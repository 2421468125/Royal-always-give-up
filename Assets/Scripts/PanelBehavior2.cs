using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PanelBehavior2 : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 p = transform.position;
        p.y = CameraSupport.cameraPosition.y + 3;
        transform.position = p;
    }
}
