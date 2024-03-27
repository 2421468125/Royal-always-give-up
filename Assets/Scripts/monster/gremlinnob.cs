using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
using System.Runtime.InteropServices.WindowsRuntime;
public class gremlinnob : Enemy
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
            default: return "";
        }
    }
    public override void Conductintension()
    {
        switch (yitu)
        {
            case 1:
                {
                    battleManager.changeBuf("fennu",this,this,1);
                    break;
                }
            case 2:
                {
                    int realDamage = battleManager.attack(this,hero,10);
                    hero.Hurt(realDamage);
                    battleManager.changeBuf("yishang",this,hero,2);
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
                battleManager.changeBuf("xuruo",this,hero,1);
                break;
            }
            default:{
                Debug.Log(this + "error");
                return;
            }
        }
    }
}