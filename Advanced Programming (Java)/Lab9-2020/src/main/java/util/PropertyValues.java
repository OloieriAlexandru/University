package util;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

// https://crunchify.com/java-properties-file-how-to-read-config-properties-values-in-java/
public class PropertyValues {
    public String getDBConnectionType() {
        String res = null;
        try {
            Properties prop = new Properties();
            String propFileName = "config.properties";

            InputStream inputStream = inputStream = getClass().getClassLoader().getResourceAsStream(propFileName);

            if (inputStream == null) {
                return "error";
            }

            prop.load(inputStream);
            res = prop.getProperty("dbType");
        } catch (IOException e) {
            System.out.println(e.getMessage());
            return "error";
        }
        return res;
    }
}
