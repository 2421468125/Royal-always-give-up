using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;

public class HealthNum : MonoBehaviour
{
    TextMeshPro health;
    GameObject health_obj;
    // Start is called before the first frame update
    void Start()
    {
        health_obj = new GameObject("health_text");

        health_obj.transform.position += new Vector3(-7, 4.25f, -1);
        health = health_obj.AddComponent<TextMeshPro>();
        health.text = GameObject.Find("Hero").GetComponent<Hero>().now_health + "/" + GameObject.Find("Hero").GetComponent<Hero>().max_health;
        health.font = BaseCards.font;
        health.fontStyle = FontStyles.Bold;
        health.fontSize = 3f;
        health.autoSizeTextContainer = true;
        health.transform.SetParent(health_obj.transform);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
