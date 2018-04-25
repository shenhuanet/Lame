package com.shenhua.lame;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import com.shenhua.lame.library.LameMp3Utils;

/**
 * @author shenhua
 */
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onPostCreate(@Nullable Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        TextView textView = findViewById(R.id.sample_text);
        textView.setText(String.format("Lame Version: %s", LameMp3Utils.getLameVersion()));
    }
}
