package io.agora.activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.JsonReader;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import okhttp3.MediaType;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.theartofdev.edmodo.cropper.CropImage;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import io.agora.adapter.AuctionAdapter;
import io.agora.model.AuctionBean;
import io.agora.model.AuctionListBean;
import io.agora.rtm.ErrorInfo;
import io.agora.rtm.ResultCallback;
import io.agora.rtm.RtmChannel;
import io.agora.rtm.RtmChannelAttribute;
import io.agora.rtm.RtmChannelListener;
import io.agora.rtm.RtmChannelMember;
import io.agora.rtm.RtmClient;
import io.agora.rtm.RtmClientListener;
import io.agora.rtm.RtmMessage;
import io.agora.rtm.RtmMessageType;
import io.agora.rtm.RtmMetadata;
import io.agora.rtm.RtmMetadataItem;
import io.agora.rtm.RtmMetadataOptions;
import io.agora.rtm.RtmStatusCode;
import io.agora.rtmtutorial.AGApplication;
import io.agora.rtmtutorial.ClientManager;
import io.agora.rtmtutorial.R;
import io.agora.utils.MessageUtil;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.MediaType;
import okhttp3.Request;
import okhttp3.Response;

public class AuctionActivity extends Activity {
    private final String TAG = AuctionActivity.class.getSimpleName();

//    private TextView mTitleTextView;
//    private EditText mMsgEditText;
//    private ImageView mBigImage;
    private RecyclerView mRecyclerView;
    private List<AuctionBean> mAuctionBeanList = new ArrayList<>();
    private AuctionAdapter mAucionAdapter;

    private boolean mIsPeerToPeerMode = true;
    private String mUserId = "";
    private String mPeerId = "";
    private String mChannelName = "";
    private int mChannelMemberCount = 1;

    private ClientManager mChatManager;
    private RtmClient mRtmClient;
    private RtmClientListener mClientListener;
    private RtmChannel mRtmChannel;

