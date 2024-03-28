using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraInShop : MonoBehaviour
{
    private float CameraMoveSpeed = 30f;
    static public bool CanMove = false;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (CanMove)
        {
            Vector3 p = transform.localPosition;
            float scrollAmount = Input.GetAxis("Mouse ScrollWheel");

            p += scrollAmount * CameraMoveSpeed * transform.up * Time.smoothDeltaTime * 20;

            p.y = Mathf.Clamp(p.y, -8f, 0f);

            transform.localPosition = p;
        }
    }

    static public void camera_return()
    {
        Camera.main.transform.localPosition = new Vector3(0, 0, -10);
    }
}
