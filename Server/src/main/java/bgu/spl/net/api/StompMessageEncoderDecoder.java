package bgu.spl.net.api;

import bgu.spl.net.srv.StompMessage;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class StompMessageEncoderDecoder implements MessageEncoderDecoder<StompMessage> {

    private byte[] bytes = new byte[1 << 10];
    private int length = 0;

    @Override
    public StompMessage decodeNextByte(byte nextByte) {
        if (nextByte == '\u0000') {
            return stringToStomp(popString());
        }
        pushByte(nextByte);
        return null;
    }

    @Override
    public byte[] encode(StompMessage message) {
        return stompToString(message).getBytes();
    }

    private void pushByte(byte nextByte) {
        if(length >= bytes.length) {
            bytes = Arrays.copyOf(bytes, length * 2);
        }
        bytes[length++] = nextByte;
    }

    private String popString() {
        String result = new String(bytes, 0, length);
        length = 0;
        return result;
    }

    private String stompToString(StompMessage sm){
        StringBuilder output = new StringBuilder();
        output.append(sm.getType()).append("\n");
        for(Map.Entry<String, String> header : sm.getHeadersMap().entrySet()) {
            output.append(header.getKey()).append(":").append(header.getValue()).append("\n");
        }
        output.append("\n");
        output.append(sm.getBody()).append("\n");
        output.append("\u0000");
        return output.toString();
    }

    public static StompMessage stringToStomp(String str) {
        StompMessage output = new StompMessage();
        int i = str.indexOf('\n');
        String type = str.substring(0, i);
        output.setType(type);
        str = str.substring(i + 1);
        i = str.indexOf("\n\n");
        String body = str.substring(i + 2);
        body = body.substring(0, body.indexOf('\n'));
        output.setBody(body);
        String headers = str.substring(0, i + 1);
        while((i = headers.indexOf('\n')) >= 0) {
            String header = headers.substring(0, i);
            int colon = header.indexOf(':');
            String key = header.substring(0, colon);
            String value = header.substring(colon + 1);
            output.setHeader(key, value);
            headers = headers.substring(i + 1);
        }
        return output;
    }
}


