
-- Oloieri Alexandru, IIA2

/*
In sursa au fost puse 6 SELECT uri in anumite locuri, care demonstreaza functionalitatile implementate:
linia 228 - select pe catalog, demonstreaza faptul ca un student + curs care nu exista in baza de date vor fi create atunci cand sunt inserate in catalog
linia 230 - select pe tabela studenti, demonstreaza faptul ca intr-adevar studentul a fost creat (studentul Oloieri Alexandru)
linia 232 - select pe tabela cursuri, demonstreaza faptul ca intr-adevar cursul a fost creat (cursul Programare Competitiva)
linia 238 - select pe tabela cursuri, demonstreaza faptul ca atunci cand este inserat in catalog nota la un curs a unui student existent, cursul va fi creat
linia 269 - select pe catalog, demonstreaza ca intr-adevar se face update asupra notei in catalog (au fost facute 2 update uri, unul in care nota a fost marita de la 8 la 10, si unul in care s-a incercat punerea notei 5 in loc de 10)
linia 298 - select pe catalog, demonstreaza ca intr-adevar studentul este sters (a fost sters studentul Oloieri Alexandru)
*/

set serveroutput on;
/

/*
Ca sa fie stersi toti studentii/ toate cursurile/ toate notele care nu au fost adaugate odata cu rularea scriptului de creare
Trebuie decomentat acest comentariu (am plecat de la presupunerea ca a fost apelat scriptul care a generat 1025 studenti)

delete from note where id_student >= 1026;
/
delete from studenti where id >= 1026;
/
delete from cursuri where id >= 25;
/
*/

CREATE OR REPLACE VIEW Catalog AS
SELECT s.nume, s.prenume, c.titlu_curs, n.valoare
FROM studenti s JOIN note n ON s.id = n.id_student
JOIN cursuri c ON c.id = n.id_curs;
/

-- the function returns INT (1/0) because boolean functions cannot be used in the following manner: select function_name(...) from dual;
CREATE OR REPLACE FUNCTION check_if_student_exists(p_nume studenti.nume%TYPE, p_prenume studenti.prenume%TYPE)
RETURN INT
AS
    v_count INT;
BEGIN
    SELECT COUNT(*) INTO v_count FROM studenti WHERE LOWER(nume) = LOWER(p_nume) AND LOWER(prenume) = LOWER(p_prenume);
    IF v_count > 0 THEN
        return 1;
    ELSE
        return 0;
    END IF;
    return 0;
END;
/

-- the function will only be called if the student exists in the database
CREATE OR REPLACE FUNCTION get_std_id(p_nume studenti.nume%TYPE, p_prenume studenti.prenume%TYPE)
RETURN studenti.id%TYPE
AS
    CURSOR return_std_by_name(p_std_nume studenti.nume%TYPE, p_std_prenume studenti.prenume%TYPE) IS
        SELECT id FROM studenti WHERE LOWER(nume) = LOWER(p_std_nume) AND LOWER(prenume) = LOWER(p_std_prenume) AND ROWNUM <= 1;
    v_std_id studenti.id%TYPE;
    v_result studenti.id%TYPE;
BEGIN
    OPEN return_std_by_name(p_nume, p_prenume);
    LOOP
        FETCH return_std_by_name INTO v_std_id;
        EXIT WHEN return_std_by_name%NOTFOUND;
        v_result := v_std_id;
    END LOOP;
    CLOSE return_std_by_name;
    RETURN v_result;
END;
/

-- the function will only be called if the student doesn't exist in the database
CREATE OR REPLACE FUNCTION generate_student(p_nume studenti.nume%TYPE, p_prenume studenti.prenume%TYPE)
RETURN studenti.id%TYPE
AS
    v_id studenti.id%TYPE;
    v_nr_matricol studenti.nr_matricol%TYPE;
    v_an studenti.an%TYPE;
    v_grupa studenti.grupa%TYPE;
    v_bursa studenti.bursa%TYPE;
    v_data_nastere studenti.data_nastere%TYPE;
    v_email studenti.email%TYPE;
    
    v_len INT;
