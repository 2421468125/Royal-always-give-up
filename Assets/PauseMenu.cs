using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PauseMenu : MonoBehaviour
{
    public static bool GameIsPaused = false;
    public GameObject pauseMenuUI;
    static public GameObject RewardUI;

    public GameObject AllUI;

    public GameObject CheckDrawUI;
    public GameObject CheckDisUI;
    public GameObject CheckConsumedUI;
    public GameObject CheckAllUI;
    public GameObject GetGoldUI;
    public GameObject StateUI;
    public GameObject HelpUI;

    GameObject buff_state;

    Hero hero = null;

    public List<BaseCards> TmpList = new List<BaseCards>();

    static CardManager card_manager;

    Canvas canvas;

    CardManager CM = null;
    int bufnum;

    // Start is called before the first frame update
    void Start()
    {
        canvas = GetComponent<Canvas>();
        pauseMenuUI = canvas.transform.GetChild(11).gameObject;
        card_manager = GameObject.Find("CardManager").GetComponent<CardManager>();
        RewardUI = FindObjectOfType<Canvas>().transform.GetChild(12).gameObject;
        CM = GameObject.Find("CardManager").GetComponent<CardManager>();

        hero = GameObject.Find("Hero").GetComponent<Hero>();
        bufnum = 0;
    }


    // Update is called once per frame
    void Update()
    {
/*        if (Input.GetKey(KeyCode.Q))
        {
            if (CardManager.card_up != null)
                return;
            GameObject.Find("Hero").GetComponent<Hero>().transform.position += new Vector3(0, 0, 10);
            RewardUI.SetActive(true);
        }*/
    }

    static public void Reward()
    {
        RewardUI.SetActive(true);
    }

    public void ShowPanel()
    {

        Time.timeScale = 0.0f;
        Pause();

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


    public void check_card_list(List<BaseCards> list)
    {
        int num = 0;
        Vector3 pos = new(-5, 2, -6);
        foreach (BaseCards card in list)
        {
            num++;
            if (num > 7)
            {
                pos += new Vector3(-1.7f * 7, -2.25f, 0);
                num -= 7;
            }
            TmpList.Add(card_manager.GetCardReward(card._id, pos));
            pos += new Vector3(1.7f, 0f, 0f);
        }
        CameraMove.CanMove = true;
    }
    public void check_draw()
    {
        CheckDrawUI.SetActive(true);
        AllUI.SetActive(false);
        check_card_list(card_manager.draw_card_list);
    }

    public void check_dis()
    {
        CheckDisUI.SetActive(true);
        AllUI.SetActive(false);
        check_card_list(card_manager.discard_list);
    }

    public void check_all_card()
    {
        CheckAllUI.SetActive(true);
        AllUI.SetActive(false);
        check_card_list(CardManager.card_list);
    }

    public void check_consumed()
    {
        CheckConsumedUI.SetActive(true);
        AllUI.SetActive(false);
        check_card_list(card_manager.exhaust_list);
    }

    public void back_from_draw()
    {
        AllUI.SetActive(true);
        CheckDrawUI.SetActive(false);
        CameraMove.CanMove = false;
        CameraMove.camera_return();
        
        for (int i = TmpList.Count - 1; i >= 0; i--)
        {
            Destroy(TmpList[i].card_obj);
        }
        
        Debug.Log("back_from_draw");
    }


    public void back_from_dis()
    {
        CheckDisUI.SetActive(false);
        AllUI.SetActive(true);
        CameraMove.CanMove = false;
        CameraMove.camera_return();
        for (int i = TmpList.Count - 1; i >= 0; i--)
        {
            Destroy(TmpList[i].card_obj);
        }
        Debug.Log("back_from_dis");
    }

    public void back_from_all()
    {
        CheckAllUI.SetActive(false);
        AllUI.SetActive(true);
        CameraMove.CanMove = false;
        for (int i = TmpList.Count - 1; i >= 0; i--)
        {
            Destroy(TmpList[i].card_obj);
        }
        CameraMove.camera_return();
    }

    public void back_from_consumed()
    {
        CheckConsumedUI.SetActive(false);
        AllUI.SetActive(true);
        CameraMove.CanMove = false;
        for (int i = TmpList.Count - 1; i >= 0; i--)
        {
            Destroy(TmpList[i].card_obj);
        }
        CameraMove.camera_return();
    }

    public void get_gold()
    {
        GetGoldUI.SetActive(false);
        hero.money += 20;
    }

    public void click_buff()
    {
        StateUI.SetActive(true);
        AllUI.SetActive(false);
        hero.gameObject.SetActive(false);
        buff_state = new GameObject("NewObject");
        foreach (KeyValuePair<string, int> pair in hero.dynamicBuf)
        {
            if (pair.Value > 0)
            {

                bufnum++;
                GameObject bufstate_obj = new GameObject(CardManager.state_icon_map[pair.Key]);
                // Texture2D bufstate_icon = Resources.Load<Texture2D>("imgs/state/"+CardManager.state_icon_map[pair.Key]);
                Texture2D bufstate_icon = Resources.Load<Texture2D>(CardManager.state_icon_map[pair.Key]);
                Debug.Log(bufstate_icon.name);
                Sprite state_sp = Sprite.Create(bufstate_icon, new Rect(0, 0, bufstate_icon.width, bufstate_icon.height), Vector2.one * 0.5f);
                bufstate_obj.transform.localPosition = new Vector3(-3, 3.5f - bufnum * 0.5f, -4);
                SpriteRenderer energy_render = bufstate_obj.AddComponent<SpriteRenderer>();
                bufstate_obj.transform.SetParent(buff_state.transform);
                energy_render.sprite = state_sp;

                GameObject description_obj = new GameObject(CardManager.state_icon_map[pair.Key] + "description");
                TextMeshPro description_val = description_obj.AddComponent<TextMeshPro>();
                description_val.GetComponent<RectTransform>().anchorMin = new(0, 1);
                description_val.GetComponent<RectTransform>().anchorMax = new(0, 1);
                description_val.GetComponent<RectTransform>().pivot = new(0, 1);
                description_val.text = "   " + CardManager.helper_map[pair.Key].ToString();
                description_val.font = BaseCards.font;
                description_val.fontSize = 2;
                description_val.autoSizeTextContainer = true;
                description_obj.transform.SetParent(buff_state.transform);
                description_val.transform.SetParent(description_obj.transform);
                description_obj.transform.localPosition = bufstate_obj.transform.localPosition + new Vector3(0.2f, 0.15f, 0);
                description_obj.transform.SetParent(description_obj.transform);
            }
        }
    }

    public void back_from_state()
    {
        bufnum = 0;
        Destroy(buff_state);
        StateUI.SetActive(false);
        AllUI.SetActive(true);
        hero.gameObject.SetActive(true);
    }

    public void showHelp()
    {
        HelpUI.SetActive(true);
        AllUI.SetActive(false);
    }

    public void back_from_help()
    {
        HelpUI.SetActive(false);
        AllUI.SetActive(true);
    }
}
