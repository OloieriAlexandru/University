package knapsack;

import abstractions.Item;
import common.RandomGenerator;
import models.*;

public class ItemFactory {
    public static Item getItem(int type, int minWeight, int maxWeight, int minValue, int maxValue) {
        Item item = null;
        switch (type) {
            case 0:
                item = new Weapon(WeaponType.AWP, RandomGenerator.intBetween(minWeight, maxWeight), RandomGenerator.doubleBetween(minValue, maxValue));
                break;
            case 1:
                item = new Book("Random generated book", RandomGenerator.intBetween(minWeight, maxWeight) * 100, RandomGenerator.doubleBetween(minValue, maxValue));
                break;
            case 2:
                item = new Food("Random generated food", RandomGenerator.intBetween(minWeight, maxWeight));
                break;
        }
        return item;
    }
}
