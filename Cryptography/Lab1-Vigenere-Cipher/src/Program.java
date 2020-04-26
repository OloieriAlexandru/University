import java.util.Scanner;

public class Program {
    private static String[] commands =  {"encrypt", "decrypt", "cryptanalysis", "all", "quit"};

    public static void main(String[] args){
        printCommands();
        Scanner scanner = new Scanner(System.in);
        while (true){
            String command = scanner.nextLine();
            if (command.length() == 1 && command.charAt(0) >= '1' && command.charAt(0) <= '5'){
                command = commands[command.charAt(0)-'1'];
            }
            if (command.length() == 0) {
                System.out.println("The command should have at least one character!");
                continue;
            }
            String inputSource = null;
            if (!command.equals("quit")){
                inputSource = readInputSource("Text source (console/file): ");
            }
            if (command.equals("encrypt")){
                Vigenere vigenere = createVigenereNormalObject(inputSource);
                if (!checkVigenereObject(vigenere)){
                    continue;
                }

                vigenere.readKey();
                vigenere.encrypt();
            } else if (command.equals("decrypt")){
                Vigenere vigenere = createVigenereEncryptedObject(inputSource);
                if (!checkVigenereObject(vigenere)){
                    continue;
                }

                vigenere.readKey();
                vigenere.decrypt();
            } else if (command.equals("cryptanalysis")){
                Vigenere vigenere = createVigenereEncryptedObject(inputSource);
                if (!checkVigenereObject(vigenere)){
                    continue;
                }

                vigenere.cryptanalysis();
            } else if (command.equals("all")){
                Vigenere vigenere = createVigenereNormalObject(inputSource);
                if (!checkVigenereObject(vigenere)){
                    continue;
                }

                vigenere.readKey();
                vigenere.encrypt();
                vigenere.decrypt();
                vigenere.cryptanalysis();
            } else if (command.equals("quit")){
                break;
            } else {
                System.out.println("Invalid command!");
            }
        }
    }

    private static void printCommands(){
        System.out.println("Enter a command (or a number, the command number)");
        System.out.println("1. encrypt");
        System.out.println("2. decrypt");
        System.out.println("3. cryptanalysis");
        System.out.println("4. all");
        System.out.println("5. quit");
    }

    private static boolean checkVigenereObject(Vigenere vigenere){
        if (vigenere == null){
            System.out.println("There was an error when reading the input!");
            return false;
        }
        return true;
    }

    private static Vigenere createVigenereNormalObject(String inputSource){
        String input = readInput(inputSource);
        if (input == null){
            return null;
        }
        return new Vigenere(input, false);
    }

    private static Vigenere createVigenereEncryptedObject(String inputSource){
        String input = readInput(inputSource);
        if (input == null){
            return null;
        }
        return new Vigenere(input, true);
    }

    private static String readInput(String inputSource){
        IInput inputReader;
        if (inputSource.equals("console")){
            inputReader = new ConsoleInput();
        } else if (inputSource.equals("file")){
            String fileName = readFileName();
            inputReader = new FileInput(fileName);
        } else {
            return null;
        }
        String input = inputReader.readInput();
        if (input == null){
            return null;
        }
        return input;
    }

    private static String readFileName(){
        System.out.println("File name:");
        Scanner scanner = new Scanner(System.in);
        return scanner.nextLine();
    }

    private static String readInputSource(String message){
        System.out.println(message);
        String input;
        Scanner scanner = new Scanner(System.in);
        while (true){
            input = scanner.nextLine();
            if (!(input.equals("console") || input.equals("file"))){
                System.out.println("You should type console/file");
                continue;
            }
            break;
        }
        return input;
    }
}
