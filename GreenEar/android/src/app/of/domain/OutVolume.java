package app.of.domain;

import org.qtproject.qt5.android.QtNative;
import android.content.Context;
import android.media.AudioManager;


    /** Manages a volume level of output stream  */
public class OutVolume {

        /** Common method to get Android audio manager object */
  private static AudioManager am() {
    Context c = QtNative.activity().getApplicationContext();
    return (AudioManager)c.getSystemService(c.AUDIO_SERVICE);
  }

        /** Displays native Android out volume control */
  public static void show() {
    am().adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_SAME, AudioManager.FLAG_SHOW_UI);
  }

        /** Maximal value of volume (depends on device) */
  public static int maxStreamVolume() {
    return am().getStreamMaxVolume(AudioManager.STREAM_MUSIC);
  }

        /** Current volume */
  public static int streamVolume() {
    return am().getStreamVolume(AudioManager.STREAM_MUSIC);
  }

        /** Sets volume */
  public static void setStreamVolume(int v) {
    am().setStreamVolume(AudioManager.STREAM_MUSIC, v, 0);
  }
}