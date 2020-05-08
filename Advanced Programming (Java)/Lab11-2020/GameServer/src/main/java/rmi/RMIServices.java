package rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface RMIServices extends Remote {
    List<String> getAvailableCommands(boolean loggedIn) throws RemoteException;
}