BEGIN
    SELECT MAX(id) INTO v_id FROM studenti;
    IF v_id IS NULL THEN
        v_id := 1;
    ELSE 
        v_id := v_id + 1;
    END IF;
    
    v_nr_matricol := FLOOR(DBMS_RANDOM.VALUE(100,999)) || CHR(FLOOR(DBMS_RANDOM.VALUE(65,91))) || CHR(FLOOR(DBMS_RANDOM.VALUE(65,91))) || FLOOR(DBMS_RANDOM.VALUE(0,9));
    v_an := TRUNC(DBMS_RANDOM.VALUE(0,3))+1;
    v_grupa := CHR(FLOOR(DBMS_RANDOM.VALUE(65,67))) || (FLOOR(DBMS_RANDOM.VALUE(0, 7)+1));
    v_data_nastere := TO_DATE('01-01-1974','MM-DD-YYYY')+TRUNC(DBMS_RANDOM.VALUE(0,365));
    IF FLOOR(DBMS_RANDOM.VALUE(0,100)) < 20 THEN
        v_bursa := FLOOR(DBMS_RANDOM.value(50, 100)) * 10 + 50;
    ELSE
        v_bursa := null;
    END IF;
    
    v_len := LENGTH(p_prenume);
    v_email := p_nume || '.';
    FOR v_i in 1..v_len LOOP
        IF SUBSTR(p_prenume, v_i, 1) = ' ' THEN
            v_email := v_email || '.';
        ELSE
            v_email := v_email || SUBSTR(p_prenume, v_i, 1);
        END IF;
    END LOOP;
    v_email := v_email || '@gmail.com';
    
    INSERT INTO studenti (id, nr_matricol, nume, prenume, an, grupa, bursa, data_nastere, email, created_at, updated_at)
        VALUES(v_id, v_nr_matricol, p_nume, p_prenume, v_an, v_grupa, v_bursa, v_data_nastere, v_email, sysdate, sysdate);
    return v_id;
END;
/

CREATE OR REPLACE FUNCTION check_if_course_exists(p_titlu_curs cursuri.titlu_curs%TYPE)
RETURN INT
AS
    v_count INT;
BEGIN
    SELECT COUNT(*) INTO v_count FROM cursuri WHERE LOWER(titlu_curs) = LOWER(p_titlu_curs);
    IF v_count > 0 THEN
        return 1;
    ELSE
        return 0;
    END IF;
    return 0;
END;
/

-- the function will only be called if the student exists in the database
CREATE OR REPLACE FUNCTION get_course_id(p_titlu_curs cursuri.titlu_curs%TYPE)
RETURN cursuri.id%TYPE
AS
    CURSOR return_course_id_by_title(p_course_title cursuri.titlu_curs%TYPE) IS
        SELECT id FROM cursuri WHERE LOWER(titlu_curs) = LOWER(p_course_title) AND ROWNUM <= 1;
    v_course_id cursuri.id%TYPE;
    v_result cursuri.id%TYPE;
BEGIN
    OPEN return_course_id_by_title(p_titlu_curs);
    LOOP
        FETCH return_course_id_by_title INTO v_course_id;
        EXIT WHEN return_course_id_by_title%NOTFOUND;
        v_result := v_course_id;
    END LOOP;
    CLOSE return_course_id_by_title;
    RETURN v_result;
END;
/

-- the function will only be called if the course doesn't exist in the database
CREATE OR REPLACE FUNCTION generate_course(p_titlu_curs cursuri.titlu_curs%TYPE)
RETURN cursuri.id%TYPE
AS
    v_id cursuri.id%TYPE;
    v_an cursuri.an%TYPE;
    v_semestru cursuri.semestru%TYPE;
    v_credite cursuri.credite%TYPE;
