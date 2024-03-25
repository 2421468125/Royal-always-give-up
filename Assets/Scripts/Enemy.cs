using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.Mathematics;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;
using UnityEngine.XR;

public class Enemy : Character
{
    // Start is called before the first frame update
    //Getbuff buff;
    public bool dir;
    static public int att=6;
    public bool attack;
    Vector3 fward=new(-1,0,0),begin,end;
    public float speed=20f;
    CharacterManager characterManager;
    public int damage;
    public Bloodbar bar;
    private Canvas canvas;
    public Bounds bound;
    GameObject go;
    GameObject intention_obj;
    public int State;//enemy的状态：碰撞前后
    public TextMeshPro intention;
    static public Vector3 IntentionMoveUp = new Vector3(0, 3,0);
    static public Hero hero;

    public void Start()
    {
        //transform.localPosition=new Vector3(5,1,0);
        //buff=gameObject.AddComponent<Getbuff>(); 
        base.Start();
        canvas = FindObjectOfType<Canvas>();
        max_health=30;
        now_health=30;
        attack=true;
        dir=false;
        begin=transform.localPosition;
        characterManager = GameObject.Find("CharacterManager").GetComponent<CharacterManager>();
        hero = GameObject.Find("Hero").GetComponent<Hero>();
        end = transform.position - new Vector3(5, 0, 0);
        speed=100f;
        fward=new(-1,0,0);
        go= Instantiate(Resources.Load("Slider") as GameObject,canvas.transform);
        bar=go.GetComponent<Bloodbar>();
        bar.transform.localPosition = new Vector3(45, -70, 0) + Vector3.right* (int)(transform.localPosition.x / 3) * 140;
        State =0;


        SetIntention();
    }

    public void SetIntention()
    {
        intention_obj = new GameObject("Intention");
        intention_obj.transform.localPosition = transform.position + IntentionMoveUp;
        intention = intention_obj.AddComponent<TextMeshPro>();
        intention.text = "6";
        intention.font = BaseCards.font ;
        intention.fontStyle = FontStyles.Bold;
        intention.fontSize = 4;
        intention.autoSizeTextContainer = true;
/*        card_name.transform.SetParent(card_obj.transform);*/
    }
    public int  move()
    {
        if (State == 1)
        {
            transform.position = Vector3.Lerp(transform.position, end, 0.1f);
            if (transform.position.x <= end.x + 0.2) State = 2;
        }

        if (State == 2)
        {
            transform.position = Vector3.Lerp(transform.position, begin, 0.1f);
            if (transform.position.x - begin.x >= -0.2)
            {
                State = 0;
                return 2;
            }
        }
        return -1;
    }
    // Update is called once per frame

    // 选择敌人
    public virtual string Getintension() { return ""; }
    public virtual void Conductintension() { }

    void Update()
    {
        go.GetComponent<Bloodbar>().HP.value=(float)now_health/(float)max_health;
        go.GetComponent<Bloodbar>().tx.text = now_health.ToString() + "/" + max_health.ToString();
        if(now_health<0) Die();
    }
    public override void Hurt(int damage)
    {
        base.Hurt(damage);
        if(now_health<0)
        {
            Die();
        }
        if(characterManager.num==0)
        {
            battlemanager.changeState(9);
        }
    }
    void Die(){
        characterManager.EnemyList.Remove(this);
        Destroy(intention_obj.GetComponent<TextMeshPro>());
        Destroy(transform.gameObject);
        Destroy(go);
        characterManager.num--;
    }

    public virtual void enemy_turn() 
    {
        int real_damage = battlemanager.attack(this, hero, 6);
        hero.Hurt(real_damage);
        //State = 1;
    }
    
}
