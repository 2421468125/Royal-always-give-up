using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ReMove : MonoBehaviour
{
    public GameObject DelCardMenuUI;
    public GameObject CanvasUI;
    public List<BaseCards> TmpList = new List<BaseCards>();
    public CardManager card_manager;
    BaseCards choose = null;
    // Start is called before the first frame update
    void Start()
    {
        card_manager = GameObject.Find("CardManager").GetComponent<CardManager>();

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            choose = IfChoosed();
            if (choose != null)
            {
                TmpList.Remove(choose);
                CardManager.card_list = TmpList;
                SceneManager.LoadScene("MapScene");
            }
            else
            {
                for (int i = TmpList.Count - 1; i >= 0; i--)
                {
                    Destroy(TmpList[i].card_obj);
                }
                TmpList.Clear();
                DelCardMenuUI.SetActive(false);
                CanvasUI.SetActive(true);
                CameraInShop.CanMove = false;
                CameraInShop.camera_return();
            }
        }

    }


    public void RemoveCard()
    {
        DelCardMenuUI.SetActive(true);
        CanvasUI.SetActive(false);
        CameraInShop.CanMove = true;
        check_card_list(CardManager.card_list);

    }


    public void check_card_list(List<BaseCards> list)
    {
        int num = 0;
        Vector3 pos = new(-6, 2, -6);
        foreach (BaseCards card in list)
        {
            num++;
            if (num > 7)
            {
                pos += new Vector3(-1.7f * 7, -2.25f, 0);
                num -= 7;
            }
            pos += new Vector3(1.7f, 0f, 0f);
            TmpList.Add(card_manager.GetCardReward(card._id, pos));
        }
        CameraMove.CanMove = true;
    }


    BaseCards  IfChoosed()
    {
        Vector3 pos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        float x = pos.x;
        float y = pos.y;
        foreach(BaseCards card in TmpList)
        {
            float cx = card.card_obj.transform.position.x;
            float cy = card.card_obj.transform.position.y;
            if (x > cx - 0.7f && x < cx + 0.7f && y > cy - 0.7f && y < cy + 0.7f)
                return card;
        }
        return null;
    }
}
