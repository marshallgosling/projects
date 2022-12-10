package io.agora.model;

import io.agora.rtm.RtmMessage;

public class AuctionBean {
    private int id;
    private String channelId;
    private int amount;
    private String owner;
    private int status;
    private String name;
    private String cover;

    public AuctionBean(int id, String channelid, String name, String cover, int amount, String owner, int status) {
        this.id = id;
        this.channelId = channelid;
        this.amount = amount;
        this.owner = owner;
        this.status = status;
        this.name = name;
        this.cover = cover;
    }

    public String getChannelId() {
        return channelId;
    }

    public void setChannelId(String channelid) {
        this.channelId = channelid;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getAmount() {
        return amount;
    }

    public void setAmount(int amount) {
        this.amount = amount;
    }

    public String getOwner() {
        return owner;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCover() {
        return cover;
    }

    public void setCover(String cover) {
        this.cover = cover;
    }
}
