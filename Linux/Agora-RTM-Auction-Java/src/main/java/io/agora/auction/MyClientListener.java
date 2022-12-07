package io.agora.auction;

import io.agora.rtm.RtmClient;
import io.agora.rtm.RtmClientListener;
import io.agora.rtm.RtmMessage;
import io.agora.rtm.RtmMetadata;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.Map;


public class MyClientListener implements RtmClientListener {

    String baseUrl;
    String userId;
    RtmClient rtmClient;

    @Override
    public void onConnectionStateChanged(int state, int reason) {
        
    }

    @Override
    public void onMessageReceived(RtmMessage rtmMessage, String peerId) {
        
    }

    @Override
    public void onTokenExpired() {
        String token = getToken();
        rtmClient.renewToken(token, null);
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

    public String getToken() {
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
}