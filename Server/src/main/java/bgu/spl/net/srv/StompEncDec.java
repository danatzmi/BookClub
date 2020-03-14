package bgu.spl.net.srv;

import java.util.Map;

public class StompEncDec {

    public StompEncDec() {}

    public String stompToString(StompMessage sm){
        StringBuilder output = new StringBuilder();
        output.append(sm.getType()).append("\n");
        for(Map.Entry<String, String> header : sm.getHeadersMap().entrySet()) {
            output.append(header.getKey()).append(":").append(header.getValue()).append("\n");
        }
        output.append("\n");
        output.append(sm.getBody());
        output.append("\0");
        return output.toString();
    }

    public StompMessage stringToStomp(String message) {
        StompMessage output = new StompMessage();
        String[] splitMessage = message.split("\n");
        output.setType(splitMessage[0]);
        int index = 1;
        while(splitMessage[index] != "") {
            String[] splitHeader = splitMessage[index].split(":");
            output.setHeader(splitHeader[0], splitHeader[1]);
            index++;
        }
        index++;
        output.setBody(splitMessage[index]);
        return output;
    }
}
