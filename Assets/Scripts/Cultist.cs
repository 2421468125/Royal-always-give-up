using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
public class Cultist:Enemy
{
    public int yitu;
    public int choice=2;//出招
    void Start()
    {
        base.Start();
        now_health=40;
        max_health=40;
    }
    public override string Getintension()
    {
        yitu=UnityEngine.Random.Range(1,choice+1);
        switch(yitu){
            case 1: {
                //battlemanager.attack(this,hero,5);
                return "attack:5";
            }
            case 2: {
                //battlemanager.defend(this,this,10);
                return "defend"+ "10";
            }
            default: return "?";
        }
    }
    public override void Conductintension()
    {
        switch(yitu){
            case 1: {
                hero.Hurt(battlemanager.attack(this,hero,5));
                break;
            }
            case 2: {
                int real_defend = battlemanager.defend(this,this,10);
                this.Defend(real_defend);
                break;
            }
        }
    }
}