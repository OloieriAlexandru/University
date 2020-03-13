package models;

import abstractions.Item;

public class Food implements Item {
    private String name;
    private int weight;

    public Food(String name, int weight) {
        this.name = name;
        this.weight = weight;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public double profitFactor() {
        return Math.round((getValue() / getWeight()) * 100.0) / 100.0;
    }

    @Override
    public int getWeight() {
        return weight;
    }

    @Override
    public double getValue() {
        return getWeight() * 2;
    }

    @Override
    public String toString() {
        return "Food: " + getName() + ", weight = " + getWeight() + ", value = " + getValue() + " (profit factor = " + profitFactor() + ")";
    }
}
