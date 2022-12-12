package io.agora.rtmtutorial;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import io.agora.model.AuctionBean;

/**
 * Receives and manages messages from RTM engine.
 */
public class AuctionPool {
    private Map<String, List<AuctionBean>> mOfflineAuctionMap = new HashMap<>();

    void insertOfflineAuction(AuctionBean auction, String channelId) {
        boolean contains = mOfflineAuctionMap.containsKey(channelId);
        List<AuctionBean> list = contains ? mOfflineAuctionMap.get(channelId) : new ArrayList<>();

        if (list != null) {
            list.add(auction);
        }

        if (!contains) {
            mOfflineAuctionMap.put(channelId, list);
        }
    }

    List<AuctionBean> getAllOfflineAuction(String channelId) {
        return mOfflineAuctionMap.containsKey(channelId) ?
                mOfflineAuctionMap.get(channelId) : new ArrayList<AuctionBean>();
    }

    void removeAllOfflineAuction(String channelId) {
        mOfflineAuctionMap.remove(channelId);
    }
}
