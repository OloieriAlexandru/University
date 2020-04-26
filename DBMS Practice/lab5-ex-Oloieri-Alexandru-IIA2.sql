
-- Exercitiul 1

ALTER TABLE note ADD CONSTRAINT uk_curs_std UNIQUE (id_student, id_curs);
/

-- varianta cu count
-- timp executie: 9.561 secunde
DECLARE
    v_i INTEGER;
    v_numar_note INTEGER;
    
    v_numar_note_logica INTEGER;
    
    v_nota_logica note.valoare%TYPE := 10;
    v_id_curs note.id_curs%TYPE := 1; -- logica, am hardcodat id ul ca sa nu influenteze timpul de executie, m-am uitat si in scriptul de creare a bazei de date si logica va avea mereu id-ul 1
    v_id_student note.id_student%TYPE := 1; -- un student care exista in baza de date
BEGIN
    SELECT COUNT(*) INTO v_numar_note FROM note; -- tabela note nu are AUTO INCREMENT, noua nota trebuie sa aiba id ul minim max(id) + 1, max(id) = count(id)
    FOR v_i IN 1..1000000 LOOP
        SELECT COUNT(valoare) INTO v_numar_note_logica FROM note WHERE id_curs = v_id_curs AND id_student = v_id_student; -- count ul va intoarce mereu 1
        IF v_numar_note_logica = 0 THEN
            INSERT INTO note (id, id_student, id_curs, valoare, data_notare)
                VALUES (v_numar_note+1, v_id_student, v_id_curs, v_nota_logica, SYSDATE);
        END IF;
    END LOOP;
END;
/

-- varianta cu exceptia
-- timp executie: 165.884 secunde
DECLARE
    v_i INTEGER;
    v_numar_note INTEGER;
    
    v_nota_logica note.valoare%TYPE := 10;
    v_id_curs note.id_curs%TYPE := 1; -- logica
    v_id_student note.id_student%TYPE := 1; -- un student care exista in baza de date
BEGIN
    SELECT COUNT(*) INTO v_numar_note FROM note;
    FOR v_i IN 1..1000000 LOOP
        BEGIN
            INSERT INTO note (id, id_student, id_curs, valoare, data_notare)
                    VALUES (v_numar_note+1, v_id_student, v_id_curs, v_nota_logica, SYSDATE);
        EXCEPTION
            WHEN dup_val_on_index THEN
                continue; -- trebuie sa fie macar o operatie
        END;
    END LOOP;
END;
/

-- Exercitiul 2

CREATE OR REPLACE FUNCTION medie_student(p_nume studenti.nume%TYPE, p_prenume studenti.prenume%TYPE)
RETURN NUMBER AS
    v_medie NUMBER(4,2);
    v_count INTEGER;
    v_id_student studenti.id%TYPE;
    
    student_inexistent EXCEPTION;
    PRAGMA EXCEPTION_INIT(student_inexistent, -20001);
BEGIN
    SELECT COUNT(*) INTO v_count FROM studenti WHERE nume = p_nume AND prenume = p_prenume;
    IF v_count = 0 THEN
        raise student_inexistent;
    END IF;
    SELECT id INTO v_id_student FROM studenti WHERE nume = p_nume AND prenume = p_prenume;
    SELECT NVL(AVG(valoare),0) INTO v_medie FROM note where id_student = v_id_student;
    return v_medie;
EXCEPTION
    WHEN student_inexistent THEN
        raise_application_error( -20001, 'Studentul '||p_nume||' '||p_prenume||' nu exista in baza de date.');
END;
/

/* 
Rezultat executie:
Media studentului Grama Claudia este 6.58
Media studentului Boca Elvis Ovidiu este 6.5
Media studentului Barzu Daria este 7.75
Studentul Baetuu Deliaa nu exista in baza de date.
Studentul Cojocaruu Bogdann nu exista in baza de date.
Studentul Mihaaila Sebastiaan nu exista in baza de date.
(Pe alte calculatoare, sansele ca output ul sa fie la fel sunt extrem de mici
 fiindca studentii sunt generati random, output ul e dat de studentii din baza mea de date)
*/
DECLARE
    TYPE informatie_student IS RECORD(
        nume studenti.nume%TYPE,
        prenume studenti.prenume%TYPE
    );
    -- o alta varianta de memorare a numelor + prenumelor studentilor ar fi fost sa creez 2 varray, unul pentru nume si unul pentru prenume,
    -- fiecare cu cate 6 elemente, si informatiile pentru un student ar fi fost numele + prenumele de pe acelasi index in varray
    -- am ales varianta cu RECORD pentru ca nu am mai folosit-o
    -- (varianta cu 2 varrays ar fi fost mai scurta, le-as fi putut initializa aici, in zona DECLARE, spre deosebire de varray ul de RECORDS,
    -- pe care a trebuit sa le initializez in blocul BEGIN END fiindca nu am gasit o varianta de a-l initializa aici)
    TYPE lista_studenti IS VARRAY(6) OF informatie_student;
    v_studenti lista_studenti := lista_studenti();
    
    student_inexistent EXCEPTION;
    PRAGMA EXCEPTION_INIT(student_inexistent, -20001);
    
    v_medie NUMBER(4,2);
BEGIN
    v_studenti.EXTEND;
    v_studenti(v_studenti.LAST).nume := 'Grama';
    v_studenti(v_studenti.LAST).prenume := 'Claudia';
    v_studenti.EXTEND;
    v_studenti(v_studenti.LAST).nume := 'Boca';
    v_studenti(v_studenti.LAST).prenume := 'Elvis Ovidiu';
    v_studenti.EXTEND;
    v_studenti(v_studenti.LAST).nume := 'Barzu';
    v_studenti(v_studenti.LAST).prenume := 'Daria';
    v_studenti.EXTEND;
    v_studenti(v_studenti.LAST).nume := 'Baetuu';
    v_studenti(v_studenti.LAST).prenume := 'Deliaa';
    v_studenti.EXTEND;
    v_studenti(v_studenti.LAST).nume := 'Cojocaruu';
    v_studenti(v_studenti.LAST).prenume := 'Bogdann';
    v_studenti.EXTEND;
    v_studenti(v_studenti.LAST).nume := 'Mihaaila';
    v_studenti(v_studenti.LAST).prenume := 'Sebastiaan';
    
    FOR v_i in 1..v_studenti.COUNT LOOP
        BEGIN
            v_medie := medie_student(v_studenti(v_i).nume, v_studenti(v_i).prenume);
            DBMS_OUTPUT.put_line('Media studentului ' || v_studenti(v_i).nume || ' ' || v_studenti(v_i).prenume || ' este ' || v_medie); -- nu a fost aruncata vreo exceptie, studentul exista
        EXCEPTION
            WHEN student_inexistent THEN
            DBMS_OUTPUT.put_line(SUBSTR(SQLERRM,12)); -- erorile au forma: "ORA-20001: Studentul Baetuu Deliaa nu exista in baza de date.", cu SUBSTR scot doar mesajul si il afisez
        END;
    END LOOP;
END;
/