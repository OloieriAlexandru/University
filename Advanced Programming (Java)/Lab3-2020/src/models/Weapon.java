package models;

import abstractions.Item;

public class Weapon implements Item {
    private static final String[] weaponNames = new String[]{"Sword", "Knife", "Awp"};
    private WeaponType weaponType = null;
    private int weight;
    private double value;

    public Weapon(WeaponType weaponType, int weight, double value) {
        this.weaponType = weaponType;
        this.weight = weight;
        this.value = value;
    }

    @Override
    public String getName() {
        if (weaponType == null) {
            return "Unknown";
        }
        return weaponNames[weaponType.getValue()];
    }

    @Override
    public double profitFactor() {
        return Math.round((getValue() / getWeight()) * 100.0) / 100.0;
    }

    @Override
    public double getValue() {
        return value;
    }

    @Override
    public int getWeight() {
        return weight;
    }

    @Override
    public String toString() {
        return "Weapon: " + getName() + ", weight = " + getWeight() + ", value = " + getValue() + " (profit factor = " + profitFactor() + ")";
    }
}
