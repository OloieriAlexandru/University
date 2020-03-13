package models;

public enum WeaponType {
    SWORD(0), KNIFE(1), AWP(2);

    private final int value;

    WeaponType(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}
