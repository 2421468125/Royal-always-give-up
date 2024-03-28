using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
/*using static UnityEditor.PlayerSettings;*/

public class ShopBehavior : MonoBehaviour
{
    public GameObject DelCardMenuUI;
    public GameObject CanvasUI;
    public List<BaseCards> shop_list = new List<BaseCards>();
    public List<BaseCards> TmpList = new List<BaseCards>();
    public CardManager card_manager;
    public GameObject RemoveUI;

    Dictionary<int,int> price_dict = new Dictionary<int,int>();
    Hero hero;
    bool remove = false;
    bool has_remove = false;
    // Start is called before the first frame update
    void Start()
    {
        hero = GameObject.Find("Hero").GetComponent<Hero>();
        card_manager = GameObject.Find("CardManager").GetComponent<CardManager>();
        DrawSellCard();
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            if(remove == false)
            {
                BaseCards choose = IfChoosed(shop_list);
                if (choose != null && price_dict[choose._id] <= hero.money)
                {
                    hero.money -= price_dict[choose._id];
                    CardManager.card_list.Add(choose);
                    shop_list.Remove(choose);
                    Destroy(choose.card_obj);
                }
            }
            else if(has_remove == false && hero.money >= 50) 
            {
                BaseCards choose = IfChoosed(TmpList);
                CameraInShop.CanMove = false;
                CameraInShop.camera_return();
                if (choose != null)
                {

                    CardManager.card_list = TmpList;
                    for (int i=TmpList.Count-1; i>=0; i--)
                    {
                        Destroy(TmpList[i].card_obj);
                    }
                    CardManager.card_list.Remove(choose);
                    Texture2D image = Resources.Load<Texture2D>("imgs/npcs/sold_out");
                    Sprite sold_out = Sprite.Create(image, new Rect(0, 0, image.height,image.width), Vector2.one * 0.5f);
                    RemoveUI.GetComponent<Image>().sprite = sold_out;
                    remove = false;
                    has_remove = true ;
                    hero.money -= 50;
                }
                else
                {
                    remove = false;
                    for (int i = TmpList.Count - 1; i >= 0; i--)
                    {
                        Destroy(TmpList[i].card_obj);
                    }
                    TmpList.Clear();
                }
                    
                DelCardMenuUI.SetActive(false);
                CanvasUI.SetActive(true);
            }
        }
    }


    void DrawSellCard()
    {
        System.Random random = new System.Random();
        for (int i = 0; i < 9; ++i)
        {
            
            int rand_id = random.Next(1, CardManager.TotalCard);
            if (price_dict.ContainsKey(rand_id) == true)
            {
                i--;
                continue;
            }
            Vector3 pos = new Vector3(-6 + 2.5f *( i%5), 2.5f - (i/5)*4, 0);
            BaseCards card = card_manager.GetCardReward(rand_id, pos);
            GameObject price_obj = new GameObject("PriceObj");
            price_obj.transform.SetParent(card.card_obj.transform);
            int value = (int)card._value * random.Next(40, 55) + random.Next(0, 10);
            TextMeshPro price = null;
            price = price_obj.AddComponent<TextMeshPro>();
            price.transform.position = pos + new Vector3(0,-2,0);
            price.text = value.ToString();
            price.font = BaseCards.font;
            price.fontStyle = FontStyles.Bold;
            price.fontSize = 5f;
            price.autoSizeTextContainer = true;
            price.color = new Color32(0,0,0, 255);
            price.transform.SetParent(price_obj.transform);
            shop_list.Add(card);
            price_dict.Add(rand_id, value);
        }
        GameObject remove_obj = new GameObject("PriceObj");
        remove_obj.transform.position = new Vector3(5,-4,0);
        TextMeshPro remove_price;
        remove_price = remove_obj.AddComponent<TextMeshPro>();
        remove_price.transform.position = new Vector3(5, -4f, 0);
        remove_price.text = "50";
        remove_price.font = BaseCards.font;
        remove_price.fontStyle = FontStyles.Bold;
        remove_price.fontSize = 5f;
        remove_price.autoSizeTextContainer = true;
        remove_price.color = new Color32(0, 0, 0, 255);
        remove_price.transform.SetParent(remove_obj.transform);
    }
    BaseCards IfChoosed(List<BaseCards> list)
    {
        Vector3 pos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        float x = pos.x;
        float y = pos.y;
        foreach (BaseCards card in list)
        {
            float cx = card.card_obj.transform.position.x;
            float cy = card.card_obj.transform.position.y;
            if (x > cx - 0.7f && x < cx + 0.7f && y > cy - 0.7f && y < cy + 0.7f)
                return card;
        }
        return null;
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

    public void RemoveCard()
    {
        if (hero.money < 50 || has_remove)
            return;
        DelCardMenuUI.SetActive(true);
        CanvasUI.SetActive(false);
        remove = true;
        CameraInShop.CanMove = true;
        check_card_list(CardManager.card_list);
    }

    public void Skip()
    {
        SceneManager.LoadScene("MapScene");
    }
}
