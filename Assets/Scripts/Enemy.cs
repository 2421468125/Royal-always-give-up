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
    public int defend;
    public Bloodbar bar;
    private Canvas canvas;
    public Bounds bound;
    GameObject go;
    GameObject intention_obj;
    public int State;//enemy的状态：碰撞前后
    public TextMeshPro intention;
    static public Vector3 IntentionMoveUp = new Vector3(0, 2,0);
    static Hero hero;

    public void Start()
    {
        //transform.localPosition=new Vector3(5,1,0);
        //buff=gameObject.AddComponent<Getbuff>(); 
        base.Start();
        canvas = FindObjectOfType<Canvas>();
        max_health=10;
        now_health=10;
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
        bar.transform.localPosition = new Vector3(0, -180, 0);
        State =0;
        Vector3 v=new (transform.localPosition.x,0,0);
        bar.transform.localPosition += Vector3.right * 11f + v * 87.2f;

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
    private void OnMouseDown()
    {
        if (CardManager.card_up != null)
            CardManager.target = this;
    }

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
