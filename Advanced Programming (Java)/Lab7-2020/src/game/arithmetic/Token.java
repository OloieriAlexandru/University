package game.arithmetic;

public class Token implements Comparable {
    private int value;

    public Token(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    /**
     * The class needs to implement the Comparable interface in order to be used as a generic argument for TreeSet
     */
    @Override
    public int compareTo(Object o) {
        if (this == o) {
            return 0;
        }
        if (o == null || getClass() != o.getClass()) {
            return 1;
        }
        return Integer.compare(value, ((Token) o).value);
    }
}
