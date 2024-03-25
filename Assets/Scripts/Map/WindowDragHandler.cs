using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WindowDragHandler : MonoBehaviour
{
    private bool isDragging = false;
    private Vector3 offset;

    private void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            if (IsMouseOverWindow())
            {
                offset = transform.position - Input.mousePosition;
                isDragging = true;
                Debug.Log("sdsds");
            }
        }
        else if (Input.GetMouseButton(0))
        {
            if (isDragging)
            {
                Vector3 newPosition = GetMouseWorldPosition() + offset;
                transform.position = newPosition;
                Debug.Log("dsd");
            }
        }
        else if (Input.GetMouseButtonUp(0))
        {
            isDragging = false;
        }
    }

    private bool IsMouseOverWindow()
    {
        Ray mouseRay = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit hit;
        if (Physics.Raycast(mouseRay, out hit))
        {
            if (hit.collider.gameObject == gameObject)
            {
                return true;
            }
        }
        return false;
    }
    private Vector3 GetMouseWorldPosition()
    {
        Ray mouseRay = Camera.main.ScreenPointToRay(Input.mousePosition);
        Plane groundPlane = new Plane(Vector3.up, Vector3.zero);
        float rayDistance;

        if (groundPlane.Raycast(mouseRay, out rayDistance))
        {
            return mouseRay.GetPoint(rayDistance);
        }

        return Vector3.zero;
    }
}
