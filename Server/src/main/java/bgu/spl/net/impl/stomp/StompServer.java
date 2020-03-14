package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocolImpl;
import bgu.spl.net.srv.Server;
import bgu.spl.net.srv.StompMessage;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class StompServer {

    public static void main(String[] args) {
        int port = Integer.parseInt(args[0]);
        String serverType = args[1];
        if(serverType.equals("tpc")) {
            Server.threadPerClient(
                    port,
                    StompMessagingProtocolImpl::new,
                    StompMessageEncoderDecoder::new).serve();
        } else if(serverType.equals("reactor")) {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    port,
                    StompMessagingProtocolImpl::new,
                    StompMessageEncoderDecoder::new).serve();
        }
    }
}
