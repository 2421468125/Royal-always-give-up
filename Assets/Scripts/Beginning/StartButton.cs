using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StartButton : MonoBehaviour
{
    public bool showhelp= false;
    public GameObject help_bg;
    public GameObject about_bg;
    public GameObject Menu_bg;
    public GameObject Quithelp;
    public GameObject Quitabout;
    public GameObject start;
    public GameObject quit;
    public GameObject about;
    public GameObject help;
    // Start is called before the first frame update
    void Start()
    {
               
    }

    // Update is called once per frame
    void Update()
    {
        
    }


    public void StartGame()
    {
        SceneManager.LoadScene("StroyScene");
    }

    public void QuitGame()
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
            Application.Quit();
#endif
    }

    public void Help()
    {
        help_bg.SetActive(true);
        Quithelp.SetActive(true);
        Menu_bg.SetActive(false);
        start.SetActive(false);
        quit.SetActive(false);  
        about.SetActive(false);
        help.SetActive(false);
    }

    public void About()
    {
        about_bg.SetActive(true);
        Quitabout.SetActive(true);
        Menu_bg.SetActive(false);
        start.SetActive(false);
        quit.SetActive(false);
        about.SetActive(false);
        help.SetActive(false);
    }


    public void BackFromHelp()
    {
        help_bg.SetActive(false);
        Quithelp.SetActive(false);
        Menu_bg.SetActive(true);
        start.SetActive(true);
        quit.SetActive(true);
        about.SetActive(true);
        help.SetActive(true);
    }

    public void BackFromAbout()
    {
        about_bg.SetActive(false);
        Quitabout.SetActive(false);
        Menu_bg.SetActive(true);
        start.SetActive(true);
        quit.SetActive(true);
        about.SetActive(true);
        help.SetActive(true);
    }
}
