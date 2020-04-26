import java.util.Scanner;

class Vigenere {
    private static double[]     englishAlphabetLettersFrequency = {0.0855, 0.0160, 0.0316, 0.0387,
            0.1210, 0.0218, 0.0209, 0.0496, 0.0733, 0.0022, 0.0081, 0.0421, 0.0253, 0.0717, 0.0747,
            0.0207, 0.0010, 0.0633, 0.0673, 0.0894, 0.0268, 0.0106, 0.0183, 0.0019, 0.0172, 0.0011};
    private String              plainText = null;
    private String              encryptedText = null;
    private String              key = null;

    Vigenere(String plainText, boolean textIsEncrypted){
        if (!textIsEncrypted){
            this.plainText = this.getRemoveAllButLetters(plainText.toUpperCase());
        } else {
            this.encryptedText = plainText.toUpperCase();
        }
    }

    void encrypt(){
        if (plainText == null){
            System.out.println("There is no plain text to be encrypted!");
            return;
        }
        if (key == null){
            System.out.println("You have to enter a key!");
            return;
        }

        System.out.println("The plain text will be encrypted!");

        StringBuilder stringBuilder = new StringBuilder();

        int wh = 0;
        for (int i=0;i<plainText.length();++i){
            stringBuilder.append(encryptCharacter(plainText.charAt(i), key.charAt(wh)));
            wh = (wh + 1) % key.length();
        }

        encryptedText = stringBuilder.toString();

        System.out.println("Result: ");
        System.out.println(encryptedText);
    }

    void decrypt(){
        if (encryptedText == null){
            System.out.println("Cannot decrypt! First you have to encrypt the text!");
            return;
        }
        if (key == null){
            System.out.println("Cannot decrypt! You haven't entered any key!");
            return;
        }

        System.out.println("The crypto text will be decrypted using the given key!");

        StringBuilder stringBuilder = new StringBuilder();

        int wh = 0;
        for (int i = 0; i< encryptedText.length(); ++i){
            stringBuilder.append(decryptCharacter(encryptedText.charAt(i),key.charAt(wh)));
            wh = (wh + 1) % key.length();
        }

        String decryptedText = stringBuilder.toString();

        System.out.println("Result: ");
        System.out.println(decryptedText);
    }

    void cryptanalysis(){
        if (encryptedText == null){
            System.out.println("Cannot do the cryptanalysis! First you have to encrypt the text!");
            return;
        }
        if (!encryptedTextFollowsSpecifications(encryptedText)){
            System.out.println("The text should only have uppercase or lowercase letters!");
            return;
        }

        int keyLength = cryptanalysisFindKeyLength(encryptedText);

        System.out.println("Found key length: " + keyLength);

        StringBuilder stringBuilder = new StringBuilder();

        for (int i=1;i<=keyLength;++i){
            stringBuilder.append(cryptanalysisGetKeyKthCharacter(encryptedText, i, keyLength));
        }

        String foundKey = stringBuilder.toString();
        System.out.println("Found the key: "+foundKey);
    }

    void readKey() {
        System.out.println("Enter the key:");
        Scanner scanner = new Scanner(System.in);
        while (true){
            key = scanner.nextLine();
            if (key.length() > 0){
                boolean ok = true;

                for (int i=0;i<key.length();++i){
                    if (!isLetter(key.charAt(i))){
                        ok = false;
                        break;
                    }
                }

                if (ok){
                    break;
                }

                System.out.println("The key has to contain only letters!");
            }
            System.out.println("The key has to have at least one character!");
        }
        key = key.toUpperCase();
    }

    private char cryptanalysisGetKeyKthCharacter(String encryptedText, int k, int keyLength){
        double bestDiff = Double.MAX_VALUE;
        char ch = 'A';
        for (int i=0;i>=-25;--i){
            double mutualIndexOfCoincidence = cryptanalysisGetKeyKthCharacterTryMGetMutualIndexOfCoincidence(encryptedText, k, keyLength, i);
            double diff = Math.abs(0.065 - mutualIndexOfCoincidence);
            if (diff < bestDiff){
                bestDiff = diff;
                ch = (char)('A'-i);
            }
        }
        return ch;
    }

