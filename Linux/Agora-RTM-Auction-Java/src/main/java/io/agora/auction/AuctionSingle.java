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
import java.util.Date;
import io.agora.rtm.RtmClient;

import io.agora.rtm.RtmMetadata;
import io.agora.rtm.RtmMetadataItem;
import io.agora.rtm.RtmMetadataOptions;
import io.agora.rtm.ErrorInfo;
import io.agora.rtm.ResultCallback;
import io.agora.rtm.RtmChannel;


import redis.clients.jedis.Jedis;

public class AuctionSingle extends java.lang.Thread {

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
    private MyChannelListener channelListener;
    private Logger logger;
    private boolean ready = false;

    private String key;

    public AuctionSingle(String appid, String userid, String channelid)
    {
        appId = appid;
        userId = userid;
        channelId = channelid;
        key = CONFIG.REDIS_KEY + channelId;
        writeLog("Init thread with channelid: "+channelId, Level.INFO);    
        
        initLog();

        try {
            jedis = new Jedis(CONFIG.REDIS_IP, 6379);
            jedis.select(CONFIG.REDIS_DB);
            //jedis.auth(redis_auth);
        }
        catch (Exception e) {
            writeLog("Redis auth error!", Level.WARNING);
            throw new RuntimeException("Need to check redis password");
        }
        try {
            mRtmClient = RtmClient.createInstance(appId, new MyClientListener());
            writeLog("Rtm sdk init succeed!", Level.INFO);
        }
        catch (Exception e) {
            writeLog("Rtm sdk init fatal error!", Level.SEVERE);
            throw new RuntimeException("Need to check rtm sdk init process");
        }
    }

    private void initLog()
    {
        try{
            //日志记录器
            
            logger = Logger.getLogger("agora-"+channelId);
            
            //日志处理器
            
            FileHandler fileHandler = new FileHandler(CONFIG.LOG_DIR + "channel_" + channelId + ".log");
        
            fileHandler.setFormatter(new LaravelFormatter());
            
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

        //writeLog(msg);
    }

    public void exitCode()
    {
        exitCode = true;
    }

    @Override
    public void run() { 

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
            String data;

            writeLog("Ready for Redis key metadata:"+key, Level.INFO);
            try {
                while(!exitCode)
                {
                    if (jedis.exists(key)) {
                        data = jedis.get(key);
                    }
                    else {
                        data = "";
                    }
                    if (data.length() > 0) {
                        jedis.set(key, "");
                                 
                        writeLog("Read Metadata from Redis: "+data, Level.INFO);

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
                writeLog("login success!", Level.INFO);
            }
            //@Override
            public void onFailure(ErrorInfo errorInfo) {
                loginStatus = false;
                writeLog("login failure!", Level.WARNING);
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
        channelListener = new MyChannelListener(channelId);
        mRtmChannel = mRtmClient.createChannel(channelId, channelListener);
        if (mRtmChannel == null) {
            writeLog("channel created failed!", Level.SEVERE);
            return false;
        }
        mRtmChannel.join(new ResultCallback<Void>() {
            @Override
            public void onSuccess(Void responseInfo) {
                writeLog("join channel success!", Level.INFO);
                channelStatus = true;
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                writeLog("join channel failure! errorCode = " + errorInfo.getErrorCode(), Level.WARNING);
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

        writeLog("Start Send Metadata.", Level.INFO);
        mRtmChannel.updateChannelMetadata(metadata.items, options, new ResultCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                writeLog("Send Metadata Succeed", Level.INFO);
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                final int errorCode = errorInfo.getErrorCode();
                writeLog("Send Metadata to channel failed, erroCode = " + errorCode, Level.WARNING);
            }
        });
    } 
}

class LaravelFormatter extends Formatter {

    private SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-M-dd hh:mm:ss");

    @Override
    public String format(LogRecord record) {
        
        return "["+simpleDateFormat.format(new Date(record.getMillis()))+"] local."
            +record.getLevel().getName()+":"
            +record.getMessage()+"\n";
    }

}