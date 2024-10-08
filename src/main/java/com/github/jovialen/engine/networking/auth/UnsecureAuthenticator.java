package com.github.jovialen.engine.networking.auth;

import com.github.jovialen.engine.networking.client.Client;
import com.github.jovialen.engine.networking.server.Server;

public class UnsecureAuthenticator implements Authenticator {
    @Override
    public boolean authenticateClient(Server server, Client client) {
        return true;
    }

    @Override
    public boolean authenticateServer(Client client) {
        return true;
    }
}
