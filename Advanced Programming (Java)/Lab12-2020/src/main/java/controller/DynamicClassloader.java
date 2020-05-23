package controller;

import java.net.URL;
import java.net.URLClassLoader;

public class DynamicClassloader extends URLClassLoader {
    public DynamicClassloader() {
        super(new URL[0], ClassLoader.getSystemClassLoader());
    }

    @Override
    public void addURL(URL url) {
        super.addURL(url);
    }
}
