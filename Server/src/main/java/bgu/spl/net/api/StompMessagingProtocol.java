package bgu.spl.net.api;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.StompMessage;

import java.util.concurrent.atomic.AtomicInteger;

public interface StompMessagingProtocol {
	/**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
    void start(int connectionId, Connections<StompMessage> connections);
    
    void process(StompMessage message);
	
	/**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
}
