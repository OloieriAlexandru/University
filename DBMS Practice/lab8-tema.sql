
set serveroutput on;

DECLARE
    CURSOR note_catalog IS
        SELECT * FROM catalog;

    v_fisier UTL_FILE.FILE_TYPE;
BEGIN
    v_fisier:=UTL_FILE.FOPEN('MYDIR','catalog.xml','W');
    IF NOT UTL_FILE.IS_OPEN(v_fisier) THEN
        DBMS_OUTPUT.put_line('Eroare la crearea fisierului!');
        RETURN;
    END IF;
  
    UTL_FILE.PUT_LINE(v_fisier, '<?xml version="1.0" encoding="UTF-8"?>');
    UTL_FILE.PUT_LINE(v_fisier,'<catalog>');
    
    FOR v_linie IN note_catalog LOOP
        UTL_FILE.PUT_LINE(v_fisier, '   <nota_student_materie>');
        
        UTL_FILE.PUT_LINE(v_fisier, '       <nume>' || v_linie.nume || '</nume>');
        UTL_FILE.PUT_LINE(v_fisier, '       <prenume>' || v_linie.prenume || '</prenume>');
        UTL_FILE.PUT_LINE(v_fisier, '       <titlu_curs>' || v_linie.titlu_curs || '</titlu_curs>');
        UTL_FILE.PUT_LINE(v_fisier, '       <valoare>' || v_linie.valoare || '</valoare>');
        
        UTL_FILE.PUT_LINE(v_fisier, '   </nota_student_materie>');
    END LOOP;
  
  
    UTL_FILE.PUT_LINE(v_fisier,'</catalog>');
  
    UTL_FILE.FCLOSE(v_fisier);
END;
/
