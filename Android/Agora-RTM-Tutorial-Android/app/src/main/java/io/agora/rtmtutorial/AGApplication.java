package io.agora.rtmtutorial;

import android.app.Application;

public class AGApplication extends Application {
    private static AGApplication sInstance;
    private ClientManager mChatManager;


    public static AGApplication the() {
        return sInstance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        sInstance = this;

        mChatManager = new ClientManager(this);
        mChatManager.init();
    }

    public ClientManager getChatManager() {
        return mChatManager;
    }
}

