using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
public class Sentry : Enemy
{
    public int yitu = 1;
    public int choice = 2;//出招
    int term=1;
    void Start()
    {
        term=1;
        yitu = 1; base.Start();
        now_health = 40;
        max_health = 40;
    }
    public override void changeintension()
    {
        if(term==1){
            yitu=2;
            term++;
        }
        else{
            yitu=1;
            term++;
        }
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
                    return "debuff1";
                }
            default: return "?";
        }
    }
    public override string Getvalue()
    {
        switch (yitu)
        {
            case 1: return "5"; 
            case 2: return ""; 
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
                    battlemanager.changeBuf("yishang",this,hero,1); //少抽一张牌
                    break;
                }
        }
    }
}