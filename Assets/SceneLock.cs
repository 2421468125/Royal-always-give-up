using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneLock : MonoBehaviour
{
    static SceneLock ins;
    public static int Lock = 0;
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
        DontDestroyOnLoad(this);

    }

    // Update is called once per frame
    void Update()
    {
        Debug.Log(Lock);
        if (SceneManager.GetActiveScene().name == "TestScene")
            Lock = 1;
        else
            Lock = 0 ;
    }
}
