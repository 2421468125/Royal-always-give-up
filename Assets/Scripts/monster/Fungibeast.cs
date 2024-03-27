using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
public class Fungibeast : Enemy
{
    public int yitu;
    public int choice = 2;//出招
    void Start()
    {
        base.Start();
        now_health = 25;
        max_health = 25;

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
                    return "buff";
                }
            default: return "?";
        }
    }
    public override string Getvalue()
    {
        switch (yitu)
        {
            case 1: return "8";
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
                    hero.Hurt(battlemanager.attack(this, hero, 8));
                    break;
                }
            case 2:
                {
                    battlemanager.changeBuf("liliang",this,this,5);
                    break;
                }
        }
    }
}