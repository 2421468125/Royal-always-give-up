using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneLock : MonoBehaviour
{
    static SceneLock ins;
    static battleManager battle_manager;
    public static int Lock = 0;
    public static int count = 0;
    public static int  add=1;
    // Start is called before the first frame update
    void Start()
    {
        if (ins != null)
        {
            Destroy(this.gameObject);
            return;
        }
        else
            ins = this;
        battle_manager = GameObject.Find("battleManager").GetComponent<battleManager>();
        DontDestroyOnLoad(this);

    }

    // Update is called once per frame
    void Update()
    {
        Debug.Log(Lock);
        if (SceneManager.GetActiveScene().name == "TestScene")
        {
            count += add;
            if (count >= 10)
            {
                Lock = 1;
                add = 0;
                count = 0;
                battle_manager.changeState(1);
            }
                
        }
            
        else
        {
            count = 0;
            Lock = 0;
            add = 1;
        }
            
    }
}
