using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwitchOnClickBeginning : MonoBehaviour
{
    public void StartGame()
    {
        SceneManager.LoadScene("MapScene");
    }

    public void Help()
    {
        Debug.Log("help");
    }

    public void Quit()
    {
        Application.Quit();
    }
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
    }

}
