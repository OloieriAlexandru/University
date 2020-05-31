
-- Oloieri Alexandru IIA2

-- https://docs.oracle.com/cd/B19306_01/server.102/b14237/statviews_1037.htm#i1576022
CREATE OR REPLACE FUNCTION afla_tip_constrangere(p_tip VARCHAR2)
RETURN VARCHAR2 AS
BEGIN
    IF p_tip = 'C' THEN
        return 'check constraint on a table';
    ELSIF p_tip = 'P' THEN
        return 'primary key';
    ELSIF p_tip = 'U' THEN
        return 'unique key';
    ELSIF p_tip = 'R' THEN
        return 'referential integrity';
    ELSIF p_tip = 'V' THEN
        return 'with check option, on a view';
    ELSIF p_tip = 'O' THEN
        return 'with read only, on a view';
    END IF;
    return 'unknown';
END;
/

/*
Pentru tabele afisez: nume, numar inregistrari, daca e nested
    + constrangeri (nume, tip, coloane)
    + indecsi (nume, tip, unicitate, coloane)
    
Pentru view-uri afisez: nume, status

Pentru indecsi afisez: nume, tip, tabelul, unicitate, nr. chei distincte, status

Pentru tipuri afisez: nume, tip, numar atribute, numar metode
    + daca e predefinit, final, instantiabil
    
Pentru pachete afisez: nume, numar linii din antet, numar linii din body

Pentru functii si proceduri afisez: nume, numar linii, daca sunt deterministe

---- Pentru fiecare dintre acestea am folosit unul sau mai multe cursoare ----
*/
set serveroutput on;
DECLARE
    v_tabel_nume user_tables.table_name%TYPE;
    v_tabel_inregistrari_nr user_tables.num_rows%TYPE;
    v_tabel_nested user_tables.nested%TYPE;
    CURSOR statistici_tabele IS
        SELECT table_name, num_rows, nested INTO v_tabel_nume, v_tabel_inregistrari_nr, v_tabel_nested
            FROM user_tables;

    v_tabel_constrangere_nume user_constraints.constraint_name%TYPE;
    v_tabel_constrangere_tip user_constraints.constraint_type%TYPE;
    CURSOR statistici_constrangeri_tabel(p_nume_tabel VARCHAR2) IS
        SELECT constraint_name, constraint_type INTO v_tabel_constrangere_nume, v_tabel_constrangere_tip
            FROM user_constraints 
            WHERE UPPER(table_name) = UPPER(p_nume_tabel) 
            GROUP BY constraint_name, constraint_type;

    v_tabel_constrangere_coloana user_cons_columns.column_name%TYPE;
    CURSOR statistici_col_const_tabel(p_nume_tabel VARCHAR2, p_nume_constrangere VARCHAR2) IS
        SELECT column_name INTO v_tabel_constrangere_coloana
            FROM user_cons_columns
            WHERE constraint_name = p_nume_constrangere AND table_name = p_nume_tabel;

    v_tabel_index_nume user_indexes.index_name%TYPE;
    v_tabel_index_unic user_indexes.uniqueness%TYPE;
    v_tabel_index_tip user_indexes.index_type%TYPE;
    CURSOR statistici_indecsi_tabel(p_nume_tabel VARCHAR2) IS
        SELECT index_name, uniqueness, index_type INTO v_tabel_index_nume, v_tabel_index_unic, v_tabel_index_tip
            FROM user_indexes
            WHERE UPPER(table_name) = UPPER(p_nume_tabel)
            GROUP BY index_name, index_type, uniqueness;

    v_tabel_index_coloana user_ind_columns.column_name%TYPE;
    CURSOR statistici_col_indecsi_tabel(p_nume_tabel VARCHAR2, p_nume_index VARCHAR2) IS
        SELECT column_name INTO v_tabel_index_coloana
            FROM user_ind_columns
            WHERE index_name = p_nume_index AND table_name = p_nume_tabel;

    v_view_nume user_objects.object_name%TYPE;
    v_view_status user_objects.status%TYPE;
    CURSOR statistici_views IS
        SELECT object_name, status INTO v_view_nume, v_view_status
            FROM user_objects
            WHERE object_type = 'VIEW';

    v_index_nume user_indexes.index_name%TYPE;
    v_index_tip user_indexes.index_type%TYPE;
    v_index_tabela user_indexes.table_name%TYPE;
    v_index_unic user_indexes.uniqueness%TYPE;
    v_index_chei_distincte user_indexes.distinct_keys%TYPE;
    v_index_status user_indexes.status%TYPE;
    CURSOR statistici_indecsi IS
        SELECT index_name, index_type, table_name, uniqueness, distinct_keys, status INTO v_index_nume, v_index_tip, v_index_tabela, v_index_unic, v_index_chei_distincte, v_index_status
            FROM user_indexes;

    v_tip_nume user_types.type_name%TYPE;
    v_tip_tip user_types.typecode%TYPE;
    v_tip_nr_atribute user_types.attributes%TYPE;
    v_tip_nr_metode user_types.methods%TYPE;
    v_tip_predefinita user_types.predefined%TYPE;
    v_tip_final user_types.final%TYPE;
    v_tip_instantiabil user_types.instantiable%TYPE;
    CURSOR statistici_tipuri IS
        SELECT type_name, typecode, attributes, methods, predefined, final, instantiable INTO v_tip_nume, v_tip_tip, v_tip_nr_atribute, v_tip_nr_metode, v_tip_predefinita, v_tip_final, v_tip_instantiabil
            FROM user_types;

    v_pachet_nume user_objects.object_name%TYPE;
    v_pachet_antet_linii user_source.line%TYPE;
    v_pachet_corp_linii user_source.line%TYPE;
    CURSOR statistici_pachete IS
        SELECT user_objects.object_name, MAX(user_source.line) INTO v_pachet_nume, v_pachet_antet_linii
            FROM user_objects 
            JOIN user_source 
            ON user_objects.object_name = user_source.name 
            WHERE object_type  = 'PACKAGE' AND user_source.TYPE = 'PACKAGE' 
            GROUP BY user_objects.object_name, user_source.type;

    v_fct_proc_name user_source.name%TYPE;
    v_fct_proc_lines user_source.line%TYPE;
    v_fct_proc_det user_procedures.deterministic%TYPE;
    CURSOR statistici_fct_proc(p_type VARCHAR2) IS
        SELECT user_source.name, max(user_source.line), user_procedures.deterministic INTO v_fct_proc_name, v_fct_proc_lines, v_fct_proc_det
            FROM user_source 
            JOIN user_procedures 
            ON user_source.name = user_procedures.object_name 
            WHERE UPPER(user_source.type) = p_type
            GROUP BY user_source.name, user_source.type, user_procedures.deterministic;    

    v_i INT;
    v_j INT;
