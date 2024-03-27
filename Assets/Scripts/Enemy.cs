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
    public bool attack;
    Vector3 fward = new(-1, 0, 0), begin, end;
    public float speed = 100f;
    public CharacterManager characterManager;
    public int damage;
    public Bloodbar bar;
    private Canvas canvas;
    public Bounds bound;
    GameObject go;
    GameObject intention_obj;
    public int State;//enemy的状态：碰撞前后
    public TextMeshPro intention;
    static public Vector3 IntentionMoveUp = new Vector3(0, 3, 0);
    static public Hero hero;
    GameObject intension_obj;
    GameObject val_obj;
    public void Start()
    {
        //transform.localPosition=new Vector3(5,1,0);
        //buff=gameObject.AddComponent<Getbuff>(); 
        base.Start();
        canvas = FindObjectOfType<Canvas>();
        max_health = 30;
        now_health = 30;
        attack = true;
        dir = false;
        begin = transform.localPosition;
        characterManager = GameObject.Find("CharacterManager").GetComponent<CharacterManager>();
        hero = GameObject.Find("Hero").GetComponent<Hero>();
        end = transform.position - new Vector3(1, 0, 0);
        speed = 100f;
        fward = new(-1, 0, 0);
        // go= Instantiate(Resources.Load("Slider") as GameObject,canvas.transform);
        go = Instantiate(Resources.Load("Slider") as GameObject, canvas.transform.GetChild(0));
        bar = go.GetComponent<Bloodbar>();
        bar.transform.localPosition = new Vector3(45, -270, 0) + Vector3.right * (int)(transform.localPosition.x / 3) * 140;
        State = 0;
        SetIntention();
    }

    public void SetIntention()
    {
        changeintension();
        Destroy(intension_obj);
        intension_obj = new GameObject(Getintension());
        Texture2D intension_icon = Resources.Load<Texture2D>("imgs/intent/" + Getintension());

        Sprite intension_sp = Sprite.Create(intension_icon, new Rect(0, 0, intension_icon.width, intension_icon.height), Vector2.one * 0.5f);
        intension_obj.transform.localPosition = new Vector3(pos_x, pos_y, 0) + new Vector3(0, 3f, 0); //+ count*new Vector3(0.4f,0,0);
        if (name == "hexaghust(Clone)" || name == "lagavulin2(Clone)")
        {
            intension_obj.transform.localPosition += new Vector3(0, 1, 0);
        }
        intension_obj.transform.SetParent(transform);

        SpriteRenderer energy_render = intension_obj.AddComponent<SpriteRenderer>();
        energy_render.sprite = intension_sp;


        val_obj = new GameObject(Getintension() + Getvalue());
        TextMeshPro state_val = val_obj.AddComponent<TextMeshPro>();
        state_val.text = Getvalue();
        state_val.font = BaseCards.font;
        state_val.fontSize = 3;
        state_val.color = Color.cyan;
        state_val.autoSizeTextContainer = true;
        state_val.transform.SetParent(val_obj.transform);
        state_val.transform.localPosition = new Vector3(pos_x, pos_y, 0) + new Vector3(0.2f, 3.0f, 0);
        if (name == "hexaghust(Clone)" || name == "lagavulin2(Clone)") state_val.transform.localPosition += new Vector3(0, 1, 0);
        state_val.transform.SetParent(intension_obj.transform);

    }

    public int move()
    {
        if (State == 1)
        {
            transform.position = Vector3.Lerp(transform.position, end, 0.1f);
            if (transform.position.x <= end.x + 0.2)
            {
                State = 2;
                Conductintension();
            }
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
    public virtual string Getvalue() { return ""; }
    public virtual void changeintension() { }
    public void Update()
    {
        go.GetComponent<Bloodbar>().HP.value = (float)now_health / (float)max_health;
        go.GetComponent<Bloodbar>().tx.text = now_health.ToString() + "/" + max_health.ToString();
        if (now_health <= 0) Die();
    }
    public override void Hurt(int damage)
    {
        base.Hurt(damage);
        if (now_health < 0)
        {
            Die();
        }
    }
    public void Die()
    {
        characterManager.EnemyList.Remove(this);
        Destroy(intention_obj);
        Destroy(transform.gameObject);
        Destroy(go);
        characterManager.num--;
    }
}
