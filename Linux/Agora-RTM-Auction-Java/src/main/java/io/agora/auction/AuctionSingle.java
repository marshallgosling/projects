package io.agora.auction;


import java.text.SimpleDateFormat;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.FileHandler;
import java.util.logging.Formatter;
import java.util.logging.LogRecord;
import java.util.logging.Logger;
import java.util.Date;
import java.util.HashMap;

import io.agora.rtm.RtmClient;

import io.agora.rtm.RtmMetadata;
import io.agora.rtm.RtmMetadataItem;
import io.agora.rtm.RtmMetadataOptions;
import io.agora.rtm.ErrorInfo;
import io.agora.rtm.ResultCallback;
import io.agora.rtm.RtmChannel;

import java.net.URI;
import java.net.URLEncoder;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.util.Map;
import java.util.Locale.Category;

import redis.clients.jedis.Jedis;

public class AuctionSingle extends java.lang.Thread {

    private String rtmTokenUrl =  CONFIG.BASE_URL + "api/v1/rtmtoken?nojson=1&uid=";
    private String channelUrl = CONFIG.BASE_URL + "api/v1/channel/";
    private String syncUrl = CONFIG.BASE_URL + "api/v1/auction/sync";

    private Jedis jedis;
    private String appId;
    private String userId;
    private String channelId;

    private RtmClient mRtmClient;
    private RtmChannel mRtmChannel;
    private boolean loginStatus = false;
    private boolean joinStatus = false;

    private boolean exitCode = false;
    private MyChannelListener channelListener;
    private MyClientListener clientListener;
    private Logger logger;
    HttpClient client;
    HttpRequest req;
    private int failedLoginCount = 0;
    private int lastCheckTime = 0;

    private String channelStatus = "";

    private String key;

    public AuctionSingle(String appid, String userid, String channelid)
    {
        appId = appid;
        userId = userid;
        channelId = channelid;
        key = CONFIG.REDIS_KEY + channelId;

        initLog();

        writeLog("Init thread with channelid: "+channelId, Level.INFO);

        client = HttpClient.newBuilder().build();
        req = HttpRequest.newBuilder(URI.create(channelUrl+channelId)).build();

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
            clientListener = new MyClientListener();
            mRtmClient = RtmClient.createInstance(appId, clientListener);
            clientListener.baseUrl = rtmTokenUrl;
            clientListener.userId = userId;
            clientListener.rtmClient = mRtmClient;
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
            SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-M-dd");
            String date = simpleDateFormat.format(new Date());
            FileHandler fileHandler = new FileHandler(CONFIG.LOG_DIR + "channel_" + channelId + "_" + date + ".log", true);
        
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
        String data;
        while (!exitCode) {
            if (isOffline()) {

                if (loginStatus || joinStatus) {
                    writeLog("Channel "+channelId+" is offline. so logout.", Level.INFO);
                    logout();
                }
                try {
                    sleep(5000);
                }
                catch(Exception e)
                {

                }
            }
            else {
                if (!channelStatus.equals("online")) continue;

                if (!loginStatus) {
                    writeLog("Channel "+channelId+" is online. Start login.", Level.INFO);
                    startRTM();
                    writeLog("Waiting for Redis key metadata:"+key, Level.INFO);
                    if(loginStatus && joinStatus) {
                        syncAuction(channelId, "1");
                    }
                }

                if(loginStatus && joinStatus) {
                    
                    try {
                        data = jedis.get(key);
                        jedis.set(key, "");
                        if (data.length() > 0) {                      
                            writeLog("Read Metadata from Redis: "+data, Level.INFO);
                
                            syncChannelMetadata("auction", data);
                        }                       
                        else {
                            sleep(100);
                        }       
                    }
                    catch (Exception e) {
            
                    }
                }
                else {
                    failedLoginCount += 1;

                    if (failedLoginCount == 10) {
                        writeLog("Failed login times over 10:", Level.SEVERE);
                    
                        break;
                    }
                }

                
        
            }


        }


        logout();
    }

    private void startRTM() {
        String token = clientListener.getToken();

        writeLog("Rtm login start!", Level.INFO);
        login(token);
        try {
            sleep(1000);
        }
        catch (Exception e) {}

        writeLog("Rtm join channel start!", Level.INFO);
        joinChannel();

        try {
            sleep(1000);
        }
        catch (Exception e) {}


    }

    private boolean isOffline()
    {
        if (lastCheckTime > 0) {
            lastCheckTime --;
            return false;
        }
        lastCheckTime = 10;
        try {
            HttpResponse<String> resp = client.send(req, HttpResponse.BodyHandlers.ofString());
            String body = resp.body().trim();
            channelStatus = body;
            return !body.equals("online");
        }
        catch(Exception e)
        {
            return false;
        }
    }

    private void syncAuction(String channelid, String status)
    {
        Map<String, String> data = new HashMap<String, String>();
        data.put("channelid", channelid);
        data.put("status", status);
        try {
            HttpRequest sync = HttpRequest.newBuilder(URI.create(syncUrl))
            .POST(buildFormDataFromMap(data))
            .header("Content-Type", "application/x-www-form-urlencoded")
            .build();

            client.send(sync, HttpResponse.BodyHandlers.ofString());
            
            
        }catch(Exception e)
        {

        }

    }

    private static HttpRequest.BodyPublisher buildFormDataFromMap(Map<String, String> data)
    {
        StringBuilder builder = new StringBuilder();
        for (Map.Entry<String, String> entry: data.entrySet()) {
            if (builder.length() > 0) {
                builder.append("&");
            }
            builder.append(URLEncoder.encode(entry.getKey(), StandardCharsets.UTF_8));
            builder.append("=");
            builder.append(URLEncoder.encode(entry.getValue(), StandardCharsets.UTF_8));
        }

        return HttpRequest.BodyPublishers.ofString(builder.toString());
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
        if(joinStatus) {
            joinStatus = false;
            mRtmChannel.leave(null);
            mRtmChannel.release();
        }

        if(loginStatus) {
            loginStatus = false;
            mRtmClient.logout(null);
            //mRtmClient.release();
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
                joinStatus = true;
            }

            @Override
            public void onFailure(ErrorInfo errorInfo) {
                writeLog("join channel failure! errorCode = " + errorInfo.getErrorCode(), Level.WARNING);
                joinStatus = false;
            }
        });

        return true;
    }

    private void syncChannelMetadata(String key, String data) {
        RtmMetadata metadata = channelListener.metadata;
        RtmMetadataOptions options = new RtmMetadataOptions();
        options.setEnableRecordTs(true);
        options.setEnableRecordUserId(true);

        
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
        }
        else {
            boolean found = false;
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

class LaravelFormatter extends Formatter {

    private SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-M-dd hh:mm:ss");

    @Override
    public String format(LogRecord record) {
        
        return "["+simpleDateFormat.format(new Date(record.getMillis()))+"] local."
            +record.getLevel().getName()+":"
            +record.getMessage()+"\n";
    }

}