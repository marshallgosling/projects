package io.agora.rtmtutorial;

import android.content.Context;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import io.agora.model.AuctionBean;
import io.agora.rtm.ResultCallback;
import io.agora.rtm.RtmClient;
import io.agora.rtm.RtmClientListener;
import io.agora.rtm.RtmMessage;
import io.agora.rtm.RtmMetadata;
import io.agora.rtm.SendMessageOptions;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;
import okio.BufferedSink;

public class ClientManager {
    private static final String TAG = ClientManager.class.getSimpleName();

    private Context mContext;
    private RtmClient mRtmClient;
    private SendMessageOptions mSendMsgOptions;
    private List<RtmClientListener> mListenerList = new ArrayList<>();
    private RtmMessagePool mMessagePool = new RtmMessagePool();
    private String mUserId;
    private String mChannelName;
    private OkHttpClient client;

    public boolean isLogin = false;

    public ClientManager(Context context) {
        mContext = context;
    }

    public void setUserId(String uid) {
        mUserId = uid;
    }

    public void init() {
        String appID = mContext.getString(R.string.agora_app_id);

        try {
            mRtmClient = RtmClient.createInstance(mContext, appID, new RtmClientListener() {
                @Override
                public void onConnectionStateChanged(int state, int reason) {
                    for (RtmClientListener listener : mListenerList) {
                        listener.onConnectionStateChanged(state, reason);
                    }
                }

                @Override
                public void onMessageReceived(RtmMessage rtmMessage, String peerId) {
                    if (mListenerList.isEmpty()) {
                        // If currently there is no callback to handle this
                        // message, this message is unread yet. Here we also
                        // take it as an offline message.
                        mMessagePool.insertOfflineMessage(rtmMessage, peerId);
                    } else {
                        for (RtmClientListener listener : mListenerList) {
                            listener.onMessageReceived(rtmMessage, peerId);
                        }
                    }
                }

                @Override
                public void onTokenExpired() {
                    getToken(new Callback() {
                        @Override
                        public void onFailure(@NonNull Call call, @NonNull IOException e) {
                            Log.i(TAG, "get token failed "+ e.getMessage());

                        }

                        @Override
                        public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {
                            String token = response.body().string();
                            Log.i(TAG, "get token : "+ token);
                            mRtmClient.renewToken(token, null);
                        }
                    });
                }

                @Override
                public void onTokenPrivilegeWillExpire() {

                }

                @Override
                public void onPeersOnlineStatusChanged(Map<String, Integer> status) {

                }

                @Override
                public void onUserMetadataUpdated(String s, RtmMetadata rtmMetadata) {

                }
            });

            if (BuildConfig.DEBUG) {
                mRtmClient.setParameters("{\"rtm.log_filter\": 65535}");
            }
        } catch (Exception e) {
            Log.e(TAG, Log.getStackTraceString(e));
            throw new RuntimeException("NEED TO check rtm sdk init fatal error\n" + Log.getStackTraceString(e));
        }

        // Global option, mainly used to determine whether
        // to support offline messages now.
        mSendMsgOptions = new SendMessageOptions();

        client = new OkHttpClient();
    }

    public RtmClient getRtmClient() {
        return mRtmClient;
    }

    public void registerListener(RtmClientListener listener) {
        mListenerList.add(listener);
    }

    public void unregisterListener(RtmClientListener listener) {
        mListenerList.remove(listener);
    }

    public SendMessageOptions getSendMessageOptions() {
        return mSendMsgOptions;
    }

    public List<RtmMessage> getAllOfflineMessages(String peerId) {
        return mMessagePool.getAllOfflineMessages(peerId);
    }

    public void getToken(Callback callback) {

        Log.i(TAG, mContext.getString(R.string.agora_server) + "/api/v1/rtmtoken?uid="+mUserId);

        Request req = new Request.Builder().url(mContext.getString(R.string.agora_server) + "/api/v1/rtmtoken?nojson=1&uid="+mUserId).build();

        client.newCall(req).enqueue(callback);

    }

    public void getChannelNameList(Callback callback) {
        Log.i(TAG, mContext.getString(R.string.agora_server) + "/api/v1/channels");

        Request req = new Request.Builder().url(mContext.getString(R.string.agora_server) + "/api/v1/channels").build();

        client.newCall(req).enqueue(callback);
    }

    public void setBid(RequestBody json, Callback callback)
    {
        Request bid = new Request.Builder().url(mContext.getString(R.string.agora_server) + "/api/v1/auction/bid")
                .post(json).addHeader("Content-type", "application/json")
                .build();
        client.newCall(bid).enqueue(callback);
    }

    public void doLogin(String token, ResultCallback callback) {
        mRtmClient.login(token, mUserId, callback);
    }

    public void doLogout(ResultCallback callback) {
        mRtmClient.logout(callback);
    }

    public void removeAllOfflineMessages(String peerId) {
        mMessagePool.removeAllOfflineMessages(peerId);
    }

    public String getChannelName()
    {
        return mChannelName;
    }

    public void setChannelName(String channelName)
    {
        mChannelName = channelName;
    }
}
