package io.agora.auction;

import java.util.HashMap;
import java.util.Scanner;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

class CONFIG {
    public static final String APP_ID = "b751e95bce6c41dea55562cb027f0c61";
    public static final String USER_ID = "auction_admin_user";
    public static final String SERVER_IP = "192.168.1.9";
    public static final String REDIS_IP = "192.168.1.9";
    public static final String REDIS_KEY = "AUCTION_CHANNEL_";
    public static final String BASE_URL = "http://192.168.1.9/";
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

            System.out.println("request channels succeed!");
            
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

    public static void main(String[] args) {

        HashMap<String, AuctionThread> threads = new HashMap<String, AuctionThread>();

        AuctionMain main = new AuctionMain();

        main.requestChannels();

        String[] channels = main.getChannels();

        for(int i=0;i<channels.length;i++)
        {
            String channelId = channels[i];
            System.out.println("Init thread with channelid: "+channelId);          
            
            AuctionThread at = new AuctionThread(CONFIG.APP_ID, CONFIG.USER_ID, channelId);

            at.start();

            threads.put(channelId, at);
        }
    }
}