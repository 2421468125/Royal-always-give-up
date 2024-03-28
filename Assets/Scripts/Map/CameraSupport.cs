using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class CameraSupport : MonoBehaviour
{
    // Start is called before the first frame update
    private float CameraMoveSpeed = 30f;
    static public Vector3 cameraPosition;
    void Start()
    {    
        if (SceneCreateManager.cameraGenerated == false)
        {
            cameraPosition.x = 0;
            cameraPosition.y = -100;
            cameraPosition.z = -10;

        }
        transform.localPosition = cameraPosition;
        SceneCreateManager.cameraGenerated = true;
    }




        // Update is called once per frame
        void Update()
    {
         Vector3 p = transform.localPosition;
         float scrollAmount = Input.GetAxis("Mouse ScrollWheel");
         p += scrollAmount * CameraMoveSpeed * transform.up * Time.smoothDeltaTime * 20;
         transform.localPosition = p;
         cameraPosition = p;
    }
}
