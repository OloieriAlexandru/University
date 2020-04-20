package entity;

public enum GenreEnum {
    ROCK(1),
    R_B(2),
    POP(3),
    LATIN(4),
    JAZZ(5),
    HIP_HOP(6),
    FOLK(7),
    FLAMENCO(8),
    ELECTRONIC(9),
    COUNTRY(10),
    BLUES(11);

    private final int value;

    GenreEnum(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}
