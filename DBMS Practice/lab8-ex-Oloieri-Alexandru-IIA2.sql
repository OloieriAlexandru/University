
-- Oloieri Alexandru, IIA2
-- Documentatie folosita: https://docs.oracle.com/database/121/ARPLS/u_file.htm#ARPLS069
-- Pentru fisierul csv nu am folosit expresii regulate, am facut parsare + validare de mana

set serveroutput on;

/*
Primele 3 campuri (coloane) nu pot fi NULL, restul pot
In cazul in care o coloana este NULL, in fisierul csv va fi scris string-ul 'NULL'
*/
DECLARE
    CURSOR lista_note IS
        SELECT * FROM note;

    v_fisier UTL_FILE.FILE_TYPE;
    v_linie_csv VARCHAR2(1023);
BEGIN
    v_fisier:=UTL_FILE.FOPEN('MYDIR','note.csv','W');
    IF NOT UTL_FILE.IS_OPEN(v_fisier) THEN
        DBMS_OUTPUT.put_line('Eroare la crearea fisierului!');
        RETURN;
    END IF;
  
    UTL_FILE.PUT_LINE(v_fisier,'id,id_student,id_curs,valoare,data_notare,created_at,updated_at'); -- fisierele csv au un antet
    
    FOR v_nota_linie IN lista_note LOOP
        v_linie_csv := v_nota_linie.id || ',' || v_nota_linie.id_student || ',' || v_nota_linie.id_curs || ',' || NVL(TO_CHAR(v_nota_linie.valoare),'NULL') || ',';
        v_linie_csv := v_linie_csv || NVL(TO_CHAR(v_nota_linie.data_notare, 'DD.MM.YYYY'), 'NULL') || ',' || NVL(TO_CHAR(v_nota_linie.created_at, 'DD.MM.YYYY'),'NULL') || ',';
        v_linie_csv := v_linie_csv || NVL(TO_CHAR(v_nota_linie.updated_at, 'DD.MM.YYYY'),  'NULL');
        UTL_FILE.PUT_LINE(v_fisier, v_linie_csv);
    END LOOP;
  
    UTL_FILE.FCLOSE(v_fisier);
END;
/

-- https://www.techonthenet.com/oracle/questions/isnumeric.php
CREATE OR REPLACE FUNCTION get_number(p_str VARCHAR)
RETURN INT
AS
    v_numar INT;
BEGIN
    v_numar := TO_NUMBER(p_str);
    return v_numar;
EXCEPTION
    WHEN value_error THEN
        RETURN NULL;
END;
/

-- https://stackoverflow.com/questions/14702050/valid-date-checks-in-oracle
CREATE OR REPLACE FUNCTION get_date(p_str VARCHAR)
RETURN DATE
AS
    v_data DATE;
BEGIN
    v_data := TO_DATE(p_str, 'DD.MM.YYYY');
    return v_data;
EXCEPTION
    WHEN others THEN
        RETURN NULL;
END;
/

select * from note order by id; -- afisez notele inainte de a le sterge
/

select count(*) from note; -- afisez numarul lor
/

delete from note;
/

select * from note; -- nu ar trebui sa fie intoarsa nici o nota
/

/*
Algoritmul folosit pentru parsare: se tine minte pozitia de inceput al ultimului camp (v_ultim_i), se parcurge linie curenta pana se ajunge la final sau la ',' (v_i)
Lungimea sirului de caractere ce reprezinta campul curent este v_i - v_ultimul_i, iar string ul poate fi extras cu SUBSTR.

Am adaugat si mai multe validari:
- Verific sa nu existe doua note cu acelasi ID
- Verific ca stringul ce reprezinta un camp sa aiba valoare nenula
- Verific sa existe exact 7 campuri pe fiecare linie
- Verific ca primele 4 campuri sa fie numere, iar ultimele 3 date valide
- Prind exceptii in cazul in care id-ul studentului sau id-ul cursului nu sunt valide

In cazul in care vreo validare esueaza, fac rollback la starea tabelei de dinainte de inceperea inserarii
Pentru a se putea corecta fisierul si a se rula din nou scriptul (altfel, a doua oara s-ar incerca inserarea unei note deja existente si ar avea aceaasi PK)
*/
DECLARE
    v_fisier UTL_FILE.FILE_TYPE;
    v_linie_csv VARCHAR(1023);
    v_camp_csv VARCHAR(511);
    v_numar_linie INT;
    
    v_j INT;
    v_i INT;
    v_ultim_i INT;
    v_lungime INT;
    
    v_id note.id%TYPE;
    v_id_student note.id_student%TYPE;
    v_id_curs note.id_curs%TYPE;
    v_valoare note.valoare%TYPE;
    v_data_notare note.data_notare%TYPE;
    v_created_at note.created_at%TYPE;
    v_updated_at note.updated_at%TYPE;
    
    -- https://stackoverflow.com/questions/23239172/how-to-catch-constraint-violation-in-pl-sql
    CHECK_CONSTRAINT_VIOLATED EXCEPTION;
    PRAGMA EXCEPTION_INIT(CHECK_CONSTRAINT_VIOLATED, -2291);
