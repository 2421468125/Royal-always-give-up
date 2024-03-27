using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class CharacterManager : MonoBehaviour
{
    static CharacterManager ins;
    public int num=0;
    public List<Enemy> EnemyList;
    static public List<string> WeakEnemy;
    static public List<string> Moderate;
    static public List<string> Severe;
    static public List<string> Boss;
    Hero Man;
    GameObject go;
    private Canvas canvas;
    battleManager battlemanager;
    private int state = 0;
    static public Dictionary<float, int> enemy_index =  new Dictionary<float, int>();
    // Start is called before the first frame update
    void Awake()
    {
        Debug.Log(111);
        if (ins != null)
        {
            Destroy(this);
            return;
        }
        else
            ins = this;
        //DontDestroyOnLoad(this);
        canvas = FindObjectOfType<Canvas>();
        EnemyList = new List<Enemy>();
        WeakEnemy = new List<string>() { "sneakygremlin", "smallslime" };
        Moderate = new List<string>() { "cultist", "fungibeast" };
        Severe = new List<string>() { "serpent", "sentry", "gremlinnob", "laga" };
        Boss = new List<string>() { "hexaghust", "fireBro" };
        GameObject instanceObject = GameObject.Find("Hero");
        Man = instanceObject.GetComponent<Hero>();
        createmonster(EnemyList);
        // go = Instantiate(Resources.Load("Slider") as GameObject,canvas.transform);
        go = Instantiate(Resources.Load("Slider") as GameObject, canvas.transform.GetChild(0));
        Man.bar = go.GetComponent<Bloodbar>();
        battlemanager = GameObject.Find("battleManager").GetComponent<battleManager>();
        Man.bar.transform.localPosition = new(-195, -270, 0);
        state = 0;
    }

    void Start()
    {

    }
    // Update is called once per frame
    void Update()
    {
        if (SceneLock.Lock == 0)
            return;
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
                if (state == num)
                {
                    battlemanager.changeState(8);
                    state = 0;// end moving
                }
                else state++;
            }
        }



    }
    //createmonster(EnemyList,num);
