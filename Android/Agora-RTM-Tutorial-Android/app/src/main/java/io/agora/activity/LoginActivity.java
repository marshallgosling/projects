package io.agora.activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;

import java.io.IOException;

import io.agora.rtm.ErrorInfo;
import io.agora.rtm.ResultCallback;
import io.agora.rtm.RtmClient;
import io.agora.rtmtutorial.AGApplication;
import io.agora.rtmtutorial.ClientManager;
import io.agora.rtmtutorial.R;
import io.agora.utils.MessageUtil;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class LoginActivity extends Activity {
    private final String TAG = LoginActivity.class.getSimpleName();

    private TextView mLoginBtn;
    private EditText mUserIdEditText;
    private EditText mChannelEditText;
    private String mUserId;
    private String mChannelName;

    private boolean mIsInChat = false;
    private ClientManager mChatManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        mUserIdEditText = findViewById(R.id.user_id);
        mLoginBtn = findViewById(R.id.button_login);
        mChannelEditText = findViewById(R.id.channel_name);
        mChatManager = AGApplication.the().getChatManager();

    }

    public void onClickLogin(View v) {
        mUserId = mUserIdEditText.getText().toString();
        mChannelName = mChannelEditText.getText().toString();
        if (mUserId.equals("")) {
            showToast(getString(R.string.account_empty));
        } else if (mUserId.length() > MessageUtil.MAX_INPUT_NAME_LENGTH) {
            showToast(getString(R.string.account_too_long));
        } else if (mUserId.startsWith(" ")) {
            showToast(getString(R.string.account_starts_with_space));
        } else if (mUserId.equals("null")) {
            showToast(getString(R.string.account_literal_null));
        } else if (mChannelName.length() == 0) {
            showToast(getString(R.string.channel_name_empty));
        }
        else {
            mLoginBtn.setEnabled(false);
            //doLogin();
            mChatManager.setUserId(mUserId);


            getToken();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        mLoginBtn.setEnabled(true);
        if (mIsInChat) {
            doLogout();
        }
    }

    private void getToken() {
        Log.i(TAG, "test is here");
        mChatManager.getToken(new Callback() {
            @Override
            public void onFailure(@NonNull Call call, @NonNull IOException e) {
                Log.i(TAG, "get token failed "+ e.getMessage());
                runOnUiThread(() -> {
                    mLoginBtn.setEnabled(true);
                    mIsInChat = false;
                    showToast(getString(R.string.login_failed));
                });
            }

            @Override
            public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {
                String token = response.body().string();
                Log.i(TAG, "get token : "+ token);
                doLogin(token);
            }
        });
    }

    /**
     * API CALL: login RTM server
     */
    private void doLogin(String token) {
        mIsInChat = true;
        //String token = getString(R.string.agora_token);
        mChatManager.doLogin(token, new ResultCallback<Void>() {
            @Override
            public void onSuccess(Void responseInfo) {
                Log.i(TAG, "login success");
                mChatManager.isLogin = true;
                runOnUiThread(() -> {
                    Intent intent = new Intent(LoginActivity.this, AuctionActivity.class);
                    intent.putExtra(MessageUtil.INTENT_EXTRA_USER_ID, mUserId);
                    intent.putExtra(MessageUtil.INTENT_EXTRA_CHANNEL_NAME, mChannelName);
                    startActivity(intent);
                });
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                Log.i(TAG, "login failed: " + errorInfo.getErrorCode());
                mChatManager.isLogin = false;
                runOnUiThread(() -> {
                    mLoginBtn.setEnabled(true);
                    mIsInChat = false;
                    showToast(getString(R.string.login_failed));
                });
            }
        });
    }

    /**
     * API CALL: logout from RTM server
     */
    private void doLogout() {
        mChatManager.doLogout(null);
        mChatManager.isLogin = false;
        MessageUtil.cleanMessageListBeanList();
    }

    private void showToast(String text) {
        Toast.makeText(this, text, Toast.LENGTH_SHORT).show();
    }
}
