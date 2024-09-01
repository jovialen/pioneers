package com.github.jovialen.pioneers.networking.server;

import com.github.jovialen.pioneers.networking.client.Client;
import org.tinylog.Logger;

import java.io.IOException;
import java.net.Socket;

public abstract class Acceptor {
    public Thread start(Server server) {
        Thread thread = new Thread(() -> run(server));
        thread.start();
        return thread;
    }

    public void run(Server server) {
        while (server.isOpen()) {
            try {
                Logger.trace("Waiting for client to connect to {}", server);
                Socket remote = server.getSocket().accept();
                Client connectingClient = new Client(server.getEventBus(), remote);

                Logger.debug("{} is attempting to connect to the server", connectingClient);
                if (authenticateClient(connectingClient)) {
                    server.connect(connectingClient);
                } else {
                    Logger.warn("Client was refused");
                    connectingClient.disconnect();
                }
            } catch (IOException e) {
                Logger.warn("Failed to accept client for server {}: {}", server, e);
            }
        }
        Logger.debug("No longer waiting for clients to connect to the server");
    }

    public abstract boolean authenticateClient(Client client);
    public abstract boolean authenticateServer(Client client);
}
