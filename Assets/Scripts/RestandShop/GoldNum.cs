using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class GoldNum : MonoBehaviour
{
    TextMeshPro gold;
    GameObject gold_obj;
    // Start is called before the first frame update
    void Start()
    {

        gold_obj = new GameObject("GoldText");
        gold_obj.transform.position = new Vector3(-5f, 4.25f, -1);
        gold = gold_obj.AddComponent<TextMeshPro>();
        gold.text = GameObject.Find("Hero").GetComponent<Hero>().money.ToString();
        gold.font = BaseCards.font;
        gold.fontStyle = FontStyles.Bold;
        gold.fontSize = 3f;
        gold.autoSizeTextContainer = true;
        gold.transform.SetParent(gold_obj.transform);
    }

    // Update is called once per frame
    void Update()
    {
        gold.text = GameObject.Find("Hero").GetComponent<Hero>().money.ToString();
    }
}