    // not used
    private boolean cryptanalysisGetKeyKthCharacterTryM(String encryptedText, int k, int keyLength, int m){
        double limitLeft = 0.055;
        double limitRight = 0.075;

        if (keyLength < 5){
            limitLeft += 0.0015 * (5-keyLength+1);
            limitRight -= 0.0015 * (5-keyLength+1);
        }

        double mutualIndexOfCoincidence = cryptanalysisGetKeyKthCharacterTryMGetMutualIndexOfCoincidence(encryptedText, k, keyLength, m);

        return limitLeft <= mutualIndexOfCoincidence && mutualIndexOfCoincidence <= limitRight;
    }

    private double cryptanalysisGetKeyKthCharacterTryMGetMutualIndexOfCoincidence(String encryptedText, int k, int keyLength, int m) {
        int[] fq = new int[26];
        double l1 = 0;
        char ch;
        for (int i=k-1;i<encryptedText.length();i+=keyLength){
            ch = (char)(encryptedText.charAt(i) + m);
            if (ch < 'A'){
                int diff = 'A' - ch;
                ch = (char)('Z' - diff + 1);
            }
            ++fq[ch-'A'];
            l1 += 1.0;
        }
        double mutualIndexOfCoincidence = 0.0;
        for (int i=0;i<26;++i){
            mutualIndexOfCoincidence += englishAlphabetLettersFrequency[i] * (fq[i]/l1);
        }
        return  mutualIndexOfCoincidence;
    }

    private int cryptanalysisFindKeyLength(String encryptedText){
        for (int i=1;i<encryptedText.length();++i){
            if (cryptanalysisKeyHasLengthK(encryptedText, i)){
                return i;
            }
        }
        double bestDiff = Double.MAX_VALUE;
        int keyLength = 1;
        for (int i=1;i<Math.min(encryptedText.length(),32);++i){
            double sum = 0.0;

            for (int j=1;j<=i;++j){
                sum += cryptanalysisKeyHasLengthKMthSubstringGetIndexOfCoincidence(encryptedText, i, j);
            }

            double mean = sum / i;
            double diff = Math.abs(mean - 0.065);
            if (diff < bestDiff){
                bestDiff = diff;
                keyLength = i;
            }
        }
        return keyLength;
    }

    private boolean cryptanalysisKeyHasLengthK(String encryptedText, int k){
        for (int i=1;i<=k;++i){
            if (!cryptanalysisKeyHasLengthKMthSubstring(encryptedText, k, i)){
                return false;
            }
        }
        return true;
    }

    private boolean cryptanalysisKeyHasLengthKMthSubstring(String encryptedText, int k, int m){
        double indexOfCoincidence = cryptanalysisKeyHasLengthKMthSubstringGetIndexOfCoincidence(encryptedText, k, m);

        double limitLeft = 0.050;
        double limitRight = 0.080;

        if (k < 5){
          limitLeft += 0.0025 * (5-k+1);
          limitRight -= 0.0025 * (5-k+1);
        }

        return limitLeft <= indexOfCoincidence && indexOfCoincidence <= limitRight;
    }

    private double cryptanalysisKeyHasLengthKMthSubstringGetIndexOfCoincidence(String encryptedText, int k, int m){
        double l1 = 0;
        int[] fq = new int[26];

        double indexOfCoincidence = 0.0;
        for (int i=m-1;i<encryptedText.length();i+=k){
            ++fq[encryptedText.charAt(i)-'A'];
            l1 += 1.0;
        }
        double l2 = l1 - 1.0;
        for (int i=0;i<26;++i){
            indexOfCoincidence += ((fq[i]/l1)*((fq[i]-1)/l2));
        }
        return indexOfCoincidence;
    }

    private char encryptCharacter(char ch, char keyCh){
        ch += (int)(keyCh-'A');
        if (ch > 'Z'){
            int diff = ch - 'Z';
            ch = (char)('A' - 1 + diff);
        }
        return ch;
    }

    private char decryptCharacter(char ch, char keyCh){
        ch -= (int)(keyCh-'A');
        if (ch < 'A'){
            int diff = 'A' - ch;
            ch = (char)('Z' + 1 - diff);
        }
        return ch;
    }

    private String getRemoveAllButLetters(String text){
        StringBuilder stringBuilder = new StringBuilder();

        for (int i=0;i<text.length();++i){
            if (!this.isLetter(text.charAt(i))){
                continue;
            }
            stringBuilder.append(text.charAt(i));
        }

        return stringBuilder.toString();
    }

    private boolean isLetter(char ch){
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    private boolean encryptedTextFollowsSpecifications(String encryptedText){
        for (int i=0;i<encryptedText.length();++i){
            if (!isLetter(encryptedText.charAt(i))){
                return false;
            }
        }
        return true;
    }
}
