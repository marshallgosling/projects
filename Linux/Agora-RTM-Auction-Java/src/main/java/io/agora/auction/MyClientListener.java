package io.agora.auction;

import io.agora.rtm.RtmClientListener;
import io.agora.rtm.RtmMessage;
import io.agora.rtm.RtmMetadata;

import java.util.Map;


public class MyClientListener implements RtmClientListener {
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