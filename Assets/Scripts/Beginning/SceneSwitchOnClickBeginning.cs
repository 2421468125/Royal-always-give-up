using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwitchOnClickBeginning : MonoBehaviour
{
    // Start is called before the first frame update
    public string targetSceneName; // 目标场景的名称

    private void OnMouseDown()
    {
        SceneManager.LoadScene(targetSceneName);
    }
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
            OnMouseDown();
    }

}
