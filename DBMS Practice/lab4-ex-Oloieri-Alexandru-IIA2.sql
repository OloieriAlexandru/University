
ALTER TABLE studenti
DROP COLUMN medii;
/

CREATE OR REPLACE TYPE lista_medii AS TABLE OF NUMBER(4, 2);
/

ALTER TABLE studenti
ADD medii lista_medii NESTED TABLE medii STORE AS l_medii;
/

DECLARE
    -- cursor prin intermediul caruia iterez prin studenti
    CURSOR lista_studenti IS
        SELECT studenti.id FROM studenti;

    -- cursor care, pentru un student, calculeaza mediile semestriale
    CURSOR medii_student(p_id_student studenti.id%TYPE) IS 
        SELECT AVG(note.valoare) FROM studenti JOIN note ON studenti.id = note.id_student 
            JOIN cursuri ON note.id_curs = cursuri.id WHERE studenti.id = p_id_student GROUP BY cursuri.an, cursuri.semestru;

    v_medie NUMBER(4,2);
    
    v_id_student studenti.id%TYPE;
    
    v_lista_medii_student lista_medii;
BEGIN
    OPEN lista_studenti;
    LOOP
        FETCH lista_studenti INTO v_id_student;
        EXIT WHEN lista_studenti%NOTFOUND;
        
        OPEN medii_student(v_id_student);
        
        v_lista_medii_student := lista_medii(); -- pentru fiecare student creez o noua tabela
        
        LOOP
            FETCH medii_student INTO v_medie;
            EXIT WHEN medii_student%NOTFOUND;
            v_lista_medii_student.EXTEND; -- fac spatiu pentru o noua medie
            v_lista_medii_student(v_lista_medii_student.LAST) := v_medie; -- adaug media
        END LOOP;
        CLOSE medii_student;        
        
        UPDATE studenti SET medii = v_lista_medii_student WHERE id = v_id_student; -- setez noua lista de medii pentru studentul curent
    END LOOP;
    CLOSE lista_studenti;
END;
/

CREATE OR REPLACE FUNCTION numar_medii(p_id_student studenti.id%TYPE)
RETURN INTEGER IS
    v_lista_medii_student lista_medii;
BEGIN
    SELECT medii INTO v_lista_medii_student FROM studenti WHERE id = p_id_student; -- selectez mediile
    return v_lista_medii_student.COUNT; -- folosesc metoda predefinita COUNT
EXCEPTION
    WHEN no_data_found THEN
    return 0;
END;
/