BEGIN
    v_i := 1;
    DBMS_OUTPUT.put_line('Tabelele definite de utilizator:');
    OPEN statistici_tabele;
    LOOP
        FETCH statistici_tabele INTO v_tabel_nume, v_tabel_inregistrari_nr, v_tabel_nested;
        EXIT WHEN statistici_tabele%NOTFOUND;
        IF UPPER(v_tabel_nested) = 'YES' THEN
            v_tabel_nested := 'da';
        ELSE
            v_tabel_nested := 'nu';
        END IF;
        DBMS_OUTPUT.put_line(v_i || '. nume: ' || v_tabel_nume || ', nr. inregistrari: ' || v_tabel_inregistrari_nr || ', nested: ' || v_tabel_nested);
        
        DBMS_OUTPUT.put_line('  Constrangerile tabelului:');
        v_j := 1;
        OPEN statistici_constrangeri_tabel(v_tabel_nume);
        LOOP
            FETCH statistici_constrangeri_tabel INTO v_tabel_constrangere_nume, v_tabel_constrangere_tip;
            EXIT WHEN statistici_constrangeri_tabel%NOTFOUND;
            DBMS_OUTPUT.put('  ' || v_j || '. nume constrangere: ' || v_tabel_constrangere_nume || ', tip constrangere: ' || afla_tip_constrangere(v_tabel_constrangere_tip) || ', coloane: ');
            
            OPEN statistici_col_const_tabel(v_tabel_nume, v_tabel_constrangere_nume);
            LOOP
                FETCH statistici_col_const_tabel INTO v_tabel_constrangere_coloana;
                EXIT WHEN statistici_col_const_tabel%NOTFOUND;
                DBMS_OUTPUT.put(v_tabel_constrangere_coloana || ', ');
            END LOOP;
            CLOSE statistici_col_const_tabel;
            DBMS_OUTPUT.put_line('');
            
            v_j := v_j + 1;
        END LOOP;
        CLOSE statistici_constrangeri_tabel;
        
        DBMS_OUTPUT.put_line('  Indecsii tabelului:');
        v_j := 1;
        OPEN statistici_indecsi_tabel(v_tabel_nume);
        LOOP
            FETCH statistici_indecsi_tabel INTO v_tabel_index_nume, v_tabel_index_unic, v_tabel_index_tip;
            EXIT WHEN statistici_indecsi_tabel%NOTFOUND;
            DBMS_OUTPUT.put('  ' || v_j || '. nume index: ' || v_tabel_index_nume || ', tip index: ' || v_tabel_index_tip || ', unicitate: ' || v_tabel_index_unic || ', coloane: ');

            OPEN statistici_col_indecsi_tabel(v_tabel_nume, v_tabel_index_nume);
            LOOP
                FETCH statistici_col_indecsi_tabel INTO v_tabel_index_coloana;
                EXIT WHEN statistici_col_indecsi_tabel%NOTFOUND;
                DBMS_OUTPUT.put(v_tabel_index_coloana || ', ');
            END LOOP;
            CLOSE statistici_col_indecsi_tabel;
            DBMS_OUTPUT.put_line('');

            v_j := v_j + 1;
        END LOOP;
        CLOSE statistici_indecsi_tabel;
        
        v_i := v_i + 1;
    END LOOP;
    CLOSE statistici_tabele;
    DBMS_OUTPUT.put_line('');

    v_i := 1;
    DBMS_OUTPUT.put_line('View-urile definite de utilizator:');
    OPEN statistici_views;
    LOOP
        FETCH statistici_views INTO v_view_nume, v_view_status;
        EXIT WHEN statistici_views%NOTFOUND;
        DBMS_OUTPUT.put_line(v_i || '. nume: ' || v_view_nume || ', status: ' || LOWER(v_view_status));
        v_i := v_i + 1;
    END LOOP;
    CLOSE statistici_views;
    DBMS_OUTPUT.put_line('');

    v_i := 1;
    DBMS_OUTPUT.put_line('Indecsii definiti de utilizator:');
    OPEN statistici_indecsi;
    LOOP
        FETCH statistici_indecsi INTO v_index_nume, v_index_tip, v_index_tabela, v_index_unic, v_index_chei_distincte, v_index_status;
        EXIT WHEN statistici_indecsi%NOTFOUND;
        DBMS_OUTPUT.put_line(v_i || '. nume: ' || v_index_nume || ', tip: ' || LOWER(v_index_tip) || ', tabel: ' || v_index_tabela || ', unicitate: ' || v_index_unic || ', nr. chei distincte: ' || v_index_chei_distincte || ', status: ' || LOWER(v_view_status));
        v_i := v_i + 1;
    END LOOP;
    CLOSE statistici_indecsi;
    DBMS_OUTPUT.put_line('');

    v_i := 1;
    DBMS_OUTPUT.put_line('Tipurile definite de utilizator:');
    OPEN statistici_tipuri;
    LOOP
        FETCH statistici_tipuri INTO v_tip_nume, v_tip_tip, v_tip_nr_atribute, v_tip_nr_metode, v_tip_predefinita, v_tip_final, v_tip_instantiabil;
        EXIT WHEN statistici_tipuri%NOTFOUND;
        IF v_tip_predefinita = 'NO' THEN
            v_tip_predefinita := 'nu';
        ELSE
            v_tip_predefinita := 'da';
        END IF;
        IF v_tip_final = 'NO' THEN
            v_tip_final := 'nu';
        ELSE
            v_tip_final := 'da';
        END IF;
        IF v_tip_instantiabil = 'NO' THEN
            v_tip_instantiabil := 'nu';
        ELSE
            v_tip_instantiabil := 'da';
        END IF;
        DBMS_OUTPUT.put_line(v_i || '. nume: ' || v_tip_nume || ', tip: ' || LOWER(v_tip_tip) || ', nr. atribute: ' || v_tip_nr_atribute || ', nr. metode: ' || v_tip_nr_metode || ', predefinit: ' || v_tip_predefinita || ', final: ' || v_tip_final || ', instantiabil: ' || v_tip_instantiabil);
        v_i := v_i + 1;
    END LOOP;
    CLOSE statistici_tipuri;
    DBMS_OUTPUT.put_line('');

    v_i := 1;
    DBMS_OUTPUT.put_line('Pachetele definite de utilizator:');
    OPEN statistici_pachete;
    LOOP
        FETCH statistici_pachete INTO v_pachet_nume, v_pachet_antet_linii;
        EXIT WHEN statistici_pachete%NOTFOUND;
        SELECT MAX(line) INTO v_pachet_corp_linii FROM user_source WHERE type = 'PACKAGE BODY' AND name = v_pachet_nume;
        DBMS_OUTPUT.put_line(v_i || '. nume: ' || v_pachet_nume || ', nr. linii antet: ' || v_pachet_antet_linii || ', nr. linii body: ' || v_pachet_corp_linii);
        v_i := v_i + 1;
    END LOOP;
    CLOSE statistici_pachete;
    DBMS_OUTPUT.put_line('');

    v_i := 1;
    DBMS_OUTPUT.put_line('Functiile definite de utilizator:');
    OPEN statistici_fct_proc('FUNCTION');
    LOOP
        FETCH statistici_fct_proc INTO v_fct_proc_name, v_fct_proc_lines, v_fct_proc_det;
        EXIT WHEN statistici_fct_proc%NOTFOUND;
        IF UPPER(v_fct_proc_det) = 'YES' THEN
            v_fct_proc_det := 'da';
        ELSE
            v_fct_proc_det := 'nu';
        END IF;
        DBMS_OUTPUT.put_line(v_i || '. nume: ' || v_fct_proc_name || ', nr linii: ' || v_fct_proc_lines || ', determinista: ' || v_fct_proc_det);
        v_i := v_i + 1;
    END LOOP;
    CLOSE statistici_fct_proc;
    DBMS_OUTPUT.put_line('');

    v_i := 1;
    DBMS_OUTPUT.put_line('Procedurile definite de utilizator:');
    OPEN statistici_fct_proc('PROCEDURE');
    LOOP
        FETCH statistici_fct_proc INTO v_fct_proc_name, v_fct_proc_lines, v_fct_proc_det;
        EXIT WHEN statistici_fct_proc%NOTFOUND;
        IF UPPER(v_fct_proc_det) = 'YES' THEN
            v_fct_proc_det := 'da';
        ELSE
            v_fct_proc_det := 'nu';
        END IF;
        DBMS_OUTPUT.put_line(v_i || '. nume: ' || v_fct_proc_name || ', nr linii: ' || v_fct_proc_lines || ', determinista: ' || v_fct_proc_det);
        v_i := v_i + 1;
    END LOOP;
    CLOSE statistici_fct_proc;
END;
/
