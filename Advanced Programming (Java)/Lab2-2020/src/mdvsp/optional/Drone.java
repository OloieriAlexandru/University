package mdvsp.optional;

public class Drone extends Vehicle {
    public Drone(String name) {
        super(name);
    }

    @Override
    public String toString() {
        return super.toString() + ", type: Drone";
    }

    @Override
    public boolean equals(Object o) {
        return super.equals(o);
    }
}
