using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraSupport : MonoBehaviour
{
    // Start is called before the first frame update
    private float CameraMoveSpeed = 30f;
    public static Vector3 cameraPosition;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 p = transform.localPosition;
        if(Input.GetKey(KeyCode.UpArrow))
            p += ((CameraMoveSpeed * Time.smoothDeltaTime) * transform.up);
        if(Input.GetKey(KeyCode.DownArrow))
            p -= ((CameraMoveSpeed * Time.smoothDeltaTime) * transform.up);
        transform.localPosition = p;
        cameraPosition = p;
    }
}