void createmonster(List<Enemy> ch)
        {
            int lay=MapCreate.type_per0[MapCreate.laye_now,MapCreate.point_now];
            if (lay == 4)
            {
                string en = Boss[UnityEngine.Random.Range(0, 2)];
                GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                ch.Add(s.GetComponent<Enemy>());
                ch[num].transform.localPosition = new(1, -1, 0); //经验参数
                ch[num].bar.SetBarPosition(new(400, -180, 0));
                ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                num++;
            }
            else if (lay == 5)
            {
                string en = Severe[UnityEngine.Random.Range(0, 3)];
                GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                ch.Add(s.GetComponent<Enemy>());
                ch[num].transform.localPosition = new(1, -1, 0); //经验参数
                ch[num].bar.SetBarPosition(new(400, -180, 0));
                ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                num++;

                if (en == "sentry")
                {
                    for (int i = 1; i <= 2; i++)
                    {
                        s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                        ch.Add(s.GetComponent<Enemy>());
                        ch[num].transform.localPosition = new(1 + 3 * i, -1, 0); //经验参数
                        ch[num].bar.SetBarPosition(new(400, -180, 0));
                        ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                        num++;
                    }
                }
                else if (en == "serpent")
                {
                    s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                    ch.Add(s.GetComponent<Enemy>());
                    ch[num].transform.localPosition = new(4, -1, 0); //经验参数
                    ch[num].bar.SetBarPosition(new(400, -180, 0));
                    ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                    num++;
                }
            }
            else
            {
                if (Man.Layer <= 2)
                {
                    string en = WeakEnemy[UnityEngine.Random.Range(0, 2)];
                    GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                    ch.Add(s.GetComponent<Enemy>());
                    ch[num].transform.localPosition = new(1, -1, 0); //经验参数
                    ch[num].bar.SetBarPosition(new(400, -180, 0));
                    ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                    num++;
                }
                else if (Man.Layer == 3)
                {
                    GameObject s = Instantiate(Resources.Load("monster/" + "smallslime") as GameObject);
                    ch.Add(s.GetComponent<Enemy>());
                    ch[num].transform.localPosition = new(4, -1, 0); //经验参数
                    ch[num].bar.SetBarPosition(new(400, -180, 0));
                    ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                    num++;
                    s = Instantiate(Resources.Load("monster/" + "sneakygremlin") as GameObject);
                    ch.Add(s.GetComponent<Enemy>());
                    ch[num].transform.localPosition = new(1, -1, 0); //经验参数
                    ch[num].bar.SetBarPosition(new(400, -180, 0));
                    ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                    num++;
                }
                else if (Man.Layer == 4)
                {
                    for (int i = 1; i <= 2; i++)
                    {
                        string en = WeakEnemy[UnityEngine.Random.Range(0, 2)];
                        GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                        ch.Add(s.GetComponent<Enemy>());
                        ch[num].transform.localPosition = new(i * 3 - 2, -1, 0); //经验参数
                        ch[num].bar.SetBarPosition(new(400, -180, 0));
                        ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                        num++;
                    }
                }
                else if (Man.Layer == 8)
                {
                    string en = WeakEnemy[UnityEngine.Random.Range(0, 2)];
                    GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                    ch.Add(s.GetComponent<Enemy>());
                    ch[num].transform.localPosition = new(1, -1, 0); //经验参数
                    ch[num].bar.SetBarPosition(new(400, -180, 0));
                    ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                    num++;
                    en = Moderate[UnityEngine.Random.Range(0, 2)];
                    s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                    ch.Add(s.GetComponent<Enemy>());
                    ch[num].transform.localPosition = new(4, -1, 0); //经验参数
                    ch[num].bar.SetBarPosition(new(400, -180, 0));
                    ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                    num++;
                }
                else if (Man.Layer == 12)
                {
                    for (int i = 1; i <= 2; i++)
                    {
                        string en = Moderate[UnityEngine.Random.Range(0, 2)];
                        GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                        ch.Add(s.GetComponent<Enemy>());
                        ch[num].transform.localPosition = new(i * 3 - 2, -1, 0); //经验参数
                        ch[num].bar.SetBarPosition(new(400, -180, 0));
                        ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                        num++;
                    }
                }
                else if ((Man.Layer-1) / 7 == 0)
                {
                    string en = WeakEnemy[UnityEngine.Random.Range(0, 2)];
                    for (int i = 1; i <= 2; i++)
                    {
                        GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                        ch.Add(s.GetComponent<Enemy>());
                        ch[num].transform.localPosition = new(i * 3 - 2, -1, 0); //经验参数
                        ch[num].bar.SetBarPosition(new(400, -180, 0));
                        ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                        num++;
                    }
                }
                else if ((Man.Layer-1) / 7 == 1)
                {
                    string en = Moderate[UnityEngine.Random.Range(0, 2)];
                    for (int i = 1; i <= 2; i++)
                    {
                        GameObject s = Instantiate(Resources.Load("monster/" + en) as GameObject);
                        ch.Add(s.GetComponent<Enemy>());
                        ch[num].transform.localPosition = new(i * 3 - 2, -1, 0); //经验参数
                        ch[num].bar.SetBarPosition(new(400, -180, 0));
                        ch[num].bound = s.GetComponent<SpriteRenderer>().sprite.bounds;
                        num++;
                    }
                }
            }
    }
    public void Rebuild() //当玩家回合开始时，修改所有enemy的意图
    {
        for(int i=0;i<num;i++)
        {
            Destroy(EnemyList[i].intention);
            EnemyList[i].SetIntention();
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
        foreach (Enemy enemy in EnemyList)
        {
            enemy.dynamicBuf["fangyu"] = 0;
            enemy.UpdateState();
        }

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