BEGIN
    SELECT MAX(id) INTO v_id from cursuri;
    IF v_id IS NULL THEN
        v_id := 1;
    ELSE 
        v_id := v_id + 1;
    END IF;
    
    v_an := TRUNC(DBMS_RANDOM.VALUE(0,3))+1;
    v_semestru := TRUNC(DBMS_RANDOM.VALUE(0,2))+1;
    v_credite := TRUNC(DBMS_RANDOM.VALUE(0,3))+4;
    INSERT INTO cursuri (id, titlu_curs, an, semestru, credite, created_at, updated_at)
        VALUES (v_id, p_titlu_curs, v_an, v_semestru, v_credite, sysdate, sysdate);
    return v_id;
END;
/

/*
2. Inserarea unei note la un curs pentru un student inexistent cu adaugarea studentului;
3. Inserarea unei note la un curs pentru un curs inexistent - cu adaugarea cursului;
4. Inserarea unei note cand nu exista nici studentul si nici cursul.

Apel: INSERT INTO catalog ({{nume_student}}, {{prenume_student}}, {{titlu_curs}}, {{valoare}});
*/
CREATE OR REPLACE TRIGGER catalog_insert
INSTEAD OF INSERT ON catalog
DECLARE
    v_student_exists INT;
    v_course_exists INT;
    
    v_std_id studenti.id%TYPE;
    v_course_id cursuri.id%TYPE;
    
    v_grade_id INT;
BEGIN
    v_student_exists := check_if_student_exists(:NEW.nume, :NEW.prenume);
    v_course_exists := check_if_course_exists(:NEW.titlu_curs);
    IF v_student_exists = 1 THEN
        v_std_id := get_std_id(:NEW.nume, :NEW.prenume);
        IF v_course_exists = 1 THEN -- the course and the student exist, insert the grade
            v_course_id := get_course_id(:NEW.titlu_curs);
        ELSE -- the student exists, generate the course
            v_course_id := generate_course(:NEW.titlu_curs);
        END IF;
    ELSIF v_course_exists = 1 THEN -- the course exists, generate the student
        v_course_id := get_course_id(:NEW.titlu_curs);
        v_std_id := generate_student(:NEW.nume, :NEW.prenume);
    ELSE -- neither the course nor the student exists, generate both
        v_std_id := generate_student(:NEW.nume, :NEW.prenume);
        v_course_id := generate_course(:NEW.titlu_curs);
    END IF;
    
    SELECT Max(id) INTO v_grade_id FROM note; -- the id column is not set as "AUTO INCREMENT", we have to manually compute de id
    v_grade_id := v_grade_id + 1;
    INSERT INTO note (id, id_student, id_curs, valoare, data_notare, created_at, updated_at) VALUES
        (v_grade_id, v_std_id, v_course_id, :NEW.valoare, SYSDATE, sysdate, sysdate);
EXCEPTION
    WHEN dup_val_on_index THEN
        DBMS_OUTPUT.put_line('Studentul '||:NEW.nume||' '||:NEW.prenume||' are deja o nota la materia '||:NEW.titlu_curs);
END;
/

INSERT INTO catalog VALUES ('Oloieri', 'Alexandru', 'Programare Competitiva', 8); -- Nu exista nici materia nici studentul
/
INSERT INTO catalog VALUES ('Oloieri', 'Alexandru', 'Programare Competitiva', 8); -- Va fi afisat mesajul "Studentul Oloieri Alexandru are deja o nota la materia Programare Competitiva"
/
SELECT * FROM Catalog WHERE nume = 'Oloieri' AND prenume = 'Alexandru';
/
SELECT * FROM studenti WHERE nume = 'Oloieri' AND prenume = 'Alexandru';
/
SELECT * FROM cursuri WHERE titlu_curs = 'Programare Competitiva';
/
INSERT INTO catalog VALUES ('Oloieri', 'Alexandru', 'DevOps', 10); -- :), Nu exista materia
/
INSERT INTO catalog VALUES ('Pavel', 'Boris', 'DevOps', 9); -- Nu exista studentul
/
SELECT * FROM catalog WHERE titlu_curs = 'DevOps';
/

