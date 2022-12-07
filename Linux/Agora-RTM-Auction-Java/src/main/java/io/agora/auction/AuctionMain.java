package io.agora.auction;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;
import java.util.Properties;
import java.util.ResourceBundle;
import java.util.Scanner;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

class CONFIG {
    public static String APP_ID = "";
    public static String USER_ID = "";
    public static String SERVER_IP = "192.168.1.3";
    public static String REDIS_IP = "127.0.0.1";
    public static String REDIS_KEY = "AUCTION_CHANNEL_";
    public static int REDIS_DB = 6;
    public static String BASE_URL = "";
    public static String LOG_DIR = "/data/agora/logs/";

    private static Properties prop;
    private static String getProperty(String key)
    {
        System.out.println("get property:"+key + " value:"+prop.getProperty(key));
        return prop.getProperty(key);
    }
    public static void setProperties(Properties properties) {
        prop = properties;

        APP_ID = getProperty("agora.appid");
        USER_ID = getProperty("agora.userid");
        REDIS_IP = getProperty("redis.ip");
        REDIS_DB = Integer.parseInt(getProperty("redis.db"));
        REDIS_KEY = getProperty("redis.key");
        LOG_DIR = getProperty("logger.dir");
        BASE_URL = getProperty("server.baseurl");
        SERVER_IP = getProperty("server.ip");
        
    }
}

public class AuctionMain {
    //private Scanner scn;
    private String[] channels;


    private void requestChannels() {
        HttpClient client = HttpClient.newBuilder().build();
        HttpRequest req = HttpRequest.newBuilder(URI.create(CONFIG.BASE_URL + "api/v1/channels?mode=1")).build();
        try {
            HttpResponse<String> resp = client.send(req, HttpResponse.BodyHandlers.ofString());
            String data = resp.body();
            channels = data.split("\n");
        }
        catch(Exception e)
        {
            throw new RuntimeException("Request channels error."+e.getMessage());
        }

        if( channels.length == 0) {
            throw new RuntimeException("No channels availables. exit");
        }

    }

    public String[] getChannels() {
        return channels;
    }

    private void initConfig() {
        Properties prop =new Properties();
       
        try {
            InputStream inputstream = AuctionMain.class.getClassLoader().getResourceAsStream( "io/agora/auction/agora.properties" );
            
            prop.load(inputstream);
            
        }
        catch(IOException e)
        {
        }

        CONFIG.setProperties(prop);

        
    }

    public static void main(String[] args) {

        AuctionMain main = new AuctionMain();
        main.initConfig();
        main.requestChannels();

        String[] channels = main.getChannels();

        HashMap<String, AuctionSingle> threads = new HashMap<String, AuctionSingle>();

        for(int i=0;i<channels.length;i++)
        {
            String channelId = channels[i];

            AuctionSingle at = new AuctionSingle(CONFIG.APP_ID, CONFIG.USER_ID+"_"+channelId, channelId);

            at.start();

            threads.put(channelId, at);
        }
    }

}
