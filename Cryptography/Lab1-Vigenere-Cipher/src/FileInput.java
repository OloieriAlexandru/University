import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

public class FileInput implements IInput {
    private File file = null;

    FileInput(String fileName) {
        file = new File(fileName);
    }

    @Override
    public String readInput(){
        if (file == null){
            return null;
        }
        try{
            FileInputStream fis = new FileInputStream(file);
            byte[] data = new byte[(int)file.length()];
            fis.read(data);
            fis.close();
            return new String(data, "UTF-8");
        } catch (FileNotFoundException e){
            System.out.println("The input file has not been found!");
            return null;
        } catch (IOException e) {
            System.out.println("Error when reading the input from file!");
            return null;
        }
    }
}
