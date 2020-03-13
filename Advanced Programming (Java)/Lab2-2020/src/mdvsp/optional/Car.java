package mdvsp.optional;

public class Car extends Vehicle {
    public Car(String name) {
        super(name);
    }

    @Override
    public String toString() {
        return super.toString() + ", type: Car";
    }

    @Override
    public boolean equals(Object o) {
        return super.equals(o);
    }
}
