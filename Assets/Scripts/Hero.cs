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
    
    public Bloodbar bar;
    GameObject go;
    GameObject energy_icon;
    TextMeshPro energy_text;
    private Canvas canvas;
    public int energy;
    public int max_energy;

    void Start()
    {
        base.Start();
        transform.localPosition=new Vector3(-5,-1,0);
        //buff=gameObject.AddComponent<Getbuff>();
        max_health=100;
        now_health=100;
        money=99;
        bar=GameObject.Find("bloodbar2").GetComponent<Bloodbar>();
        bar.transform.localPosition=new (-195,-70,0);
        max_energy = energy = 3;
        CreateEnergyIcon();
    }
    // Update is called once per frame
    void Update()
    {
        bar.HP.value=(float)now_health/(float)max_health;
        bar.tx.text=now_health.ToString()+"/"+max_health.ToString();
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

    void UpdateEnergy()
    {
        energy_icon.GetComponentInChildren<TextMeshPro>().text = String.Format("{0}/{1}", energy, max_energy);
    }

    public void CreateEnergyIcon()
    {
        energy_icon = new GameObject("Energy_Icon");
        Texture2D icon = Resources.Load<Texture2D>("imgs/characters/ironclad/card_red_orb");
        Sprite icon_sp = Sprite.Create(icon, new Rect(0, 0, icon.width, icon.height), Vector2.one * 0.5f);
        energy_icon.transform.localPosition = new Vector3(-8, -2.5f, 0);
        SpriteRenderer energy_render = energy_icon.AddComponent<SpriteRenderer>();
        energy_render.sprite = icon_sp;

        GameObject text_obj = new GameObject("CardName");
        text_obj.transform.localPosition = new Vector3(-8,-2.5f,0);
        energy_text = text_obj.AddComponent<TextMeshPro>();
        energy_text.text = String.Format("{0}/{1}",energy,max_energy);
        energy_text.font = BaseCards.font;
        energy_text.fontStyle = FontStyles.Bold;
        energy_text.fontSize = 5;
        energy_text.autoSizeTextContainer = true;
        energy_text.transform.SetParent(text_obj.transform);
        text_obj.transform.SetParent(energy_icon.transform);
    }

}
