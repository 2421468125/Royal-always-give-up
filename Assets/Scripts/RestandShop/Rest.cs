using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Rest : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void GetRestAndReturnMap()
    {
        Hero hero = GameObject.Find("Hero").GetComponent<Hero>();
        int health = (int)(hero.max_health * 0.3f);
        hero.now_health = Mathf.Min(hero.max_health, hero.now_health + health);
        SceneManager.LoadScene("MapScene");

    }


}
