package mdvsp.optional;

public class Truck extends Vehicle {
    public Truck(String name) {
        super(name);
    }

    @Override
    public String toString() {
        return super.toString() + ", type: Truck";
    }

    @Override
    public boolean equals(Object o) {
        return super.equals(o);
    }
}
