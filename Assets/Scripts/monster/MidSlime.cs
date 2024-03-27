using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using Unity.VisualScripting;
using UnityEngine;
using TMPro;
public class MidSlime : Enemy  
{
    public int yitu;
    public int choice = 3;//出招
    bool changei=false;
    void Start()
    {
        base.Start();
        now_health = 50;
        max_health = 50;
    }
    void Update()
    {
        base.Update();
        judge();
        bar.HP.value=(float)now_health/(float)max_health;
        bar.tx.text = now_health.ToString() + "/" + max_health.ToString();
        if(now_health<= 0) Die();
    }
    public override void changeintension()
    {
        if(yitu==4) return;
        yitu = UnityEngine.Random.Range(1, 4);
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
            case 4: return "unknown";
            default: return "?";
        }
    }
    public override string Getvalue()
    {
        switch (yitu)
        {
            case 1: return "12"; 
            case 2: return "12"; 
            case 3: return "0";
            case 4: return "0"; 
            default: return "0";
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
                GameObject s1= Instantiate(Resources.Load("smallslime") as GameObject);  
                GameObject s2= Instantiate(Resources.Load("smallslime") as GameObject);
                s1.GetComponent<Enemy>().transform.localPosition = new(- 2, -1, 0);
                s1.GetComponent<Enemy>().bar.SetBarPosition(new(400, -180, 0));
                s1.GetComponent<Enemy>().transform.localPosition = new(1, -1, 0);
                s1.GetComponent<Enemy>().bar.SetBarPosition(new(400, -180, 0));
                characterManager.EnemyList.Add(s1.GetComponent<Enemy>());
                characterManager.EnemyList.Add(s2.GetComponent<Enemy>());
                characterManager.num+=2;
                Die();
                break;
            }
            default: break;
        }
    }
    void judge()
    {
        if(now_health<max_health/2)
        {
            yitu=4;
            if(changei==false){
                SetIntention();
                changei=true;
            }
        }
    }
}