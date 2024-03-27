using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PauseMenu : MonoBehaviour
{
    public static bool GameIsPaused = false;
    public GameObject pauseMenuUI;
    public GameObject RewardUI;

    public GameObject AllUI;

    public GameObject CheckDrawUI;
    public GameObject CheckDisUI;
    public GameObject CheckConsumedUI;
    public GameObject CheckAllUI;
    public GameObject GetGoldUI;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.Q))
        {
            if (CardManager.card_up != null)
                return;
            GameObject.Find("Hero").GetComponent<Hero>().transform.position += new Vector3(0, 0, 10);
            RewardUI.SetActive(true);
        }
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

    public void SelectCard()
    {
        SceneManager.LoadScene("SelectCardScene");
    }

    public void back_to_map()
    {
        SceneManager.LoadScene("MapScene");
    }

    public void check_draw()
    {
        CheckDrawUI.SetActive(true);
        AllUI.SetActive(false);
    }

    public void check_dis()
    {
        CheckDisUI.SetActive(true);
        AllUI.SetActive(false);
    }

    public void check_all_card()
    {
        CheckAllUI.SetActive(true);
        AllUI.SetActive(false);
    }

    public void check_consumed()
    {
        CheckConsumedUI.SetActive(true);
        AllUI.SetActive(false);
    }

    public void back_from_draw()
    {
        AllUI.SetActive(true);
        CheckDrawUI.SetActive(false);
        Debug.Log("back_from_draw");
    }


    public void back_from_dis()
    {
        CheckDisUI.SetActive(false);
        AllUI.SetActive(true);
        Debug.Log("back_from_dis");
    }

    public void back_from_all()
    {
        CheckAllUI.SetActive(false);
        AllUI.SetActive(true);
    }

    public void back_from_consumed()
    {
        CheckConsumedUI.SetActive(false);
        AllUI.SetActive(true);
    }

    public void get_gold()
    {
        GetGoldUI.SetActive(false);
    }
}
