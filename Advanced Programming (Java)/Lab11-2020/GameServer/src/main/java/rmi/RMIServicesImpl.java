package rmi;

import cmd.GameServerCommands;

import java.rmi.RemoteException;
import java.util.List;

public class RMIServicesImpl implements RMIServices {
    private GameServerCommands gameServerCommands;

    public RMIServicesImpl(GameServerCommands gameServerCommands) throws RemoteException {
        super();
        this.gameServerCommands = gameServerCommands;
    }

    @Override
    public List<String> getAvailableCommands(boolean loggedIn) throws RemoteException {
        return gameServerCommands.getCommandsDescription(loggedIn);
    }
}
