-- Oloieri Alexandru, IIA2

-- https://pastebin.com/ZsFQVWbQ
-- Am adaugat aceasta functie aici pentru ca scriptul sa poata fi rulat in modul urmator, fara sa apara probleme: CTRL+A, CTRL+Enter
CREATE OR REPLACE FUNCTION gettype(v_rec_tab DBMS_SQL.DESC_TAB, v_nr_col int) RETURN VARCHAR2 AS
  v_tip_coloana VARCHAR2(200);
  v_precizie VARCHAR2(40);
BEGIN
     CASE (v_rec_tab(v_nr_col).col_type)
        WHEN 1 THEN v_tip_coloana := 'VARCHAR2'; v_precizie := '(' || v_rec_tab(v_nr_col).col_max_len || ')';
        WHEN 2 THEN v_tip_coloana := 'NUMBER'; v_precizie := '(' || v_rec_tab(v_nr_col).col_precision || ',' || v_rec_tab(v_nr_col).col_scale || ')';
        WHEN 12 THEN v_tip_coloana := 'DATE'; v_precizie := '';
        WHEN 96 THEN v_tip_coloana := 'CHAR'; v_precizie := '(' || v_rec_tab(v_nr_col).col_max_len || ')';
        WHEN 112 THEN v_tip_coloana := 'CLOB'; v_precizie := '';
        WHEN 113 THEN v_tip_coloana := 'BLOB'; v_precizie := '';
        WHEN 109 THEN v_tip_coloana := 'XMLTYPE'; v_precizie := '';
        WHEN 101 THEN v_tip_coloana := 'BINARY_DOUBLE'; v_precizie := '';
        WHEN 100 THEN v_tip_coloana := 'BINARY_FLOAT'; v_precizie := '';
        WHEN 8 THEN v_tip_coloana := 'LONG'; v_precizie := '';
        WHEN 180 THEN v_tip_coloana := 'TIMESTAMP'; v_precizie :='(' || v_rec_tab(v_nr_col).col_scale || ')';
        WHEN 181 THEN v_tip_coloana := 'TIMESTAMP' || '(' || v_rec_tab(v_nr_col).col_scale || ') ' || 'WITH TIME ZONE'; v_precizie := '';
        WHEN 231 THEN v_tip_coloana := 'TIMESTAMP' || '(' || v_rec_tab(v_nr_col).col_scale || ') ' || 'WITH LOCAL TIME ZONE'; v_precizie := '';
        WHEN 114 THEN v_tip_coloana := 'BFILE'; v_precizie := '';
        WHEN 23 THEN v_tip_coloana := 'RAW'; v_precizie := '(' || v_rec_tab(v_nr_col).col_max_len || ')';
        WHEN 11 THEN v_tip_coloana := 'ROWID'; v_precizie := '';
        WHEN 109 THEN v_tip_coloana := 'URITYPE'; v_precizie := '';
      END CASE;
      RETURN v_tip_coloana||v_precizie;
END;
/

/*
Operatiile pe care le fac sunt:
1. Selectez numele cursului ce are id-ul egal cu parametrul procedurii
2. Daca nu exista cursul afisez un mesaj de eroare si ma opresc
3. Daca exista cursul, construiesc numele tabelei in modul urmator: caracterele ' ' si '_' sunt inlocuite cu '_'
4. Verific, cu ajutorul unui select in tabela "all_tables", daca tabela pentru cursul curent exista
5. Daca exista, intrucat nu era specificat in enuntul temei ce trebuie facut, sterg toate inregistrarile din tabel (pentru ca apoi sa fie inserate cele curente)
6. Daca nu exista, fac un join in care selectez coloanele ce trebuie sa fie incluse in noul tabel, si extrag tipurile acestor coloane
7. Creez tabela
8. In punctul curent, sigur am un tabel cu numele potrivit ce nu are inregistrari, fac din nou un join pentru a lua studentii ce au note la materia specificata ca si parametru si inserez inregistrarile

Motivul pentru care am decis ca atunci cand o tabela exista deja, sa-i sterg inregistrarile pentru ca mai apoi sa le inserez din nou este pentru ca nu era precizat ce trebuie facut in cazul asta, si daca nu le stergeam e foarte posibil sa fi aparut duplicate
*/
CREATE OR REPLACE PROCEDURE construieste_tabel(p_id_curs NUMBER)
AS
    v_cursor_afla_nume_curs NUMBER;
    v_ok_afla_nume NUMBER;
    v_titlu_curs VARCHAR2(52);
    v_lg_titlu_curs NUMBER;

    v_cursor_id NUMBER;
    v_ok NUMBER;
    v_rec_tab DBMS_SQL.DESC_TAB;
    v_nr_col NUMBER;
    v_total_coloane NUMBER;
    v_nume_coloana VARCHAR2(100);
    
    v_cursor_verificare_existenta NUMBER;
    v_ok_verificare_existenta NUMBER;
    
    v_cursor_stergere_note NUMBER;
    v_ok_stergere_note NUMBER;
    
    v_cursor_creare NUMBER;
    v_ok_creare NUMBER;
    v_nume_tabel VARCHAR(58);
        
    v_cursor_inserare NUMBER;
    v_ok_inserare NUMBER;
        
    v_tip_nr_matricol VARCHAR2(240);
    v_tip_nume VARCHAR2(240);
    v_tip_prenume VARCHAR2(240);
    v_tip_valoare VARCHAR2(240);
    v_tip_data_notare VARCHAR2(240);
    
    v_id_nota INT;
    
    v_id INT;
    v_nr_matricol studenti.nr_matricol%TYPE;
    v_nume studenti.nume%TYPE;
    v_prenume studenti.prenume%TYPE;
    v_valoare note.valoare%TYPE;
    v_data_notare note.data_notare%TYPE;
