using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Runtime.InteropServices.WindowsRuntime;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class Hero: Character
{
    // Start is called before the first frame update
    // Getbuff buff;
    //public Bloodbar bar;

    static Hero ins;
    public int Layer;
    public Bloodbar bar;
    GameObject go;
    GameObject energy_icon;
    private Canvas canvas;
    public int energy;
    public int max_energy;
    public int soul;
    public TextMeshProUGUI soul_count = null;
    public TextMeshProUGUI energy_text = null;
    public TextMeshProUGUI hp_in_bar = null;
    public TextMeshProUGUI gold_text = null;
    private void Awake()
    {
        Layer = 1;
    }
    void Start()
    {
        DontDestroyOnLoad(this);
        if (ins != null)
        {
            Destroy(this.gameObject);
            return;
        }
        else
            ins = this;
        base.Start();
        transform.localPosition=new Vector3(-5,-1,0);
        //buff=gameObject.AddComponent<Getbuff>();
        max_health=100;
        now_health=100;
        money=99;
        soul = 0;
        canvas = FindObjectOfType<Canvas>();
        bar.transform.localPosition=new (-195,-270,0);
        max_energy = energy = 3;
        CreateEnergyIcon();
    }
    // Update is called once per frame
    void Update()
    {
        if (SceneLock.Lock == 0)
            return;
        bar.HP.value=(float)now_health/(float)max_health;

        bar.tx.text=now_health.ToString()+"/"+max_health.ToString();
        hp_in_bar.text = now_health.ToString() + "/" + max_health.ToString();
        gold_text.text = this.money.ToString();
        UpdateEnergy();
    }

    public override void Hurt(int damage)
    {
        base.Hurt(damage);
        if (now_health < 0) {
            now_health = 0;
            battlemanager.changeState(11);
        }
    }

    public void UpdateEnergy()
    {
        energy_text.text = String.Format("{0}/{1}", energy, max_energy);
    }

    public void CreateEnergyIcon()
    {
        energy_text.text = String.Format("{0}/{1}", energy, max_energy);
    }


    public void AddSoul(int num)
    {
        soul += num;
        soul_count.text = soul.ToString();
    }
}
