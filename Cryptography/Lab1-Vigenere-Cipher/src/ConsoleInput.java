import java.util.Scanner;

public class ConsoleInput implements IInput {
    @Override
    public String readInput() {
        System.out.println("Enter the text: ");

        Scanner scanner = new Scanner(System.in);

        return scanner.nextLine();
    }
}
