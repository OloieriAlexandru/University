package mdvsp.common;

import java.util.Objects;

public class Client {
    private String name;
    private int visitingTime;

    public Client(String clientName, int clientVisitingTime){
        name = clientName;
        visitingTime = clientVisitingTime;
    }

    @Override
    public String toString() {
        return "Client{" +
                "name='" + name + '\'' +
                ", visitingTime=" + visitingTime +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Client client = (Client) o;
        return Objects.equals(name, client.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name);
    }

    public int getVisitingTime() {
        return visitingTime;
    }

    public void setVisitingTime(int visitingTime) {
        this.visitingTime = visitingTime;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
