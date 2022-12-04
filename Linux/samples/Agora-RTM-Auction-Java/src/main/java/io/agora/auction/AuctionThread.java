package io.agora.auction;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

import java.util.List;
import java.util.Map;
import io.agora.rtm.RtmClient;
import io.agora.rtm.RtmClientListener;
import io.agora.rtm.RtmMessage;
import io.agora.rtm.RtmMetadata;
import io.agora.rtm.RtmMetadataItem;
import io.agora.rtm.RtmMetadataOptions;
import io.agora.rtm.ErrorInfo;
import io.agora.rtm.ResultCallback;
import io.agora.rtm.RtmChannel;
import io.agora.rtm.RtmChannelListener;
import io.agora.rtm.RtmChannelMember;
import io.agora.rtm.RtmStatusCode;
import io.agora.rtm.RtmChannelAttribute;

import redis.clients.jedis.Jedis;

public class AuctionThread extends java.lang.Thread {

    private String baseUrl =  CONFIG.BASE_URL + "api/v1/rtmtoken?uid=";

    private Jedis jedis;
    private String appId;
    private String userId;
    private String channelId;

    private RtmClient mRtmClient;
    private RtmChannel mRtmChannel;
    private boolean loginStatus = false;
    private boolean channelStatus = false;

    private boolean exitCode = false;
    private ChannelListener channelListener;

    public AuctionThread(String appid, String userid, String channelid)
    {
        appId = appid;
        userId = userid;
        channelId = channelid;

    }

    public void exitCode()
    {
        exitCode = true;
    }

    @Override
    public void run() {

        try {
            jedis = new Jedis(CONFIG.REDIS_IP, 6379);
            //jedis.auth(redis_auth);
        }
        catch (Exception e) {
            System.out.println("Redis auth error!");
            throw new RuntimeException("Need to check redis password");
        }
        try {
            mRtmClient = RtmClient.createInstance(appId, new ClientListener());
            System.out.println("Rtm sdk init succeed!");
        }
        catch (Exception e) {
            System.out.println("Rtm sdk init fatal error!");
            throw new RuntimeException("Need to check rtm sdk init process");
        }

        String token = getToken();

        System.out.println("Rtm login start!");
        login(token);
        try {
            sleep(1000);
        }
        catch (Exception e) {
           
            throw new RuntimeException("sleep error");
        }

        System.out.println("Rtm join channel start!");
        joinChannel();
        
        try {
            sleep(1000);
        }
        catch (Exception e) {
            
            throw new RuntimeException("sleep error");
        }

        
        if (loginStatus && channelStatus) {
            String data;
            String key = CONFIG.REDIS_KEY + channelId;
    
            System.out.println("Ready for Redis key metadata:"+key);
            try {
                while(!exitCode)
                {
                    if (jedis.exists(key)) {
                        data = jedis.get(key);

                        if (data.length() == 0)
                        {
                            continue;
                        }
            
                        jedis.set(key, "");

                        System.out.println("Read Metadata from Redis: "+data);
            
                        sendChannelMetadata("auction", data);
                    }
                    else {
                        sleep(100);
                    }
                    
                }
            }
            catch (Exception e) {
            
                throw new RuntimeException("Redis function error."+e.getMessage());
            }
    
        }

        
        logout();
    }

    private String getToken() {
        HttpClient client = HttpClient.newBuilder().build();
        HttpRequest req = HttpRequest.newBuilder(URI.create(baseUrl+userId)).build();
        try {
            HttpResponse<String> resp = client.send(req, HttpResponse.BodyHandlers.ofString());
            return resp.body();
        }
        catch(Exception e)
        {
            throw new RuntimeException("Request Token error.");
        }
   
    }

    private boolean login(String token) {
        
        mRtmClient.login(token, userId, new ResultCallback<Void>() {
            //@Override
            public void onSuccess(Void responseInfo) {
                loginStatus = true;
                System.out.println("login success!");
            }
            //@Override
            public void onFailure(ErrorInfo errorInfo) {
                loginStatus = false;
                System.out.println("login failure!");
            }
        });
        return true;
    }

    public void logout() {
        if(channelStatus) {
            channelStatus = false;
            mRtmChannel.leave(null);
            mRtmChannel.release();
        }

        if(loginStatus) {
            loginStatus = false;
            mRtmClient.logout(null);
            mRtmClient.release();
        }
        
    }



    private boolean joinChannel() {
        channelListener = new ChannelListener(channelId);
        mRtmChannel = mRtmClient.createChannel(channelId, channelListener);
        if (mRtmChannel == null) {
            System.out.println("channel created failed!");
            return false;
        }
        mRtmChannel.join(new ResultCallback<Void>() {
            @Override
            public void onSuccess(Void responseInfo) {
                System.out.println("join channel success!");
                channelStatus = true;
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                System.out.println("join channel failure! errorCode = " + errorInfo.getErrorCode());
                channelStatus = false;
            }
        });

        return true;
    }

    private void sendChannelMetadata(String key, String data) {

        RtmMetadata metadata = channelListener.metadata;

        for (RtmMetadataItem item : metadata.items) {
            if (item.getKey().equals(key)) {
                item.setValue(data);
            }
        }

        RtmMetadataOptions options = new RtmMetadataOptions();
        options.setEnableRecordTs(true);
        options.setEnableRecordUserId(true);

        System.out.println("Send Metadata start");
        mRtmChannel.updateChannelMetadata(metadata.items, options, new ResultCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                System.out.println("Send Metadata Succeed");
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                final int errorCode = errorInfo.getErrorCode();
                System.out.println("Send Metadata to channel failed, erroCode = " + errorCode);
            }
        });
    } 
}

class ChannelListener implements RtmChannelListener {
    private String channel_;
    public RtmMetadata metadata;

    public ChannelListener(String channel) {
            channel_ = channel;
    }

    @Override
    public void onMemberCountUpdated(int memberCount) {
    }

    @Override
    public void onAttributesUpdated(List<RtmChannelAttribute> attribute) {
    }

    @Override
    public void onMessageReceived(
            final RtmMessage message, final RtmChannelMember fromMember) {

    }

    @Override
    public void onMemberJoined(RtmChannelMember member) {
        String account = member.getUserId();
        System.out.println("member " + account + " joined the channel " + channel_);
    }

    @Override
    public void onMemberLeft(RtmChannelMember member) {
        String account = member.getUserId();
        System.out.println("member " + account + " lefted the channel " + channel_);
    }

    @Override
    public void onMetadataUpdated(RtmMetadata metadata) {
        this.metadata = metadata;
        System.out.println("onMetadataUpdated.");
    }

    
}


class ClientListener implements RtmClientListener {
    @Override
    public void onConnectionStateChanged(int state, int reason) {
        System.out.println("on connection state changed to "
            + state + " reason: " + reason);
    }

    @Override
    public void onMessageReceived(RtmMessage rtmMessage, String peerId) {
        String msg = rtmMessage.getText();
        System.out.println("Receive message: " + msg 
                    + " from " + peerId);
    }

    @Override
    public void onTokenExpired() {
    }

    @Override
    public void onTokenPrivilegeWillExpire() {
    }

    @Override
    public void onPeersOnlineStatusChanged(Map<String, Integer> peersStatus) {
    }

    @Override
    public void onUserMetadataUpdated(String userId, RtmMetadata metadata) {

    }
}