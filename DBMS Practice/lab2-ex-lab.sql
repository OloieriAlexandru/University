
set serveroutput on;
accept grupa prompt 'Introdu grupa: ';
DECLARE
    CURSOR lista_studenti_10 (p_grupa studenti.grupa%type, p_curs cursuri.titlu_curs%type, p_nota note.valoare%type) IS
            SELECT DISTINCT s.nume, s.prenume FROM STUDENTI s 
                            join note n on s.id = n.id_student 
                            join cursuri c on c.id = n.id_curs where c.titlu_curs = p_curs and n.valoare = p_nota and s.grupa = p_grupa;
    v_std_linie studenti%ROWTYPE;
    v_grupa VARCHAR2(5);
    v_nr_std NUMBER;
    v_std_nume studenti.nume%TYPE;
    v_std_prenume studenti.prenume%TYPE;
BEGIN   
    v_grupa := '&grupa';
    UPDATE note t set valoare = valoare + 1 
        WHERE t.id_student in (select id from studenti where grupa = v_grupa) 
        and valoare < 10 and t.id_curs = (select id from cursuri where TITLU_CURS = 'Baze de date');
    IF (SQL%FOUND)
        THEN
            DBMS_OUTPUT.PUT_LINE('Au fost modificate notele a '||SQL%ROWCOUNT||' studenti ');
        ELSE
            DBMS_OUTPUT.PUT_LINE('Nu a fost modificata nota niciunui student');
    END IF;
    v_nr_std := 0;
    DBMS_OUTPUT.PUT_LINE('Studentii care au 10 dupa ce notele au fost modificate:');
    OPEN lista_studenti_10 (v_grupa, 'Baze de date', 10);
    LOOP
        FETCH lista_studenti_10 INTO v_std_nume, v_std_prenume;
        EXIT WHEN lista_studenti_10%NOTFOUND;
        DBMS_OUTPUT.PUT_LINE(v_std_nume||' '||v_std_prenume);
        v_nr_std := v_nr_std + 1;
    END LOOP;
    DBMS_OUTPUT.PUT_LINE('Total studenti: '||v_nr_std);
END;

update note t set valoare = valoare + 1 where t.id_student in (select id from studenti where grupa = 'A2') and valoare < 10 and t.id_curs = (select id from cursuri where TITLU_CURS = 'Baze de date');

select * from studenti s join note n on s.id = n.id_student join cursuri c on c.id = n.id_curs where c.titlu_curs = 'Baze de date' and s.grupa = 'A2';

select count(DISTINCT s.ID) from studenti s join note n on n.id_student = s.id join cursuri c on c.id = n.id_curs where c.titlu_curs = 'Baze de date' and n.valoare = 10 and s.grupa = 'A2';

rollback;
