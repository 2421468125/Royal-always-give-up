using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Audio : MonoBehaviour
{
    // Start is called before the first frame update
    private AudioManager audioManager;

    private void Start()
    {
        audioManager = FindObjectOfType<AudioManager>();
    }

    public void PlayAudioClip(AudioClip clip)
    {
        if (audioManager != null)
        {
            audioManager.PlayAudio(clip);
        }
    }

    void Update()
    {

    }
}