    private RtmMetadata mMetadata;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_auction);
        init();
    }

    private void init() {
        mChatManager = AGApplication.the().getChatManager();
        mRtmClient = mChatManager.getRtmClient();
        mClientListener = new MyRtmClientListener();
        mChatManager.registerListener(mClientListener);
        Intent intent = getIntent();
        mChannelName = intent.getStringExtra(MessageUtil.INTENT_EXTRA_CHANNEL_NAME);
        mUserId = intent.getStringExtra(MessageUtil.INTENT_EXTRA_USER_ID);
//
//        mTitleTextView = findViewById(R.id.message_title);
//        if (mIsPeerToPeerMode) {
//            mPeerId = targetName;
//            mTitleTextView.setText(mPeerId);
//
//            // load history chat records
//            MessageListBean messageListBean = MessageUtil.getExistMessageListBean(mPeerId);
//            if (messageListBean != null) {
//                mMessageBeanList.addAll(messageListBean.getMessageBeanList());
//            }
//
//            // load offline messages since last chat with this peer.
//            // Then clear cached offline messages from message pool
//            // since they are already consumed.
//            MessageListBean offlineMessageBean = new MessageListBean(mPeerId, mChatManager);
//            mMessageBeanList.addAll(offlineMessageBean.getMessageBeanList());
//            mChatManager.removeAllOfflineMessages(mPeerId);
//        } else {
//            mChannelName = targetName;
//            mChannelMemberCount = 1;
//            mTitleTextView.setText(MessageFormat.format("{0}({1})", mChannelName, mChannelMemberCount));
//
//        }

        createAndJoinChannel();
        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        layoutManager.setOrientation(RecyclerView.VERTICAL);
        mAucionAdapter = new AuctionAdapter(this, mAuctionBeanList, new AuctionAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(AuctionBean bean, String amount) {
                JSONObject json = new JSONObject();
                try {
                    json.put("id", bean.getId());
                    json.put("uid", mUserId);
                    json.put("amount", amount);
                }catch (JSONException e)
                {

                }

                mChatManager.setBid(FormBody.create(MediaType.parse("application/json"), String.valueOf(json)),
                        new Callback() {
                            @Override
                            public void onFailure(@NonNull Call call, @NonNull IOException e) {
                                showToast(e.getMessage());
                            }

                            @Override
                            public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {
                                runOnUiThread(() -> {
                                    try {
                                        String stt = response.body().string();
                                        JSONObject res = new JSONObject(stt);
                                        showToast(res.getString("reason"));
                                    }
                                    catch (Exception e)
                                    {
                                        showToast(e.getMessage());
                                    }

                                });
                            }
                        });
            }
        });


        mRecyclerView = findViewById(R.id.message_list);
        mRecyclerView.setLayoutManager(layoutManager);
        mRecyclerView.setAdapter(mAucionAdapter);

        //mMsgEditText = findViewById(R.id.message_edittiext);
        //mBigImage = findViewById(R.id.big_image);
    }



    @Override
    protected void onDestroy() {
        super.onDestroy();

        leaveAndReleaseChannel();

        mChatManager.unregisterListener(mClientListener);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == CropImage.CROP_IMAGE_ACTIVITY_REQUEST_CODE) {
            CropImage.ActivityResult result = CropImage.getActivityResult(data);
            if (resultCode == RESULT_OK) {

            } else if (resultCode == CropImage.CROP_IMAGE_ACTIVITY_RESULT_ERROR_CODE) {
                result.getError().printStackTrace();
            }
        }
    }


    public void onClickFinish(View v) {
        finish();
    }


    /**
     * API CALL: create and join channel
     */
    private void createAndJoinChannel() {
        // step 1: create a channel instance
        mRtmChannel = mRtmClient.createChannel(mChannelName, new MyChannelListener());
        if (mRtmChannel == null) {
            showToast(getString(R.string.join_channel_failed));
            finish();
            return;
        }

        Log.e("channel", mRtmChannel + "");

        // step 2: join the channel
        mRtmChannel.join(new ResultCallback<Void>() {
            @Override
            public void onSuccess(Void responseInfo) {
                Log.i(TAG, "join channel success");
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                Log.e(TAG, "join channel failed");
                runOnUiThread(() -> {
                    showToast(getString(R.string.join_channel_failed));
                    finish();
                });
            }
        });
    }

    private void syncChannelMetadata(RtmMetadata metadata, RtmMetadataOptions options) {
        Log.e(TAG, "deleteChannelMetadata");

        mRtmChannel.deleteChannelMetadata(metadata.items, options, null );
    }

    /**
     * API CALL: leave and release channel
     */
    private void leaveAndReleaseChannel() {
        if (mRtmChannel != null) {
            mRtmChannel.leave(null);
            mRtmChannel.release();
            mRtmChannel = null;
        }
    }

    /**
     * API CALLBACK: rtm event listener
     */
    class MyRtmClientListener implements RtmClientListener {

        @Override
        public void onConnectionStateChanged(final int state, int reason) {
            runOnUiThread(() -> {
                switch (state) {
                    case RtmStatusCode.ConnectionState.CONNECTION_STATE_RECONNECTING:
                        showToast(getString(R.string.reconnecting));
                        break;
                    case RtmStatusCode.ConnectionState.CONNECTION_STATE_ABORTED:
                        showToast(getString(R.string.account_offline));
                        setResult(MessageUtil.ACTIVITY_RESULT_CONN_ABORTED);
                        finish();
                        break;
                }
            });
        }

        @Override
        public void onMessageReceived(final RtmMessage message, final String peerId) {

        }

        @Override
        public void onTokenExpired() {

        }

        @Override
        public void onTokenPrivilegeWillExpire() {

        }

        @Override
        public void onPeersOnlineStatusChanged(Map<String, Integer> map) {

        }

        @Override
        public void onUserMetadataUpdated(String s, RtmMetadata rtmMetadata) {

        }
    }

    /**
     * API CALLBACK: rtm channel event listener
     */
    class MyChannelListener implements RtmChannelListener {
        @Override
        public void onMemberCountUpdated(int i) {

        }

        @Override
        public void onAttributesUpdated(List<RtmChannelAttribute> list) {

        }

        @Override
        public void onMessageReceived(final RtmMessage message, final RtmChannelMember fromMember) {

        }

        @Override
        public void onMemberJoined(RtmChannelMember member) {

        }

        @Override
        public void onMemberLeft(RtmChannelMember member) {

        }

        @Override
        public void onMetadataUpdated(RtmMetadata rtmMetadata) {
            Log.i(TAG, "onMetadataUpdated revision: " + rtmMetadata.getMajorRevision());
            mMetadata = rtmMetadata;

            getAuctionBeanFromMetedata(mMetadata.items, "auction");

            runOnUiThread(() -> {
                if (mAuctionBeanList.size() > 0) {

                    mAucionAdapter.setItems(mAuctionBeanList);

                    mAucionAdapter.notifyDataSetChanged();
                    mRecyclerView.scrollToPosition(mAuctionBeanList.size() - 1);
                }
            });
        }

        private void getAuctionBeanFromMetedata(List<RtmMetadataItem> items, String key)
        {
            boolean find = true;
            String json = null;
            for (RtmMetadataItem item: items) {
                Log.i(TAG, "Key:"+item.getKey());
                if(item.getKey().equals(key)) {
                    json = item.getValue();
                    find = true;
                    break;
                }
            }

            if(find) {
                Log.i(TAG, "find auction:"+json);
                Gson gson = new Gson();
                mAuctionBeanList = gson.fromJson(json, new TypeToken<List<AuctionBean>>(){}.getType());

                for(AuctionBean bean : mAuctionBeanList)
                {
                    Log.i(TAG, "owner:"+bean.getOwner());
                }
            }
            else {
                Log.i(TAG, "no auction json");
            }

        }
    }


    private void showToast(final String text) {
        runOnUiThread(() -> Toast.makeText(AuctionActivity.this, text, Toast.LENGTH_SHORT).show());
    }
}
