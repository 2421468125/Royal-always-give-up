using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
public class Slime : Enemy
{
    public int yitu;
    public int choice = 4;//出招
    void Start()
    {
        base.Start();
        now_health = 20;
        max_health = 20;

    }
    public override void changeintension()
    {
        yitu = UnityEngine.Random.Range(1, choice + 1);
    }
    public override string Getintension()
    {
        switch (yitu)
        {
            case 1:
                {
                    //battlemanager.attack(this,hero,5);
                    return "attack";
                }
            case 2:
                {
                    //battlemanager.defend(this,this,10);
                    return "defend";
                }
            case 3: return "debuff1";
            case 4: return "debuff1";
            default: return "?";
        }
    }
    public override string Getvalue()
    {
        switch (yitu)
        {
            case 1: return "5"; break;
            case 2: return "5"; break;
            case 3: return "";break;
            case 4: return ""; break;
            default: return "";
        }
    }
    public override void Conductintension()
    {
        switch (yitu)
        {
            case 1:
            {
                hero.Hurt(battlemanager.attack(this, hero, 5));
                break;
            }
            case 2:
            {
                int real_defend = battlemanager.defend(this, this, 5);
                this.Defend(real_defend);
                break;
            }
            case 3:
            {
                battlemanager.changeBuf("xuruo",this,hero,1);
                break;
            }
            case 4:
            {
                battlemanager.changeBuf("cuiruo",this,hero,1);
                break;
            }
        }
    }
}