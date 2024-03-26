using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwitchOnClickBeginning : MonoBehaviour
{
    // Start is called before the first frame update
    public string targetSceneName; // 目标场景的名称
    public float size = 0.2f;
    private void OnMouseDow()
    {
        Debug.Log(222);
        SceneManager.LoadScene(targetSceneName);
    }
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0) && ClickInBounds())
            OnMouseDow();
    }


    bool ClickInBounds()
    {
        float x = Camera.main.ScreenToWorldPoint(Input.mousePosition).x;
        float y = Camera.main.ScreenToWorldPoint(Input.mousePosition).y;
        float[] bounds = { transform.position.x, transform.position.y };
        if (bounds[0] - size < x && bounds[0] + size > x && bounds[1] - size < y && bounds[1] + size > y)
            return true;
        return false;
    }
}
