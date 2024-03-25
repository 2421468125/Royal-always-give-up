using System;
using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;

public class Character : MonoBehaviour
{
    // Start is called before the first frame update
    public int max_health;
    public int now_health;
    public int money;
    public Dictionary<string , int > staticBuf= new Dictionary<string, int>();//buff的目录
    public Dictionary<string , int > dynamicBuf= new Dictionary<string, int>();//buff的目录    
    public battleManager battlemanager;
   
    public Vector3 StateMoveMent = new(-1.5f, 0, 0);
    public GameObject state_block;
    public void Start()
    {
        staticBuf.Add("yishang",0);//strange数值代表易伤回合数
        staticBuf.Add("liliang",0);//strange数值代表力量
        staticBuf.Add("xuruo",0); //虚弱回合数量
        staticBuf.Add("minjie",0); //敏捷
        staticBuf.Add("cuiruo",0);
        staticBuf.Add("jinji", 0);
        staticBuf.Add("fangyu", 0);
        battlemanager = GameObject.Find("battleManager").GetComponent<battleManager>();
       
        //Debug.Log(this.transform.position);
    }
    public void getStarted()
    {
        dynamicBuf=staticBuf;
        UpdateState();
    }

    // Update is called once per frame
    void Update()
    {

    }

    private void OnMouseDown()
    {
        if (CardManager.card_up != null)
            CardManager.target = this;
    }

    public virtual void Hurt(int damage)
    {
        if (dynamicBuf["fangyu"] >= damage)
        {
            Defend(-damage);
            damage = 0;
        }
        else
        {
            damage -= dynamicBuf["fangyu"];
            Defend(-dynamicBuf["fangyu"]);
        }
        now_health -= damage;
    }

    public void Defend(int defense)
    {
        string[] buff = { "fangyu" };
        int[] val = { dynamicBuf["fangyu"]+defense };
        ChangeState(buff,val);
    }


    public void ChangeState(string[] buff, int[] val)
    {
        for(int i = 0; i < buff.Length; i++)
        {
            dynamicBuf[buff[i]] = val[i];
        }
        UpdateState();
    }


    public void UpdateState()
    {
        if (state_block !=null)
            Destroy(state_block);
        state_block = new GameObject(name + "StateBlock");
        int count = 0;
        foreach(KeyValuePair<string,int> pair in dynamicBuf)
        {
            if (pair.Value == 0)
                continue;
            GenerateState(pair.Key, pair.Value, count);
            count++;
        }
    }


    void GenerateState(string state, int val, int count)
    {
        GameObject state_obj = new GameObject(state);
        Texture2D state_icon = Resources.Load<Texture2D>(CardManager.state_icon_map[state]);
        Sprite state_sp = Sprite.Create(state_icon, new Rect(0, 0, state_icon.width, state_icon.height), Vector2.one * 0.5f);
        state_obj.transform.localPosition = transform.localPosition+ new Vector3(-0.8f, -1f, 0) + count*new Vector3(0.4f,0,0);
        state_obj.transform.SetParent(state_block.transform);
        SpriteRenderer energy_render = state_obj.AddComponent<SpriteRenderer>();
        energy_render.sprite = state_sp;


        GameObject val_obj = new GameObject(state + val.ToString());
        TextMeshPro state_val = val_obj.AddComponent<TextMeshPro>();
        state_val.text = val.ToString();
        state_val.font = BaseCards.font;
        state_val.fontSize = 2;
        state_val.autoSizeTextContainer = true;
        state_val.transform.SetParent(val_obj.transform);
        val_obj.transform.localPosition = state_obj.transform.localPosition + new Vector3(0, -0.2f, 0);
        val_obj.transform.SetParent(state_obj.transform);
    }
}
