using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PauseMenu : MonoBehaviour
{
    public static bool GameIsPaused = false;
    public GameObject pauseMenuUI;

    // Start is called before the first frame update
    void Start()
    {
        // pauseMenuUI  = GameObject.Find("PauseMenu").GetComponent<GameObject>();
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void ShowPanel()
    {
        if (!GameIsPaused)
        {
            Time.timeScale = 0.0f;
            Pause();
        }
    }

    public void Resume()
    {
        pauseMenuUI.SetActive(false);
        Time.timeScale = 1.0f;
        GameIsPaused = false;
    }

    public void Pause()
    {
        pauseMenuUI.SetActive(true);
        //pauseMenuUI.GetComponent<Renderer>().sortingOrder = 100;
        Time.timeScale = 0.0f;
        GameIsPaused = true;
    }

    public void Quit()
    {
        Application.Quit();
    }

    public void Skip()
    {

    }

    public void Help()
    {

    }

    public void MainMenu()
    {
        GameIsPaused = false;
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("Menu");
    }
}
