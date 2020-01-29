
## Retele de calculatoare, tema saptamana 4
**Oloieri Alexandru IIA2**

Build: make sau make release
Run: make run sau ./build/main.bin (trebuie rulata comanda de build inainte de asta)

!Pentru ca aplicatia sa functioneze bine asigurati-va ca exista directoarele "build" si "users" langa fisierul "Makefile"

Descriere aplicatie:

Exista 2 meniuri: meniul cand utilizatorul nu e logat (M1), si meniul cand acesta este logat(M2):
Cat timp aplicatia ruleaza, va aparea mesajul "cmd:", iar utilizatorul trebuie sa introduca o comanda.

Comanda "cmd" este una dintre cele doua comenzi care e disponibila in ambele meniuri: aceasta comanda va lista toate comenzile disponibile in meniul curent (daca se adauga si optiunea -details, vor aparea detalii despre fiecare dintre comenzi)
Cealalta comanda disponibila in ambele meniuri este "quit", care tot ce face este sa inchida aplicatia

### M1:
Cand utilizatorul nu e logat acesta poate rula 5 comenzi:
- register: utilizatorul poate crea un nou cont (cu care apoi sa se logheze), iar pentru asta va trebui sa specifice un nume si o parola
- login: utilizatorul se poate loga cu un cont 
- list-users: vor fi afisate toate conturile (numele acestora)
- cmd ...
- quit: ...

### M2:
Ceea cu ce vine in plus aceasta aplicatie este faptul ca la prima logare, fiecarui cont ii va fi creat in directorul users/ un director cu numele contului sau (ex: prima data cand cineva se logheaza cu un cont alt carui nume este alex, va aparea un folder /users/alex), 
director care va fi spatiu de lucru pe tot parcursul timpului in care e utilizatorul e logat. Mai exact, comenzi de tipul myfind/ mystat/ myls vor avea ca si rezultate numai fisiere din spatiul curent de lucru (/users/[nume utilizator]).

In acest meniul utilizatorul poate rula 12 comenzi:
- logout: se revine in M1
- myfind file (-detailed, -dir:<>): se cauta fisiere ce contin in numele lor ca si subsir continutul argumentului file
- mystat file: se afiseaza informatii despre fisierul file
- mycreate file: se creaza in spatiul de lucru al utilizatorului logat fisierul cu numele file
- myls (-r, -dir:<>): se afiseaza toate fisierele din directorul curent (sau cel specificat de optiunea -dir<>), in caz ca se adauga optiunea -r (de la recursive), vor fi afisate toate fisierele din arborele de fisiere cu radacina in directorul din care incepe cautarea
- mycat file (-first:<>): se afiseaza continutul fisierului file (sau numai primele x linii, in caz ca se adauga optiunea -first:x)
- myedit file (-editor:<>): se va deschide un editor de text pentru a putea edita continutul fisierului file (by default se deschide vim, alte optiuni fiind nano si joe)
- myrm file: fisierul file este sters
- mymkdir dir: in spatiul de lucru al utilizatorului curent este creat directorul dir
- run program -lang:programming-language: (toate optiunile trebuie adaugate la rularea comenzii) se va incerca rularea programului din fisierul program, fiind disponibile 3 limbaje de programare:
	- C/C++ (comportamentul este asemanator): se va incerca compilarea fisierului program (rezultatul va fi un fisier program.bin), iar in caz de succes, programul va fi rulat
	- Python: va fi apelata comanda: python program (deci daca fisierul program este un script corect de python, acesta mereu va fi rulat cu succes)
- cmd ...
- quit ...

In caz ca se doreste si lucrul asupra altor fisiere, programul suporta si alte comenzi, inclusiv comenzi complexe (ce contin pipe-uri, de exemplu "ls -la | wc"), insa acestea nu vor mai fi rulate din spatiul de lucru al utilizatorului curent, ci din directorul aplicatiei 
(mai exact, prin rularea comenzii "ls", vor fi afisate (cel putin) directoarele src, users si build, si fisierele Makefile si READ.me)

### Alte detalii:

* Notatii:
  P = programul principal
  SC = spawner child ul (copilul ce va rula pe toata durata executiei programului)
  C = copii create de SC, pentru a executa comenzi ce nu au fost implementate in program
  x -> y = x transmite date lui y

* P -> SC printr-un socketpair
  SC -> P printr-un pipe extern (FIFO)
  SC -> C printr-un pipe
  C -> SC printr-un pipe
  (deci au fost utilizate toate cele 3 mecanisme de comunicare)

* Comenzile complexe au fost implementate cu ajutorul unui program recursiv, iar comunicarea intre copii generati de acel program se face prin pipe uri + redirectari ale stdout/stdin

* Unele comenzi (de exemplu login sau register) sunt interactive, adica trebuie introduse date si dupa citirea de catre P a comenzii. 
  Pentru a nu citi informatii direct din copil (de exemplu username ul la logare), am extins putin ideea transmiterii mesajului prefixat de lungimea sa: dupa ce P -> SC, acesta va citi dintr-un fifo la infinit un numar natural. 
  Exista 2 flag uri in aplicatie pentru comunicarea in timpul rularii unei comenzi dintre P si SC: flagGetMessageFromUser si flagPrintMessage, iar in caz ca numarul citit este unul dintre aceste 2 flaguri, acesta va avea comportament specific fiecarui caz 
  (de exemplu, pentru flagPrintMessage parintele stie ca va urma un alt numar natural, iar apoi un sir de caractere de acea lungime, si ca trebuie sa afiseze acel mesaj pe ecran). 
  In cazul in care numarul citit este egal cu valoarea flagului flagStop, aplicatia este oprita, iar in rest inseamna ca urmeaza raspunsul SC la comanda trimisa, va afisa raspunsul si va astepta alta comanda de la tastatura. 
  Prin implementarea comportamentului descris mai sus, a fost nevoie sa apelez functia readLine (cu care sunt realizate toate citirile de la tastatura din aplicatie) de numai 2 ori, desi utilizatorul trebuie sa introduca date de mult mai multe ori.
