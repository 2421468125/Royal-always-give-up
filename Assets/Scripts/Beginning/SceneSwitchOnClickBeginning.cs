using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwitchOnClickBeginning : MonoBehaviour
{
    // Start is called before the first frame update
    public string targetSceneName; // Ŀ�곡��������

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