/*
5. Update la valoarea notei pentru un student - se va modifica valoarea campului updated_at. De asemenea, valoarea nu poate fi modificata cu una mai mica (la mariri se considera nota mai mare).

Triggerul se apeleaza numai asupra randurilor care exista in view, deci studentul si cursul sigur exista (deci exista si nota, daca nu exista nota nu s-ar fi apelat triggerul)
Deci in tabelul note exista deja un rand in care id_student = id-ul studentului curent si id_curs = id-ul cursului student
Deci nu mai e nevoie sa fac validari, daca noua nota e strict mai mare decat vechea nota, iau id ul cursului si a studentului si fac update la valori

Apel: UPDATE catalog SET valoare = {{noua_nota}} WHERE nume = {{nume_student}} AND prenume = {{prenume_student}} AND titlu_curs = {{titlu_curs}}
*/
CREATE OR REPLACE TRIGGER catalog_update
INSTEAD OF UPDATE ON catalog
DECLARE
    v_std_id studenti.id%TYPE;
    v_course_id cursuri.id%TYPE;
BEGIN
    IF :OLD.valoare < :NEW.valoare THEN
        v_std_id := get_std_id(:NEW.nume, :NEW.prenume);
        v_course_id := get_course_id(:NEW.titlu_curs);
        
        UPDATE note SET valoare = :NEW.valoare, UPDATED_AT = SYSDATE WHERE id_student = v_std_id AND id_curs = v_course_id;
    END IF;
END;
/

UPDATE catalog SET valoare = 10 WHERE nume = 'Oloieri' AND prenume = 'Alexandru' AND titlu_curs = 'Programare Competitiva'; -- nota curenta este 8
/
UPDATE catalog SET valoare = 5 WHERE nume = 'Oloieri' AND prenume = 'Alexandru' AND titlu_curs = 'Programare Competitiva'; -- nota este 5, nu se va modifica
/
SELECT * FROM catalog WHERE nume = 'Oloieri' AND prenume = 'Alexandru'; -- nota este 10 (daca nu se intoarce niciun rezultat este pentru ca delete-ul de mai jos s-a executat inaintea select ului -> am vazut ca se executa in paralel unele query-uri)
/

/*
1. Stergerea unui student si totodata a notelor sale (fara a folosi cascade constraints);

Apel: DELETE FROM catalog WHERE nume = {{nume_student}} AND prenume = {{prenume_student}};
*/
CREATE OR REPLACE TRIGGER catalog_delete
INSTEAD OF DELETE ON catalog
DECLARE
    v_student_exists INT;

    v_std_id studenti.id%TYPE;
BEGIN
    v_student_exists := check_if_student_exists(:OLD.nume, :OLD.prenume);
    
    IF v_student_exists = 1 THEN
        v_std_id := get_std_id(:OLD.nume, :OLD.prenume);
        
        DELETE FROM note WHERE id_student = v_std_id; -- se sterg mai intai notele
        DELETE FROM prieteni WHERE id_student1 = v_std_id OR id_student2 = v_std_id; -- apoi se sterge studentul din tabelul cu prietenii
        DELETE FROM studenti WHERE id = v_std_id; -- iar abia apoi studentul (altfel vor aparea erori din cauza fk)
    END IF;
END;
/

DELETE FROM catalog WHERE nume = 'Oloieri' AND prenume = 'Alexandru'; -- vor fi sterse toate notele
/
SELECT * FROM catalog WHERE nume = 'Oloieri' AND prenume = 'Alexandru'; -- nu ar trebui sa fie intors niciun rand
/