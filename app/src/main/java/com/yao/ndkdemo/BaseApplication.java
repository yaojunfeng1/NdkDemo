package com.yao.ndkdemo;

import android.app.Application;

/**
 * Created by Administrator on 2018/8/4.
 */

public class BaseApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        // η­Ύεζζ
        SignatureUtils.signatureVerify(this);
    }
}
