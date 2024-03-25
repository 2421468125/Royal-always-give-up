using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class CharacterManager : MonoBehaviour
{
    public int num=0;
    public List<Enemy> EnemyList;
    Hero Man;
    battleManager battlemanager;
    private int state = 0;
    // Start is called before the first frame update
    void Awake()
    {
        EnemyList = new List<Enemy>();
        num=UnityEngine.Random.Range(1,3);

        // num = 3;
        createmonster(EnemyList,num);
        battlemanager=GameObject.Find("battleManager").GetComponent<battleManager>();
        GameObject instanceObject = GameObject.Find("Hero");
        Man=instanceObject.GetComponent<Hero>();
        state = 0;
    }

    void start()
    {

    }
    // Update is called once per frame
    void Update()
    {
        for(int i=0;i<EnemyList.Count;i++)
        {
            if (state != i + 1) continue;
            if (EnemyList[i].State == 0)
            {
                EnemyList[i].State = 1;
            }
            int flag = EnemyList[i].move();
            if(flag == 2)
            {
                EnemyList[i].enemy_turn();
                if (state == num)
                {
                    battlemanager.changeState(8);
                    state = 0;// end moving
                }
                else state++;
            }
        }
        endwar();



    }
    //createmonster(EnemyList,num);
    void createmonster(List<Enemy> ch,int n)
    {
        for(int i=1;i<=n;i++)
        {
            GameObject s= Instantiate(Resources.Load("cultist") as GameObject);
            ch.Add(s.GetComponent<Enemy>());
            ch[i - 1].transform.localPosition = new(3 * i - 2, -0.6f, 0); //�������
            ch[i - 1].bar.SetBarPosition(new(400, -180, 0));
            // ch[i - 1].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
        }
    }
    public Character Click()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Vector3 mousePosition = Camera.main.ScreenToWorldPoint(Input.mousePosition); 
            for (int i = 1; i <= num; i++)
            {
                float distance = Vector3.Distance(mousePosition, transform.localPosition);
                if (distance < 0.5f)
                {
                    return EnemyList[i-1];
                }
            }
        }
        return null;
    }
    void endwar()
    {
        if(num==0) battlemanager.changeState(9);
    }

    public void CopyBuff()
    {
        Man.getStarted();
        foreach(Enemy enemy in EnemyList)
        {
            enemy.getStarted();
        }
    }

    public void startEnemyturn()
    {
        battlemanager.changeState(7);
        state = 1;
        /*
        foreach(Enemy enemy in EnemyList)
        {
            enemy.enemy_turn();
        }
        */
    }
}
