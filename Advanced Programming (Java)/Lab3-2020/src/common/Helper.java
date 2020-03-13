package common;

import java.math.BigDecimal;

public class Helper {
    public static String roundToNDecimals(double number, int decimals) {
        return BigDecimal.valueOf(number).setScale(decimals, BigDecimal.ROUND_HALF_UP).toPlainString();
    }
}