BEGIN
    -- selectez numele cursului
    v_cursor_afla_nume_curs := DBMS_SQL.OPEN_CURSOR;
    DBMS_SQL.PARSE(v_cursor_afla_nume_curs, 'SELECT titlu_curs FROM cursuri WHERE id = ' || p_id_curs, DBMS_SQL.NATIVE);
    DBMS_SQL.DEFINE_COLUMN(v_cursor_afla_nume_curs, 1, v_titlu_curs, 52);
    v_ok_afla_nume := DBMS_SQL.EXECUTE(v_cursor_afla_nume_curs);
    
    IF DBMS_SQL.FETCH_ROWS(v_cursor_afla_nume_curs) > 0 THEN
        DBMS_SQL.COLUMN_VALUE(v_cursor_afla_nume_curs, 1, v_titlu_curs); -- extrag numele cursului (stiu ca exista)
    ELSE
        DBMS_OUTPUT.put_line('Cursul nu exista!');
        return;
    END IF;
    DBMS_SQL.close_cursor(v_cursor_afla_nume_curs);

    -- construiesc numele tabelei noi
    v_lg_titlu_curs := LENGTH(v_titlu_curs);
    v_nume_tabel := '';
    FOR v_i in 1..v_lg_titlu_curs LOOP
        IF SUBSTR(v_titlu_curs, v_i, 1) = ' ' OR SUBSTR(v_titlu_curs, v_i, 1) = '-' THEN
            v_nume_tabel := v_nume_tabel || '_';
        ELSE 
            v_nume_tabel := v_nume_tabel || SUBSTR(v_titlu_curs, v_i, 1);
        END IF;
    END LOOP;

    -- https://stackoverflow.com/a/35723066
    v_cursor_verificare_existenta := DBMS_SQL.OPEN_CURSOR;
    DBMS_SQL.PARSE(v_cursor_verificare_existenta, 'SELECT table_name FROM all_tables WHERE UPPER(table_name) = UPPER(''' || v_nume_tabel || ''')', DBMS_SQL.NATIVE);
    v_ok_verificare_existenta := DBMS_SQL.EXECUTE(v_cursor_verificare_existenta);

    IF DBMS_SQL.FETCH_ROWS(v_cursor_verificare_existenta) > 0 THEN
        -- tabela exista, voi sterge toate notele
        DBMS_OUTPUT.put_line('Tabelul pentru materia ' || v_titlu_curs || ' exista deja!');
        
        v_cursor_stergere_note := DBMS_SQL.OPEN_CURSOR;
        DBMS_SQL.PARSE(v_cursor_stergere_note, 'DELETE FROM ' || v_nume_tabel, DBMS_SQL.NATIVE);
        v_ok_stergere_note := DBMS_SQL.EXECUTE(v_cursor_stergere_note);
        DBMS_SQL.CLOSE_CURSOR(v_cursor_stergere_note);
        
    ELSE
         -- tabela nu exista, o creez
        DBMS_OUTPUT.put_line('Tabelul pentru materia ' || v_titlu_curs || ' nu exista, o creez!');
        v_cursor_id  := DBMS_SQL.OPEN_CURSOR;
        DBMS_SQL.PARSE(v_cursor_id , 'SELECT nr_matricol, nume, prenume, valoare, data_notare FROM studenti JOIN note ON studenti.id = note.id_student JOIN cursuri ON note.id_curs = cursuri.id', DBMS_SQL.NATIVE);
        v_ok := DBMS_SQL.EXECUTE(v_cursor_id);
        DBMS_SQL.DESCRIBE_COLUMNS(v_cursor_id, v_total_coloane, v_rec_tab);
        
        v_nr_col := v_rec_tab.first;
        IF (v_nr_col IS NOT NULL) THEN
            LOOP
                v_nume_coloana := UPPER(v_rec_tab(v_nr_col).col_name);
                IF v_nume_coloana = 'NR_MATRICOL' THEN
                    v_tip_nr_matricol := gettype(v_rec_tab,v_nr_col);
                ELSIF v_nume_coloana = 'NUME' THEN
                    v_tip_nume := gettype(v_rec_tab,v_nr_col);
                ELSIF v_nume_coloana = 'PRENUME' THEN
                    v_tip_prenume := gettype(v_rec_tab,v_nr_col);
                ELSIF v_nume_coloana = 'VALOARE' THEN
                    v_tip_valoare := gettype(v_rec_tab,v_nr_col);
                ELSIF v_nume_coloana = 'DATA_NOTARE' THEN
                    v_tip_data_notare := gettype(v_rec_tab,v_nr_col);
                END IF;
                v_nr_col := v_rec_tab.next(v_nr_col);
            EXIT WHEN (v_nr_col IS NULL);
            END LOOP;
        END IF;
        DBMS_SQL.CLOSE_CURSOR(v_cursor_id);
    
        -- cand ajung aici, in cele 5 variabile de mai sus am retinut tipul fiecarei coloane, pot sa creez tabela
        v_cursor_creare := DBMS_SQL.OPEN_CURSOR;
        DBMS_SQL.PARSE(v_cursor_creare, 'CREATE TABLE ' || v_nume_tabel || '( id INT PRIMARY KEY, 
                    nr_matricol ' || v_tip_nr_matricol || ', 
                    nume ' || v_tip_nume || ', 
                    prenume ' || v_tip_prenume || ', 
                    valoare ' || v_tip_valoare || ', 
                    data_notare ' || v_tip_data_notare || ')', DBMS_SQL.NATIVE);
        v_ok_creare := DBMS_SQL.EXECUTE(v_cursor_creare);
        DBMS_SQL.CLOSE_CURSOR(v_cursor_creare);
        
    END IF;
    DBMS_SQL.CLOSE_CURSOR(v_cursor_verificare_existenta);
    
    -- selectez inregistrarile ce trebuie sa fie inserate in noul tabel
    v_cursor_id  := DBMS_SQL.OPEN_CURSOR;
    DBMS_SQL.PARSE(v_cursor_id , 'SELECT nr_matricol, nume, prenume, valoare, data_notare FROM studenti JOIN note ON studenti.id = note.id_student JOIN cursuri ON note.id_curs = cursuri.id WHERE titlu_curs = '''
                                    || v_titlu_curs || '''', DBMS_SQL.NATIVE);
    DBMS_SQL.DEFINE_COLUMN(v_cursor_id, 1, v_nr_matricol, 6); 
    DBMS_SQL.DEFINE_COLUMN(v_cursor_id, 2, v_nume, 15); 
    DBMS_SQL.DEFINE_COLUMN(v_cursor_id, 3, v_prenume, 30); 
    DBMS_SQL.DEFINE_COLUMN(v_cursor_id, 4, v_valoare); 
    DBMS_SQL.DEFINE_COLUMN(v_cursor_id, 5, v_data_notare); 
    v_ok := DBMS_SQL.EXECUTE(v_cursor_id);
    
    v_id := 1;
    LOOP 
        -- iterez prin inregistrari si le inserez
        IF DBMS_SQL.FETCH_ROWS(v_cursor_id)>0 THEN 
            DBMS_SQL.COLUMN_VALUE(v_cursor_id, 1, v_nr_matricol); 
            DBMS_SQL.COLUMN_VALUE(v_cursor_id, 2, v_nume); 
            DBMS_SQL.COLUMN_VALUE(v_cursor_id, 3, v_prenume); 
            DBMS_SQL.COLUMN_VALUE(v_cursor_id, 4, v_valoare);
            DBMS_SQL.COLUMN_VALUE(v_cursor_id, 5, v_data_notare);
            
            v_cursor_inserare := DBMS_SQL.OPEN_CURSOR;
            DBMS_SQL.PARSE(v_cursor_inserare, 'INSERT INTO ' || v_nume_tabel || ' (id, nr_matricol, nume, prenume, valoare, data_notare) VALUES ('
               || v_id || ', ''' || v_nr_matricol || ''', ''' || v_nume || ''', ''' || v_prenume || ''', ' || v_valoare || ', ''' || v_data_notare || ''')', DBMS_SQL.NATIVE);
            v_ok_inserare := DBMS_SQL.EXECUTE(v_cursor_inserare);
            DBMS_SQL.CLOSE_CURSOR(v_cursor_inserare);
            
            v_id := v_id + 1;
            -- DBMS_OUTPUT.put_line(v_nr_matricol || ' ' || v_nume || ' ' || v_prenume || ' ' || v_valoare || ' ' || v_data_notare);
        ELSE 
            EXIT; 
        END IF; 
    END LOOP;
    
    DBMS_SQL.CLOSE_CURSOR(v_cursor_id);
END;
/

set serveroutput on;
BEGIN
    construieste_tabel(5); -- creez tabela pentru Sisteme de operare
    construieste_tabel(10); -- creez tabela pentru Baze de date
    construieste_tabel(-1);
END;
/