package io.agora.auction;

import io.agora.rtm.RtmChannelListener;
import io.agora.rtm.RtmChannelMember;
import io.agora.rtm.RtmMetadata;
import io.agora.rtm.RtmChannelAttribute;
import io.agora.rtm.RtmMessage;


import java.util.List;

public class MyChannelListener implements RtmChannelListener {
    private String channel_;
    public RtmMetadata metadata;

    public MyChannelListener(String channel) {
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
        System.out.println("onMetadataUpdated. "+channel_);
    }

    
}