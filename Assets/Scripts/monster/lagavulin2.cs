using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
using System.Runtime.InteropServices.WindowsRuntime;
public class lagavulin2 : Enemy
{
    public int yitu;
    public int choice = 1;//出招
    public int turns = 0;
    public battleManager battleManager;
    void Start()
    {
        base.Start();
        now_health = 80;
        max_health = 80;
        battleManager = GameObject.Find("battleManager").GetComponent<battleManager> ();
    }
    public override void changeintension()
    {
        turns++;
        if(turns % 2 == 1){
            yitu = 1;
        }
        else yitu = 2;
    }
    public override string Getintension()
    {
        switch (yitu)
        {
            case 1:
                {
                    //battlemanager.attack(this,hero,5);
                    return "buff";
                }
            case 2:
                {
                    //battlemanager.defend(this,this,10);
                    return "attack";
                }
            default: return "?";
        }
    }
    public override string Getvalue()
    {
        switch (yitu)
        {
            case 1: return ""; 
            case 2: return "13";
            default: return "";
        }
    }
    public override void Conductintension()
    {
        switch (yitu)
        {
            case 1:
                {
                    battleManager.changeBuf("jinji+",this,this,1); // 生命值每次减少，随机丢弃英雄的一张卡牌
                    break;
                }
            case 2:
                {
                    int realDamage = battleManager.attack(this,hero,13);
                    hero.Hurt(realDamage);
                    break;
                }
            default:{
                Debug.Log(this + "error");
                return;
            }
        }
    }
}