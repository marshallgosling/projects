package io.agora.model;

import java.util.ArrayList;
import java.util.List;

import io.agora.rtm.RtmMessage;
import io.agora.rtmtutorial.ClientManager;

public class AuctionListBean {
    private String channelId;
    private List<AuctionBean> auctionBeanList;

    public AuctionListBean(String channelId, List<AuctionBean> auctionBeanList) {
        this.channelId = channelId;
        this.auctionBeanList = auctionBeanList;
    }

    public String getChannelId() {
        return channelId;
    }

    public void setChannelId(String channelId) {
        this.channelId = channelId;
    }

    public List<AuctionBean> getAuctionBeanList() {
        return auctionBeanList;
    }

    public void setAuctionBeanList(List<AuctionBean> auctionBeanList) {
        this.auctionBeanList = auctionBeanList;
    }
}
