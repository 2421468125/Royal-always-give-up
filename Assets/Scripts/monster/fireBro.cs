using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
using System.Runtime.InteropServices.WindowsRuntime;
public class fireBro : Enemy
{
    public int yitu;
    public int choice = 2;//出招
    public int turns = 0;
    public battleManager battleManager;
    void Start()
    {
        base.Start();
        now_health = 200;
        max_health = 200;
        battleManager = GameObject.Find("battleManager").GetComponent<battleManager> ();
    }
    public override void changeintension()
    {
        turns++;
        if(turns == 1) yitu = 1;
        else if(turns == 2) yitu = 2;
        else{
            yitu = UnityEngine.Random.Range(3, choice + 3);
        }
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
                    return "attackDebuff";
                }
            case 3:
                {
                    return "attack";
                }
            case 4:
                {
                    return "attackDebuff";
                }
            case 5:
                {
                    return "defend";
                }
            default: return "?";
        }
    }
    public override string Getvalue()
    {
        switch (yitu)
        {
            case 1: return ""; 
            case 2: return "10";
            case 3: return "15";
            case 4: return "10";
            case 5: return "20";
            default: return "";
        }
    }
    public override void Conductintension()
    {
        switch (yitu)
        {
            case 1:
                {
                    battleManager.changeBuf("noxiousFumes",this,this,1);
                    break;
                }
            case 2:
                {
                    int realDamage = battleManager.attack(this,hero,10);
                    hero.Hurt(realDamage);
                    battleManager.changeBuf("zuzhou",this,hero,5);
                    break;
                }
            case 3:
            {
                int realDamage = battleManager.attack(this,hero,15);
                hero.Hurt(realDamage);
                break;
            }
            case 4:
            {
                int realDamage = battleManager.attack(this,hero,10);
                hero.Hurt(realDamage);
                battleManager.changeBuf("zuzhou",this,hero,3);
                break;
            }
            case 5:
            {
                int realDefend = battleManager.defend(this,this,20);
                Defend(realDefend);
                break;
            }
            default:{
                Debug.Log(this + "error");
                return;
            }
        }
    }
}