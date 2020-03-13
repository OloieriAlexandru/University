package models;

import abstractions.Item;

public class Book implements Item {
    private String name;
    private int numberOfPages;
    private double value;

    public Book(String name, int numberOfPages, double value) {
        this.name = name;
        this.numberOfPages = numberOfPages;
        this.value = value;
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
        return numberOfPages / 100;
    }

    @Override
    public double getValue() {
        return value;
    }

    @Override
    public String toString() {
        return "Book: " + getName() + ", weight = " + getWeight() + ", value = " + getValue() + " (profit factor = " + profitFactor() + ")";
    }
}
