using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwitchMaprelated : MonoBehaviour
{
    // Start is called before the first frame update
    public string targetSceneName; // Ŀ�곡��������
    public float size;
    private Renderer objectRenderer;
    private bool visited = false;
    private MapCreate mapCreate = null;
    public int laye=2, point_num=2;

    void Awake()
    {
        size = 1f;
        objectRenderer = GetComponent<Renderer>();
        objectRenderer.material.color = Color.blue;
        GameObject targetObject = GameObject.Find("GameWindow");
        mapCreate = targetObject.GetComponent<MapCreate>();
      //  laye = (int)gameObject.name[0];
      //  point_num = (int)gameObject.name[0];
    //    Debug.Log(gameObject.name);
//        Debug.Log(point_num);
    }

    // Update is called once per frame
    void Update()
    {
        if (MapCreate.If_addressable0[laye,point_num]==1)
        {
            objectRenderer.material.color = Color.white;
            if (Input.GetMouseButtonDown(0) && ClickInBounds() && (!visited))
            {
             //   Debug.Log($"visit:{laye}");
                visited = true;
                for(int i = 0;i < MapCreate.cnt_layer0[laye+1];i++)
                {
                    if (MapCreate.If_connectted0[laye,point_num,i] == 1)
                    {
                        MapCreate.If_addressable0[laye + 1, i] = 1;
                    }
                }
                for(int i = 0; i < MapCreate.cnt_layer0[laye];i++)
                {
                    if (i != point_num)
                        MapCreate.If_addressable0[laye, i] = 0;
                }
                MapCreate.If_addressable0[laye,point_num] = 0;
                MapCreate.laye_now = laye;
                MapCreate.point_now = point_num;
                OnMouseDow_();
            }
            
        }     
    }


    bool ClickInBounds()
    {
        float x = Camera.main.ScreenToWorldPoint(Input.mousePosition).x;
        float y = Camera.main.ScreenToWorldPoint(Input.mousePosition).y;
        float[] bounds = { transform.position.x, transform.position.y };
        if (bounds[0] - size < x && bounds[0] + size > x && bounds[1] - size < y && bounds[1] + size > y)
        {
            Debug.Log(size);
            return true;
        }
            
        return false;
    }
    private void OnMouseDow_()
    {
        SceneManager.LoadScene(targetSceneName);
    }
}
