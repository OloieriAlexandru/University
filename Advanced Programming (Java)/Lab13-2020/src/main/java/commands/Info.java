package commands;

import com.fasterxml.jackson.databind.ObjectMapper;
import helper.Helper;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.text.DateFormatSymbols;
import java.text.MessageFormat;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.FormatStyle;
import java.util.*;

public class Info implements Command {
    private static List<Method> infoMethods;

    /**
     * Some code that is used to initialize the array of static methods that print information about a Locale object on the screen
     */
    static {
        String[] methods = new String[]{
                "printCountry",
                "printLanguage",
                "printCurrency",
                "printWeekDays",
                "printMonths",
                "printToday",
                "printOtherInfo"
        };
        infoMethods = new ArrayList<>();

        try {
            Class clazz = Class.forName("commands.Info");
            for (String method : methods) {
                infoMethods.add(clazz.getMethod(method, Locale.class));
            }
        } catch (ClassNotFoundException | NoSuchMethodException e) {
            e.printStackTrace();
        }
    }

    /**
     * Prints information about the current location
     * This is done by iterating through a list of methods created using reflection, and executing them
     */
    public Locale execute(Locale locale) {
        String msg = Helper.loadPropertyFromMessagesFile(locale, "info");
        System.out.println(new MessageFormat(msg).format(new Object[]{locale.getDisplayCountry(locale)}));

        for (Method method : infoMethods) {
            try {
                method.invoke(null, locale);
            } catch (IllegalAccessException | InvocationTargetException e) {
                e.printStackTrace();
            }
        }
        return locale;
    }

    public static void printCountry(Locale locale) {
        System.out.println("Country: " + locale.getDisplayCountry(locale));
    }

    public static void printLanguage(Locale locale) {
        System.out.println("Language: " + locale.getDisplayLanguage(locale));
    }

    public static void printCurrency(Locale locale) {
        System.out.println("Currency: " + Currency.getInstance(locale).getDisplayName(locale));
    }

    public static void printWeekDays(Locale locale) {
        String[] weekDays = new DateFormatSymbols(locale).getWeekdays();
        System.out.print("Week Days: ");
        for (int i = 0; i < weekDays.length; ++i) {
            if (weekDays[i].length() == 0) {
                continue;
            }
            System.out.print(weekDays[i]);
            if (i + 1 < weekDays.length) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }

    public static void printMonths(Locale locale) {
        System.out.print("Months: ");
        String[] months = new DateFormatSymbols(locale).getMonths();
        for (int i = 0; i < months.length; ++i) {
            System.out.print(months[i]);
            if (i + 1 < months.length && months[i + 1].length() > 0) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }

    public static void printToday(Locale locale) {
        DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofLocalizedDate(FormatStyle.FULL).withLocale(locale);
        System.out.println("Today: " + LocalDate.now().format(dateTimeFormatter));
    }

    /**
     * Makes a HTTP GET request to an external service to get more information about the current location
     * Tutorial used: https://www.tutorialspoint.com/apache_httpclient/apache_httpclient_http_get_request.htm
     */
    public static void printOtherInfo(Locale locale) {
        CloseableHttpClient httpClient = HttpClients.createDefault();
        HttpGet httpGet = new HttpGet("http://www.oorsprong.org/websamples.countryinfo/CountryInfoService.wso/FullCountryInfo/JSON/debug?sCountryISOCode=" + locale.getCountry());
        try {
            HttpResponse httpResponse = httpClient.execute(httpGet);
            Scanner sc = new Scanner(httpResponse.getEntity().getContent());
            StringBuilder responseBuilder = new StringBuilder();

            while (sc.hasNext()) {
                responseBuilder.append(sc.nextLine());
            }

            ObjectMapper mapper = new ObjectMapper();
            Map<String, String> map = mapper.readValue(responseBuilder.toString(), Map.class);

            System.out.println("Phone Code: +" + map.get("sPhoneCode"));
            System.out.println("Capital: " + map.get("sCapitalCity"));
            System.out.println("Continent: " + map.get("sContinentCode"));
            System.out.println("Country Flag: " + map.get("sCountryFlag"));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
