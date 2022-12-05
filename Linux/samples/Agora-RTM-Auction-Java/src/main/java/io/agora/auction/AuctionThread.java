package io.agora.auction;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.text.SimpleDateFormat;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.FileHandler;
import java.util.logging.Formatter;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

import java.util.List;
import java.util.Date;
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

class MyFormatter extends Formatter {

    private SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-M-dd hh:mm:ss");

    @Override
    public String format(LogRecord record) {
        
        return "["+simpleDateFormat.format(new Date(record.getMillis()))+"] local."
            +record.getLevel().getName()+":"
            +record.getMessage()+"\n";
    }

}

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
    private Logger logger;
    private boolean ready = false;

    private String key;

    public AuctionThread(String appid, String userid, String channelid)
    {
        appId = appid;
        userId = userid;
        channelId = channelid;
        key = CONFIG.REDIS_KEY + channelId;
        initLog();

        try {
            jedis = new Jedis(CONFIG.REDIS_IP, 6379);
            jedis.select(6);
            //jedis.auth(redis_auth);
        }
        catch (Exception e) {
            writeLog("Redis auth error!", Level.INFO);
            
            throw new RuntimeException("Need to check redis password");
        }
        try {
            mRtmClient = RtmClient.createInstance(appId, new ClientListener());
            writeLog("Rtm sdk init succeed!", Level.INFO);
        }
        catch (Exception e) {
            writeLog("Rtm sdk init fatal error!", Level.INFO);
            throw new RuntimeException("Need to check rtm sdk init process");
        }

        writeLog("Init thread with channelid: "+channelId, Level.INFO);   

    }

    private void initLog()
    {
        try{
            //日志记录器
            
            logger = Logger.getLogger("agora-"+channelId);
            
            //日志处理器
            
            FileHandler fileHandler = new FileHandler(CONFIG.LOG_DIR + "channel_" + channelId + ".log");
        
            fileHandler.setFormatter(new MyFormatter());
            
            //注册处理器          
            logger.addHandler(fileHandler);
                    
        }
        catch(IOException err)
        {
            
            logger = null;
        }
    }

    private void writeLog(String msg, Level level )
    {
        if(logger != null)
        {
            //记录日志消息
            //需要记录的日志消息
            LogRecord lr = new LogRecord(level, msg);
            logger.log(lr);
        }

        //System.out.println(msg);
    }

    public void exitCode()
    {
        exitCode = true;
    }

    public boolean isReady(){
        return ready;
    }

    @Override
    public void run() {

        if (!ready) {
            String token = getToken();

            writeLog("Rtm login start!", Level.INFO);
            login(token);
            try {
                sleep(1000);
            }
            catch (Exception e) {
            
                throw new RuntimeException("sleep error");
            }

            writeLog("Rtm join channel start!", Level.INFO);
            joinChannel();
            
            try {
                sleep(1000);
            }
            catch (Exception e) {
                
                throw new RuntimeException("sleep error");
            }

            if (loginStatus && channelStatus) {
                ready = true;
                syncFirstTimeMetadata();

                try {
                    sleep(1000);
                }
                catch (Exception e) {
                    
                    throw new RuntimeException("sleep error");
                }

                writeLog("Ready for sync Redis key & metadata:"+key, Level.INFO);
            }
        }
        
    
    }

    public void checkMetadata() {
        if( !ready ) {
            try {
                sleep(100);
            }
            catch (Exception e) {
                
                throw new RuntimeException("sleep error");
            }
        }
        String data;
            if (jedis.exists(key)) {
                data = jedis.get(key);

                if (data.length() > 0)
                {
                    jedis.set(key, "");

                    writeLog("Read Metadata from Redis: "+data, Level.INFO);
        
                    sendChannelMetadata("auction", data);
                }

            }
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
                writeLog("login success!", Level.INFO);
            }
            //@Override
            public void onFailure(ErrorInfo errorInfo) {
                loginStatus = false;
                writeLog("login failure!", Level.SEVERE);
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
            writeLog("channel created failed!", Level.INFO);
            return false;
        }
        mRtmChannel.join(new ResultCallback<Void>() {
            @Override
            public void onSuccess(Void responseInfo) {
                writeLog("join channel success!", Level.WARNING);
                channelStatus = true;
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                writeLog("join channel failure! errorCode = " + errorInfo.getErrorCode(), Level.SEVERE);
                channelStatus = false;
            }
        });

        return true;
    }

    private void syncFirstTimeMetadata() {
        HttpClient client = HttpClient.newBuilder().build();

        HttpRequest req = HttpRequest.newBuilder(URI.create(CONFIG.BASE_URL + "api/v1/auction?channelid=" + channelId)).build();
        
        writeLog("Sync metadata on Initialization Ready", Level.INFO);

        try {
            HttpResponse<String> resp = client.send(req, HttpResponse.BodyHandlers.ofString());
            sendChannelMetadata("auction", resp.body());
        }
        catch(Exception e)
        {
            writeLog("Sync init metadata failed", Level.SEVERE);
        }
    }

    private void sendChannelMetadata(String key, String data) {
        RtmMetadataOptions options = new RtmMetadataOptions();
        options.setEnableRecordTs(true);
        options.setEnableRecordUserId(true);

        RtmMetadata metadata = channelListener.metadata;

        writeLog("Major revision:"+metadata.getMajorRevision(), Level.INFO);

        boolean found = false;
        if (metadata.items.isEmpty()) {
            RtmMetadataItem item  = new RtmMetadataItem();
            item.setKey(key);
            item.setValue(data);
            
            metadata.items.add(item);   

            mRtmChannel.addChannelMetadata(metadata.items, options, new ResultCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    writeLog("add Metadata Succeed", Level.INFO);
                }
    
                @Override
                public void onFailure(ErrorInfo errorInfo) {
                    final String errorCode = errorInfo.getErrorDescription();
                    
                    writeLog("Add Metadata to channel failed, erroCode = " + errorCode, Level.SEVERE);
                }
            });

            return;
        }
        else {
            for (RtmMetadataItem item : metadata.items) {
                if (item.getKey().equals(key)) {
                    item.setValue(data);
                    found = true;
                    break;
                }
            }
            if (!found) {
                RtmMetadataItem item  = new RtmMetadataItem();
                item.setKey(key);
                item.setValue(data);
                
                metadata.items.add(item);     
            }

            writeLog("Start Update Metadata", Level.INFO);
            mRtmChannel.updateChannelMetadata(metadata.items, options, new ResultCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    writeLog("Update Metadata Succeed", Level.INFO);
                }

                @Override
                public void onFailure(ErrorInfo errorInfo) {
                    final String errorCode = errorInfo.getErrorDescription();
                    
                    writeLog("Update Metadata to channel failed, erroCode = " + errorCode, Level.SEVERE);
                }
            });
        }

        

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
        
    }

    @Override
    public void onMemberLeft(RtmChannelMember member) {
        
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
        
    }

    @Override
    public void onMessageReceived(RtmMessage rtmMessage, String peerId) {
        
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