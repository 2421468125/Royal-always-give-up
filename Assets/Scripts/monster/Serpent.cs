using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
public class Serpent : Enemy
{
    public int yitu;
    public int choice = 4;//出招
    void Start()
    {
        base.Start();
        now_health = 80;
        max_health = 80;

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
                    return "attackDefend";
                }
            case 3: return "debuff";
            case 4: return "buff";
            default: return "?";
        }
    }
    public override string Getvalue()
    {
        switch (yitu)
        {
            case 1: return "16"; 
            case 2: return "10";
            case 3: return "";
            case 4: return ""; 
            default: return "";
        }
    }
    public override void Conductintension()
    {
        switch (yitu)
        {
            case 1:
            {
                hero.Hurt(battlemanager.attack(this, hero, 16));
                break;
            }
            case 2:
            {
                int real_defend = battlemanager.defend(this, this, 10);
                hero.Hurt(battlemanager.attack(this, hero, 10));
                this.Defend(real_defend);
                break;
            }
            case 3:
            {
                battlemanager.changeBuf("choupaijianyi",this,hero,2); 
                break;
            }
            case 4:
            {
                battlemanager.changeBuf("liliang",this,this,3);
                break;
            }
        }
    }
}