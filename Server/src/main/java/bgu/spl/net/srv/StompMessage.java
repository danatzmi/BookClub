package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.Map;

public class StompMessage {

    private String type;
    private Map<String, String> headersMap;
    private String body;

    public StompMessage() {
        type = "";
        headersMap = new HashMap<>();
        body = "";
    }

    public StompMessage(StompMessage other) {
        this.type = other.type;
        this.headersMap = new HashMap<>();
        for (Map.Entry<String, String> p : other.headersMap.entrySet()) {
            this.setHeader(p.getKey(), p.getValue());
        }
        this.body = other.body;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setHeader(String k, String v) {
        this.headersMap.put(k, v);
    }

    public void setBody(String body) {
        this.body = body;
    }

    public String getBody() { return body; }

    public String getType() {
        return type;
    }

    public String getHeader(String key) {
        return headersMap.get(key);
    }

    public Map<String, String> getHeadersMap() {
        return headersMap;
    }

    @Override
    public String toString() {
        StringBuilder output = new StringBuilder();
        output.append(type).append("\n");
        for(Map.Entry<String, String> header : headersMap.entrySet()) {
            output.append(header.getKey()).append(":").append(header.getValue()).append("\n");
        }
        output.append("\n");
        output.append(body).append("\n");
        output.append("\u0000");
        return output.toString();
    }
}
