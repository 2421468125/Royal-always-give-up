using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class BesselArrow : MonoBehaviour
{
    public GameObject reticleBlock;
    public GameObject reticleArrow;
    public int MaxCount = 18;
    public Vector3 startPoint; // 起始点  
    private Vector3 controlPoint1; // 控制点1  
    private Vector3 controlPoint2; // 控制点2  
    private Vector3 endPoint; // 结束点  
    private List<GameObject> ArrowItemList;
    private List<SpriteRenderer> RendererItemList;
    private Animator Arrow_anim;
    private bool isSelect;

    public bool IsSelect
    {
        get => isSelect;
        set
        {
            if (isSelect != value)
            {
                isSelect = value;
                if (value == true)
                {
                    PlayAnim();
                }
            }
        }
    }
    private void Awake()
    {
        InitData();
    }
    private void InitData()
    {
        reticleArrow = new GameObject("BesselArrow");
        reticleArrow.AddComponent<SpriteRenderer>();
        Texture2D arrow = Resources.Load<Texture2D>("imgs/bg/besselarrow");
        reticleArrow.GetComponent<SpriteRenderer>().sprite = Sprite.Create(arrow, new Rect(0, 0, arrow.width,arrow.height), Vector2.one * 0.5f);
        reticleArrow.transform.position = new Vector3(0, 10, 0);

        reticleBlock = new GameObject("ArrowBody");
        reticleBlock.AddComponent<SpriteRenderer>();
        Texture2D body =  Resources.Load<Texture2D>("imgs/bg/arrowbody");
        reticleBlock.GetComponent<SpriteRenderer>().sprite = Sprite.Create(body, new Rect(0, 0, body.width, body.height), Vector2.one * 0.5f);
        reticleBlock.transform.position = new Vector3(0, 10, 0);

        Arrow_anim = new Animator();
        ArrowItemList = new List<GameObject>();
        RendererItemList = new List<SpriteRenderer>();

        for (int i = 0; i < MaxCount; i++)
        {
            GameObject Arrow = (i == MaxCount - 1) ? reticleArrow : reticleBlock;
            GameObject temp = Instantiate(Arrow, this.transform);
            if (i == MaxCount - 1)
            {
                Arrow_anim = temp.GetComponent<Animator>();
            }
            ArrowItemList.Add(temp);
/*            SpriteRenderer re = temp.transform.GetChild(0).GetComponent<SpriteRenderer>();
            if (re != null)
            {
                RendererItemList.Add(re);
            }*/
        }
        Destroy( reticleArrow );
        Destroy( reticleBlock );

        Vector3 mouseWorldPosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);

        startPoint = new Vector2(mouseWorldPosition.x, mouseWorldPosition.y); 
    }
    public void Update()
    {
        Move();
        DrawBezierCurve();
    }
    private void DrawBezierCurve()
    {
        for (int i = 0; i < ArrowItemList.Count; i++)
        {
            float t = i / (float)(ArrowItemList.Count - 1); // 参数 t 在 0 到 1 之间  
            Vector3 position = CalculateBezierPoint(t, startPoint, controlPoint1, controlPoint2, endPoint);
            ArrowItemList[i].gameObject.SetActive(i != ArrowItemList.Count - 2);
            ArrowItemList[i].transform.position = position;
            ArrowItemList[i].transform.localScale = Vector3.one * (t / 2f) + Vector3.one * 0.3f;
            if (i > 0)
            {
                float SignedAngle = Vector2.SignedAngle(Vector2.up,
                ArrowItemList[i].transform.position - ArrowItemList[i - 1].transform.position);
                Vector3 euler = new Vector3(0, 0, SignedAngle);
                ArrowItemList[i].transform.rotation = Quaternion.Euler(euler);
            }
        }
    }
    private void OnDrawGizmos()
    {
        // Gizmos.color = Color.yellow;        
        // Gizmos.DrawLine(startPoint, controlPoint1);       
        // Gizmos.DrawSphere(startPoint, 0.1f);        
        // Gizmos.DrawSphere(controlPoint1, 0.1f);        
        // Gizmos.DrawLine(endPoint, controlPoint2);        
        // Gizmos.DrawSphere(endPoint, 0.1f);        
        // Gizmos.DrawSphere(controlPoint2, 0.1f);    
    }
    public void Move()
    {
        Vector3 mousePosition = Input.mousePosition; // 获取鼠标位置  
        mousePosition.z = Camera.main.nearClipPlane; // 设置z坐标为摄像机近裁剪平面的位置  
        Vector3 worldPosition = Camera.main.ScreenToWorldPoint(mousePosition);
        worldPosition.z = 2f;
        endPoint = worldPosition;

        controlPoint1 = (Vector2)startPoint + (worldPosition - startPoint) * new Vector2(-0.28f, 0.8f);
        controlPoint2 = (Vector2)startPoint + (worldPosition - startPoint) * new Vector2(0.12f, 1.4f);
        controlPoint1.z = startPoint.z;
        controlPoint2.z = startPoint.z;
    }
    /// <summary>  
    /// 设置起始位置  
    /// </summary>  
    public void SetStartPos(Vector3 pos)
    {
        startPoint = pos;
    }
    /// <summary>  
    /// 设置颜色  
    /// </summary>  
    public void SetColor(Color color)
    {
        if (RendererItemList == null)
        {
            return;
        }
        for (int i = 0; i < RendererItemList.Count; i++)
        {
            RendererItemList[i].color = color;
        }
    }
    /// <summary>  
    /// 设置颜色  
    /// </summary>  
    /// <param name="isSelect"></param>    
    public void SetColor(bool isSelect)
    {
        IsSelect = isSelect;
        if (isSelect)
        {
            SetColor(Color.red);
        }
        else
        {
            SetColor(Color.white);
        }
    }
    private void PlayAnim()
    {
        if (Arrow_anim == null)
        {
            return;
        }
        Arrow_anim.SetTrigger("select");
    }
    /// <summary>  
    /// 获取贝塞尔曲线中间点  
    /// </summary>  
    /// <param name="t">参数 t 在 0 到 1 之间</param>  
    /// <param name="startPoint">起始点</param>  
    /// <param name="controlPoint1">起点控制点</param>  
    /// <param name="controlPoint2">终点控制点</param>  
    /// <param name="endPoint">终点</param>  
    /// <returns></returns>    
    public static Vector3 CalculateBezierPoint(float t, Vector3 startPoint, Vector3 controlPoint1, Vector3 controlPoint2, Vector3 endPoint)
    {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        Vector3 point = uuu * startPoint; // (1-t)^3 * P0  
        point += 3 * uu * t * controlPoint1; // 3 * (1-t)^2 * t * P1  
        point += 3 * u * tt * controlPoint2; // 3 * (1-t) * t^2 * P2  
        point += ttt * endPoint; // t^3 * P3  

        return point;
    }
}

