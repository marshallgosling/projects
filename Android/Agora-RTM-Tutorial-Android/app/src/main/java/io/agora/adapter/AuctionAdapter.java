package io.agora.adapter;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import android.app.Activity;
import android.content.Context;
import android.media.Image;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;


import com.squareup.picasso.Picasso;

import io.agora.model.AuctionBean;
import io.agora.model.MessageBean;
import io.agora.rtm.RtmMessage;
import io.agora.rtm.RtmMessageType;
import io.agora.rtmtutorial.R;

import java.util.List;

public class AuctionAdapter extends RecyclerView.Adapter<AuctionAdapter.ViewHolder> {

    private List<AuctionBean> auctionBeanList;
    private LayoutInflater inflater;
    private AuctionAdapter.OnItemClickListener listener;
    private String baseUrl;

    public AuctionAdapter(Context context, List<AuctionBean> list, @NonNull AuctionAdapter.OnItemClickListener listener) {
        this.inflater = ((Activity) context).getLayoutInflater();
        this.baseUrl = context.getString(R.string.agora_server) + "/storage/";
        this.auctionBeanList = list;
        this.listener = listener;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = inflater.inflate(R.layout.auction_item_layout, parent, false);
        return new AuctionAdapter.ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(final ViewHolder holder, int position) {
        setupView(holder, position);
    }

    public void setItems(List<AuctionBean> items)
    {
        auctionBeanList = items;
    }

    private void setupView(AuctionAdapter.ViewHolder holder, int position) {
        AuctionBean bean = auctionBeanList.get(position);

        holder.mBidButton.setOnClickListener(v -> {
            if (listener != null) listener.onItemClick(bean, holder.mBidAmount.getText().toString());
        });

        holder.mProductView.setText(bean.getName());
        holder.mOwnerView.setText(bean.getOwner());
        holder.mPriceView.setText(String.format("$%d", bean.getAmount()));
        holder.mBidAmount.setHint(String.format("$%d", bean.getAmount()));

        Picasso.get().load(Uri.parse(baseUrl+bean.getCover())).into(holder.mCoverView);

    }

    public interface OnItemClickListener {
        void onItemClick(AuctionBean message, String amount);
    }

    @Override
    public int getItemCount() {
        return auctionBeanList.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        public final TextView mProductView;
        public final TextView mPriceView;
        public final TextView mOwnerView;
        public final ImageView mCoverView;
        public final EditText mBidAmount;
        public final TextView mBidButton;

        public ViewHolder(View view) {
            super(view);
            mProductView = (TextView) view.findViewById(R.id.item_name);
            mPriceView = (TextView) view.findViewById(R.id.item_price);
            mOwnerView = (TextView) view.findViewById(R.id.item_owner);
            mCoverView = (ImageView) view.findViewById(R.id.item_cover);
            mBidAmount = (EditText) view.findViewById(R.id.bid_amount);
            mBidButton = (TextView) view.findViewById(R.id.button_bid);
        }

        @Override
        public String toString() {
            return super.toString() + " '" + mProductView.getText() + "'";
        }
    }
}