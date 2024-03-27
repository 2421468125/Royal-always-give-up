using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
public class Hexaghust : Enemy
{
    public int yitu;
    public int choice = 4;//出招
    int term=0;
    void Start()
    {
        term=1;
        base.Start();
        now_health = 200;
        max_health = 200;
    }
    public override void changeintension()
    {
        if(term==1||term==3||term==6) yitu=1;
        else if(term==2||term==5) yitu=2;
        else if(term==4) yitu=3;
        else if(term==7) {yitu=4;term=0;}
        term++;
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
                    return "attack";
                }
            case 3:
                {
                    //battlemanager.attack(this,hero,5);
                    return "defendBuff";
                }
            case 4:
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
            case 1: return "6"; 
            case 2: return "6*2";
            case 3: return ""; 
            case 4: return "3*6"; 
            default: return "";
        }
    }
    public override void Conductintension()
    {
        switch (yitu)
        {
            case 1:
                {
                    hero.Hurt(battlemanager.attack(this, hero, 6));
                    break;
                }
            case 2:
                {
                    hero.Hurt(battlemanager.attack(this, hero, 6));
                    hero.Hurt(battlemanager.attack(this, hero, 6));
                    break;
                }
            case 3:
            {
                int real_defend = battlemanager.defend(this, this, 12);
                this.Defend(real_defend);
                battlemanager.changeBuf("liliang",hero,this,3);
                break;
            }
            case 4:
            {
                for(int i=1;i<=6;i++) hero.Hurt(battlemanager.attack(this, hero, 3));
                break;
            }
        }
    }
}