using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEngine.SceneManagement;


public class RewardCard : MonoBehaviour
{
    static CardManager card_manager;
    int random1, random2, random3;
    // Start is called before the first frame update
    void Start()
    {
        card_manager = GameObject.Find("CardManager").GetComponent<CardManager>();
        Choose();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            int num = ChooseCard();
            if (num < 0)
                return;
            CardManager.card_list.Add(card_manager.CardCreate(num));
            SceneManager.LoadScene("MapScene");

        }
    }


    public void Choose()
    {
        System.Random random = new System.Random();
        random1 = random.Next(1, CardManager.TotalCard + 1);
        random2 = random.Next(1, CardManager.TotalCard + 1); ;
        random3 = random.Next(1, CardManager.TotalCard + 1); ;

        // 确保生成的三个随机数不相同
        while (random2 == random1)
        {
            random2 = random.Next(1, CardManager.TotalCard + 1); ;
        }

        while (random3 == random1 || random3 == random2)
        {
            random3 = random.Next(1, CardManager.TotalCard + 1); ;
        }

        card_manager.GetCardReward(random1, new(-5, 0, 0));
        card_manager.GetCardReward(random2, new(0, 0, 0));
        card_manager.GetCardReward(random3, new(5, 0, 0));
    }

    public int ChooseCard()
    {
        Vector3 p = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        if (p.x >= -5.7 && p.x <= -4.3 && p.y >= -1 && p.y <= 1)
        {
            return random1;
        }
        if (p.x >= -0.7 && p.x <= 0.7 && p.y >= -1 && p.y <= 1)
        {
            return random2;
        }
        if (p.x >= 4.3 && p.x <= 5.7 && p.y >= -1 && p.y <= 1)
        {
            return random3;
        }
        return -1;
    }

}