BEGIN
    SAVEPOINT fara_note_inserate;

    v_fisier:=UTL_FILE.FOPEN('MYDIR','note.csv','R');
    IF NOT UTL_FILE.IS_OPEN(v_fisier) THEN
        DBMS_OUTPUT.put_line('Eroare la deschiderea fisierului!');
        RETURN;
    END IF;
    
    UTL_FILE.GET_LINE(v_fisier, v_linie_csv); -- citim antetul in gol
    v_numar_linie := 1;
    
    LOOP
        BEGIN
        v_numar_linie := v_numar_linie + 1;  
        UTL_FILE.GET_LINE(v_fisier, v_linie_csv); -- am citit o linie
        -- parsez si validez
        
        v_i := 1;
        v_ultim_i := 1;
        v_lungime := LENGTH(v_linie_csv);
        
        FOR v_j IN 1..7 LOOP
            LOOP
                EXIT WHEN v_i > v_lungime OR SUBSTR(v_linie_csv, v_i, 1) = ',';
                v_i := v_i + 1;
            END LOOP;
            
            IF v_i - v_ultim_i = 0 THEN
                DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', exista cel putin un camp vid!');
                UTL_FILE.FCLOSE(v_fisier);
                ROLLBACK TO fara_note_inserate;
                RETURN;
            END IF;
            
            IF v_ultim_i > v_lungime AND v_j <> 7 THEN
                DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', prea putine campuri!');
                UTL_FILE.FCLOSE(v_fisier);
                ROLLBACK TO fara_note_inserate;
                RETURN;
            END IF;
            
            v_camp_csv := SUBSTR(v_linie_csv, v_ultim_i, (v_i - v_ultim_i));
            
            CASE (v_j)
                WHEN 1 THEN
                    v_id := get_number(v_camp_csv);
                    IF v_id IS NULL THEN
                        DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', id-ul nu este un numar!');
                        UTL_FILE.FCLOSE(v_fisier);
                        ROLLBACK TO fara_note_inserate;
                        RETURN;
                    END IF;
                WHEN 2 THEN
                    v_id_student := get_number(v_camp_csv);
                    IF v_id_student IS NULL THEN
                        DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', id-ul studentului nu este un numar!');
                        UTL_FILE.FCLOSE(v_fisier);
                        ROLLBACK TO fara_note_inserate;
                        RETURN;
                    END IF;
                WHEN 3 THEN
                    v_id_curs := get_number(v_camp_csv);
                    IF v_id_curs IS NULL THEN
                        DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', id-ul cursului nu este un numar!');
                        UTL_FILE.FCLOSE(v_fisier);
                        ROLLBACK TO fara_note_inserate;
                        RETURN;
                    END IF;
                WHEN 4 THEN
                    IF v_camp_csv = 'NULL' THEN
                        v_valoare := NULL;
                    ELSE
                        v_valoare := get_number(v_camp_csv);
                        IF v_valoare IS NULL OR (v_valoare < 0 OR v_valoare > 10)THEN
                            DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', valoarea notei nu este un numar sau nu este in intervalul [0,10]!');
                            UTL_FILE.FCLOSE(v_fisier);
                            ROLLBACK TO fara_note_inserate;
                            RETURN;
                        END IF;
                    END IF;
                WHEN 5 THEN
                    IF v_camp_csv = 'NULL' THEN
                        v_data_notare := NULL;
                    ELSE
                        v_data_notare := get_date(v_camp_csv);
                        IF v_data_notare IS NULL THEN
                            DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', data_notare invalida!');
                            UTL_FILE.FCLOSE(v_fisier);
                            ROLLBACK TO fara_note_inserate;
                            RETURN;
                        END IF;
                    END IF;
                WHEN 6 THEN
                    IF v_camp_csv = 'NULL' THEN
                        v_created_at := NULL;
                    ELSE
                        v_created_at := get_date(v_camp_csv);
                        IF v_created_at IS NULL THEN
                            DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', created_at are valoare invalida!');
                            UTL_FILE.FCLOSE(v_fisier);
                            ROLLBACK TO fara_note_inserate;
                            RETURN;
                        END IF;
                    END IF;
                WHEN 7 THEN
                    IF v_camp_csv = 'NULL' THEN
                        v_updated_at := NULL;
                    ELSE
                        v_updated_at := get_date(v_camp_csv);
                        IF v_updated_at IS NULL THEN
                            DBMS_OUTPUT.PUT_LINE('Eroare la linia '||v_numar_linie||', updated_at are valoare invalida!');
                            UTL_FILE.FCLOSE(v_fisier);
                            ROLLBACK TO fara_note_inserate;
                            RETURN;
                        END IF;
                    END IF;                    
            END CASE;
            
            v_i := v_i + 1;
            v_ultim_i := v_i;
        END LOOP;
    
        -- DBMS_OUTPUT.put_line(v_id || ' ' || v_id_student || ' ' || v_id_curs || ' ' || v_valoare || ' ' || v_data_notare || ' ' || v_created_at || ' ' || v_updated_at);
    
        INSERT INTO note (id, id_student, id_curs, valoare, data_notare, created_at, updated_at)
            VALUES (v_id, v_id_student, v_id_curs, v_valoare, v_data_notare, v_created_at, v_updated_at);
    
        EXCEPTION
            WHEN no_data_found THEN
                EXIT;
        END;
    END LOOP;

    UTL_FILE.FCLOSE(v_fisier);
EXCEPTION
    WHEN dup_val_on_index THEN
        DBMS_OUTPUT.put_line('Eroare! Doua dintre notele din fisier au acelasi id (acelasi primary key)!');
        IF UTL_FILE.IS_OPEN(v_fisier) THEN
            UTL_FILE.FCLOSE(v_fisier);
        END IF;
        ROLLBACK TO fara_note_inserate;
    WHEN check_constraint_violated THEN
        DBMS_OUTPUT.put_line('Eroare! Cel putin un camp id_student sau id_curs este o cheie straina invalida!');
        IF UTL_FILE.IS_OPEN(v_fisier) THEN
            UTL_FILE.FCLOSE(v_fisier);
        END IF;
        ROLLBACK TO fara_note_inserate;
END;
/

select * from note order by id; -- afisez notele dupa ce le-am inserat
/

select count(*) from note; -- ar trebui sa fie aceeasi valoare ca si la primul "select count(*) from note;"
/
